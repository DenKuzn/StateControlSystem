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
};
