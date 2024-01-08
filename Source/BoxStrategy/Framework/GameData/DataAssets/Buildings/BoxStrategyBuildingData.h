// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"

#include "ObjectDescriptionPlugin/Framework/ObjectDescriptionStruct.h"

#include "BoxStrategyBuildingData.generated.h"

class ABoxStrategyBuilding;

/** Здание в игре. Может быть зданием игрока, может быть зданием при генерации карты или зданием, которое построит ИИ.
 */
UCLASS(abstract)
class BOXSTRATEGY_API UBoxStrategyBuildingData : public UDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FObjectDescription BuildingDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer BuildingTags;

	// Building Blueprint for spawn when player have chosen place for building
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<ABoxStrategyBuilding> BuildingBplueprintReference = nullptr;

	// Building Mesh used for palce on map when player chose place for building
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UStaticMesh> BuildingMesh = nullptr;
	
};
