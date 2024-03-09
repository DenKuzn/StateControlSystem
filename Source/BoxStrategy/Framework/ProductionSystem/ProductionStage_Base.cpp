// Fill out your copyright notice in the Description page of Project Settings.


#include "ProductionStage_Base.h"
#include "GeneralDebugMacroses/Framework/DebugMacroses.h"
#include "ProductDataAssets/BoxStrategyProductData_Base.h"


UWorld* UProductionStage_Base::GetWorld() const
{
	if ( HasAllFlags( RF_ClassDefaultObject ) )
	{
		// If we are a CDO, we must return nullptr instead of calling Outer->GetWorld() to fool UObject::ImplementsGetWorld.
		return nullptr;
	}
	return GetOuter()->GetWorld();
}

void UProductionStage_Base::InitializationStage(UBoxStrategyProductData_Base* NewProductData)
{
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( NewProductData, );
#endif
	}

	ProductionData = NewProductData;
	ProductionStageState = EProductionStageState::WaitActivation;
}

void UProductionStage_Base::SetNextStage(UProductionStage_Base* NewNextStage)
{
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( NewNextStage, );
#endif
	}

	NextProductionStage = NewNextStage;
	NextProductionStage->OnStageUpdated.		AddDynamic( this, &UProductionStage_Base::OnNextStageUpdated );
	NextProductionStage->OnStageChainComplete.	AddDynamic( this, &UProductionStage_Base::OnLastStageInChainCompleted );
	NextProductionStage->OnStageCanceled.		AddDynamic( this, &UProductionStage_Base::OnNextStageCanceled );
	NextProductionStage->OnStageCompleted.		AddDynamic( this, &UProductionStage_Base::OnNewProductionStageCompleted );
	NextProductionStage->OnStageActivated.		AddDynamic( this, &UProductionStage_Base::OnNewProductionStageActivated );

}

void UProductionStage_Base::ExecuteProduction()
{
	//TODO: ADd validation for ProductData
	ProductionStageState = EProductionStageState::Active;
	K2_ActivateExecution();
	OnStageActivated.Broadcast(this);
}

void UProductionStage_Base::StopProductionExecution()
{
	// Если следующий этап ожидает активации, то его можно не останавливать, так как он ничего не должен сделать до активации вобще.
	// Все ниже по цепочке этапы в этом случае так же не запущены и могут быть проигнорированны.
	if ( IsValid( NextProductionStage ) && ( NextProductionStage->GetProductionStageState() != EProductionStageState::Canceled ) )

	{
		NextProductionStage->OnStageUpdated.		RemoveDynamic( this, &UProductionStage_Base::OnNextStageUpdated );
		NextProductionStage->OnStageChainComplete.	RemoveDynamic( this, &UProductionStage_Base::OnLastStageInChainCompleted );
		NextProductionStage->OnStageCanceled.		RemoveDynamic( this, &UProductionStage_Base::OnNextStageCanceled );
		NextProductionStage->OnStageCompleted.		RemoveDynamic( this, &UProductionStage_Base::OnNewProductionStageCompleted );
		NextProductionStage->OnStageActivated.		RemoveDynamic( this, &UProductionStage_Base::OnNewProductionStageActivated );
		NextProductionStage->StopProductionExecution();
	}

	K2_CancelExecution();
	ProductionStageState = EProductionStageState::Canceled;
	OnStageCanceled.Broadcast();
}

void UProductionStage_Base::SetNewPriority(int32 NewPriority)
{
}

AActor* UProductionStage_Base::GetOwnerActor()
{
	return Cast<AActor>(GetOuter());
}

float UProductionStage_Base::GetStageProgress_Implementation()
{
	ensureAlwaysMsgf( false, TEXT("%s(). Do not forget override this function"), *FString(__FUNCTION__) );
	return 0.f;
}

UProductionStage_Base* UProductionStage_Base::GetCurrentActiveStage()
{
	if(GetProductionStageState() == EProductionStageState::Active)
	{
		return this;
	}
	else if ( IsValid( NextProductionStage ) )
	{
		return NextProductionStage->GetCurrentActiveStage();
	}

	UE_LOG(LogTemp, Error, TEXT("%s(). Error. No active stage found."), *FString(__FUNCTION__));
	return nullptr;

}

FText UProductionStage_Base::GetStageRequirements_Implementation()
{
	ensureAlwaysMsgf( false, TEXT("%s(). Do not forget override this function"), *FString(__FUNCTION__) );
	return FText{};
}

int32 UProductionStage_Base::GetStageTopID()
{
	if ( IsValid( NextProductionStage ) )
	{
		return 1 + NextProductionStage->GetStageTopID();
	}
	else
	{
		return 1;
	}
}

void UProductionStage_Base::GetAllStageStates(TArray<EProductionStageState>& OutStates)
{
	if ( IsValid( NextProductionStage ) )
	{
		NextProductionStage->GetAllStageStates( OutStates );
	}

	OutStates.EmplaceAt(0, ProductionStageState );
}

void UProductionStage_Base::CurrentStageCompleted()
{
	ProductionStageState = EProductionStageState::Finished;
	OnStageCompleted.Broadcast(this);

	if ( IsValid( NextProductionStage ) )
	{
		NextProductionStage->ExecuteProduction();
	}
	else // if no next stage
	{
		OnStageChainComplete.Broadcast();
	}
}





void UProductionStage_Base::OnLastStageInChainCompleted()
{
	OnStageChainComplete.Broadcast();
}

void UProductionStage_Base::OnNextStageCanceled()
{
	StopProductionExecution();
}

void UProductionStage_Base::OnNewProductionStageActivated(UProductionStage_Base* NewProductionStageActivated)
{
	OnStageActivated.Broadcast(NewProductionStageActivated);
}

void UProductionStage_Base::OnNewProductionStageCompleted(UProductionStage_Base* NewProductionStageCompleted)
{
	OnStageCompleted.Broadcast(NewProductionStageCompleted);
}

void UProductionStage_Base::OnNextStageUpdated(UProductionStage_Base* CurrentStage)
{
	OnStageUpdated.Broadcast(CurrentStage);
}


