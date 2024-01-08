// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#include "BoxStrategyAssetManager.h"

#include "BoxStrategy/Global/BoxStrategyNativeGameplayTags.h"

void UBoxStrategyAssetManager::StartInitialLoading()
{
	FBoxStrategyNativeGameplayTags::InitializeNativeTags();

	Super::StartInitialLoading();
}

UBoxStrategyAssetManager& UBoxStrategyAssetManager::Get()
{
	UBoxStrategyAssetManager* This = Cast<UBoxStrategyAssetManager>( GEngine->AssetManager );

	if ( This )
	{
		return *This;
	}
	else
	{
		UE_LOG( LogTemp, Fatal, TEXT("%s(). Invalid AssetManager"), *FString(__FUNCTION__) );
		return *NewObject<UBoxStrategyAssetManager>();
	}
}
