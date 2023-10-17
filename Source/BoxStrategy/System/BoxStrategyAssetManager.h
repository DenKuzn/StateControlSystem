// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
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

	/** Добавить нужно делегат, который будет вызываться в объекте, который запросил подгрузку ассета. */

	template <class Type>
	void AsyncLoadAsset(TSoftObjectPtr<Type> SoftObjectPtr,
						TSharedPtr<FStreamableHandle>& OutHandle,
						FStreamableManager& StreamableManager,
						Type** Outer,
						FStreamableDelegate OnLoadedObjectDelegate,
						FString AssetNameForDebug = "UnknownAssetType")
	{
		OutHandle.Reset();
		OutHandle = nullptr;

		if ( IsValid( SoftObjectPtr.Get() ) )
		{
			*Outer = Cast<Type>( SoftObjectPtr.Get() );
			return;
		}

		if ( SoftObjectPtr.IsPending() )
		{
			FStreamableDelegate OnLoaded;
			OnLoaded.BindLambda( [this, &TempOutHandle = OutHandle, &Outer = *Outer, &OnLoadedObjectDelegate = OnLoadedObjectDelegate]() { HandleLoadedAsset( TempOutHandle, &Outer, OnLoadedObjectDelegate ); } );
			OutHandle = StreamableManager.RequestAsyncLoad( SoftObjectPtr.ToSoftObjectPath(), OnLoaded );
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("%s(). SoftObjectPtr (%s type) Invalid"), *FString(__FUNCTION__), *AssetNameForDebug);
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

		UE_LOG( LogTemp, Error, TEXT("%s(). Is Valid Bullet: %s"), *FString(__FUNCTION__), TEXT_TRUE_FALSE( IsValid( *Outer ) ) );
	}
};
