// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#include "BoxStrategyAssetManager.h"

#include "BoxStrategy/Global/BoxStrategyNativeGameplayTags.h"

void UBoxStrategyAssetManager::StartInitialLoading()
{
	FBoxStrategyNativeGameplayTags::InitializeNativeTags();

	Super::StartInitialLoading();
}
