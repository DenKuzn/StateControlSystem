// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/ProductionSystem/ProductDataAssets/BoxStrategyProductData_Base.h"

#include "PickableItemData_Base.generated.h"

/**
 *
 */
UCLASS()
class BOXSTRATEGY_API UPickableItemData_Base : public UBoxStrategyProductData_Base
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PickableItem")
	TSoftObjectPtr<UStaticMesh> ItemMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PickableItem")
	TSoftObjectPtr<UMaterialInterface> ItemMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PickableItem")
	bool bCanStock = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PickableItem")
	int32 MaxStock = 10;
};
