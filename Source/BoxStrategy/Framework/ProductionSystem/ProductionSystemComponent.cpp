// Fill out your copyright notice in the Description page of Project Settings.


#include "ProductionSystemComponent.h"
#include "GeneralDebugMacroses/Framework/DebugMacroses.h"
#include "ProductDataAssets/BoxStrategyProductData_Base.h"


// Sets default values for this component's properties
UProductionSystemComponent::UProductionSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UProductionSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UProductionSystemComponent::CanStartNewProduction() const
{
	return !IsValid( CurrentProductData );
}

void UProductionSystemComponent::GetCurrentStageDescription(FObjectDescription& StageDescription) const
{
	if ( IsValid( FirstStage ) && IsValid( FirstStage->GetCurrentActiveStage() ) )
	{
		StageDescription = FirstStage->GetCurrentActiveStage()->GetStageDescription();
		return;
	}

	UE_LOG( LogTemp, Warning, TEXT("%s(). Someone tried to get Description but No stage available"), *FString(__FUNCTION__) );
}

void UProductionSystemComponent::AddNextProduct(UBoxStrategyProductData_Base* NewProductData)
{
	{
#if GAME_DEBUG_BUILDS
		if ( !CanStartNewProduction() )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !CanStartNewProduction()"), *FString(__FUNCTION__) );
			return;
		}

		if ( !IsValid( NewProductData ) )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !IsValid( NewProductData )"), *FString(__FUNCTION__) );
			return;
		}

		if ( NewProductData->ProductionStages.Num() == 0 )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). NewProductData->ProductionStages.Num() == 0"), *FString(__FUNCTION__) );
			return;
		}
#endif
	}

	CurrentProductData = NewProductData;
}

void UProductionSystemComponent::ActivateProduction()
{
	{
#if GAME_DEBUG_BUILDS
		if ( !IsValid( CurrentProductData ) )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !IsValid( NewProductData )"), *FString(__FUNCTION__) );
			return;
		}
#endif
	}

	UProductionStage_Base* LastStage = nullptr;
	for ( auto& StageClass : CurrentProductData->ProductionStages )
	{
		UProductionStage_Base* NewStage = NewObject<UProductionStage_Base>( GetOwner(), StageClass );
		NewStage->InitializationStage( CurrentProductData );

		// Если последний этап не валиден, значит, еще не было вообще никаких этапов.
		if ( !IsValid( LastStage ) )
		{
			FirstStage = NewStage;
			LastStage = NewStage;
			FirstStage->OnStageActivated.		AddDynamic( this, &UProductionSystemComponent::StageActivated );
			FirstStage->OnStageCompleted.		AddDynamic( this, &UProductionSystemComponent::StageCompleted );
			FirstStage->OnStageChainComplete.	AddDynamic( this, &UProductionSystemComponent::StageChainComplete );
			FirstStage->OnStageUpdated.			AddDynamic( this, &UProductionSystemComponent::StageUpdated );
		}
		else
		{
			LastStage->SetNextStage( NewStage );
			LastStage = NewStage;
		}
	}

	FirstStage->ExecuteProduction();
	OnProductionStarted.Broadcast();
}

void UProductionSystemComponent::CancelProduction()
{
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( FirstStage, );
#endif
	}

	FirstStage->OnStageActivated.		RemoveDynamic( this, &UProductionSystemComponent::StageActivated );
	FirstStage->OnStageCompleted.		RemoveDynamic( this, &UProductionSystemComponent::StageCompleted );
	FirstStage->OnStageChainComplete.	RemoveDynamic( this, &UProductionSystemComponent::StageChainComplete );
	FirstStage->OnStageUpdated.			RemoveDynamic( this, &UProductionSystemComponent::StageUpdated );
	FirstStage->StopProductionExecution();

	OnProductionCanceled.Broadcast();
	FirstStage = nullptr;
	CurrentProductData = nullptr;
}

void UProductionSystemComponent::StageActivated(UProductionStage_Base* CurrentStage)
{
	OnStageStarted.Broadcast( CurrentStage );
}

void UProductionSystemComponent::StageCompleted(UProductionStage_Base* CurrentStage)
{
	OnStageCompleted.Broadcast( CurrentStage );
}

void UProductionSystemComponent::StageChainComplete()
{
	OnProductionCompleted.Broadcast(); // Сначала уведомляем о завершении всей цепочки, чтобы все, кому нужно, считали последний раз всю информацию.
	FirstStage = nullptr;
	CurrentProductData = nullptr;
}

void UProductionSystemComponent::StageUpdated(UProductionStage_Base* CurrentStage)
{
	int32 StagesNum = CurrentProductData->ProductionStages.Num();
	int32 CurrentStageIndex = CurrentStage->GetStageTopID();
	float Progress = ( ( StagesNum - CurrentStageIndex ) + CurrentStage->GetStageProgress() ) / StagesNum;


	OnProductionProgressUpdated.Broadcast( Progress );
}

