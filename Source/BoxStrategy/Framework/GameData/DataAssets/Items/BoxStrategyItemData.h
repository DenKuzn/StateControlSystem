// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"

#include "ObjectDescriptionPlugin/Framework/ObjectDescriptionStruct.h"

#include "BoxStrategyItemData.generated.h"


/**
 * Представляет собой все элементы в игре. Например, оружие, броня, еда, ресурсы, и так далее.
 */
UCLASS(BlueprintType)
class BOXSTRATEGY_API UBoxStrategyItemData : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FObjectDescription ItemDescription;

	/** Определяют суть этого ресурса.
	 * - Тип ресурса. Производственный, природный.
	 * - Действия, которые можно сделать с этим ресурсом (добывать, искать, готовить и так далее).
	 * - Какой тип слота использования для персонажа.
	 *		- Тип Одежды (штаны, куртка).
	 *		- Тип использования рук (одноручный, двуручный).
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer ResourceTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UStaticMesh> ItemMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanStock = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxStock = 99999999;


	
};
