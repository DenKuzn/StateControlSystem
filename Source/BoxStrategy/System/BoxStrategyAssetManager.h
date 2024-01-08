// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"

#include "GeneralDebugMacroses/Framework/DebugMacroses.h"

#include "BoxStrategyAssetManager.generated.h"

/** Нужно для того, чтобы в редакторе можно использовать свои ассеты более гибко.
 *
 */

UCLASS()
class BOXSTRATEGY_API UBoxStrategyAssetManager : public UAssetManager
{
	GENERATED_BODY()

	virtual void StartInitialLoading() override;

public:

	static UBoxStrategyAssetManager& Get();

public:

	template <class Type>
	void AsyncLoadAsset(TSoftObjectPtr<Type> SoftObjectPtr,
						Type** Outer,
						FStreamableDelegate OnLoadedObjectDelegate,
						FString AssetNameForDebug = "UnknownAssetType")
	{

		//OutHandle.Reset();
		//OutHandle = nullptr;

		if ( ::IsValid( SoftObjectPtr.Get() ) )
		{
			*Outer = Cast<Type>( SoftObjectPtr.Get() );
			UE_LOG(LogTemp, Error, TEXT("%s(). Object is already loaded. AssetName: %s."), *FString(__FUNCTION__), *AssetNameForDebug);
			OnLoadedObjectDelegate.ExecuteIfBound();
			return;
		}

		if ( SoftObjectPtr.IsPending() )
		{
			TSharedPtr<FStreamableHandle> NewStreambleHandle;
			FStreamableDelegate OnLoaded;
			OnLoaded.BindLambda( [this, &TempOutHandle = NewStreambleHandle, &Outer = *Outer, &OnLoadedObjectDelegate = OnLoadedObjectDelegate]()
			{
				HandleLoadedAsset( TempOutHandle, &Outer, OnLoadedObjectDelegate );
			} );
			NewStreambleHandle = Get().GetStreamableManager().RequestAsyncLoad( SoftObjectPtr.ToSoftObjectPath(), OnLoaded );
			return;
		}

		UE_LOG(LogTemp, Error, TEXT("%s(). SoftObjectPtr (%s type) Invalid"), *FString(__FUNCTION__), *AssetNameForDebug);
		OnLoadedObjectDelegate.ExecuteIfBound();
	}




private:

	template <class Type>
	void HandleLoadedAsset(TSharedPtr<FStreamableHandle>& StreamableHandle,
	                       Type** Outer,
	                       FStreamableDelegate OnLoadedObjectDelegate)
	{
		*Outer = Cast<Type>( StreamableHandle->GetLoadedAsset() );
		StreamableHandle.Reset();
		OnLoadedObjectDelegate.ExecuteIfBound();

		UE_LOG( LogTemp, Error, TEXT("%s(). Is Valid Bullet: %s"), *FString(__FUNCTION__), TEXT_TRUE_FALSE( ::IsValid( *Outer ) ) );
	}
};
