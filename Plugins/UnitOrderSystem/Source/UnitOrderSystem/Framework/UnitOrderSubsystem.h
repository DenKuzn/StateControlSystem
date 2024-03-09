// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UnitData/UnitOrderAbilityData.h"
#include "UnitData/UnitOrderUnitTypeData.h"
#include "UnitOrderSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FUnitOrderSubSystemDelegate );

struct FUnitOrderUnitSubtype;
struct FUnitOrderUnitType;

/* All Data must me stored here and used from here. Components must not have any data, only asking from here.
*  It allows us to control and manage all data in one place.
*  Зачем:
*	- Хранит все DA и всю рид-онли базовую информацию по всем юнитам в игре.
*	- Экономит время на расчеты и запросы со стороны компонентов.
*	- Все данные хранятся только здесь - все компоненты при смене подтипа юнита обращаются вновь сюда и запрашивают новые данные.
*	- Все данные при добавлении сюда проверяются на ошибки, тем самым создавая безопасную среду хранения.
*
*  Не хранит в себе никакие дополнительные ассеты, кроме структур, считанных с PrimaryDataAssets. Все текстуры, модели и прочее хранится в SoftObjectPtr, поэтому не будут подгружены вместе с PAD.
 */
UCLASS()
class UNITORDERSYSTEM_API UUnitOrderSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	/** WARNING. If in the project no one UnitType exists - All other DataAssets will not load. It counts for every step down.
	 * If DataAssets are not found all lower steps wiil be canceled. It is still counts like a done initialization */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;


private:

	UPROPERTY()
	TArray<FUnitOrderUnitType> AllFullUnitTypeData;

	bool bAllUnitDataInitialized = false;

public:

	// Delegates for messages when Unit Data is loaded.
	UPROPERTY(BlueprintAssignable, Category = "UnitOrderSubsystem")
	FUnitOrderSubSystemDelegate LoadingUnitDataComplited;

public:

	bool IsAllUnitDataInitialized() const { return bAllUnitDataInitialized; };

	UFUNCTION(BlueprintCallable, Category = "UnitOrderSubsystem")
	bool GetUnitAbilityData(const FGameplayTag& UnitTypeTag, const FGameplayTag& AbilityTag, FUnitOrderAbilityData& OutAbilityData);

	UFUNCTION( BlueprintPure, Category = "UnitOrderSubsystem" )
	UUnitOrderUnitSubtypeData* GetUnitSubtypeData(const FGameplayTag& UnitSubtypeTag);

	UFUNCTION( BlueprintPure, Category = "UnitOrderSubsystem" )
	bool GetUnitSubtypeAbilityTags(const FGameplayTag& UnitSubtypeTag, TArray<FGameplayTag>& OutAbilityTags);

	UFUNCTION(BlueprintCallable, Category = "UnitOrderSubsystem")
	void GetAllUnitDataCopy(TArray<FUnitOrderUnitType>& OutAllFullUnitTypeData);

	const TArray<FUnitOrderAbilityData>* GetAllUnitTypeAbilityData(const FGameplayTag& UnitSubtypeTag);

	const TArray<FUnitOrderUtilityAIAbilityData_Struct>* GetAvailableUtilityAIAbilities(const FGameplayTag& UnitSubtypeTag);

private:
	void LoadingUnitTypeData();
	void LoadingUnitTypeAbilityData();

	void LoadingUnitSubtypeData();
	void LoadingUnitSubtypeAbilityData();
	void LoadingUnitSubtypeUtilityAIAbilityData();  //UtilityAI Ability Data Loading

	void InitializationUnitDataCompllite();

	FUnitOrderUnitSubtype* GetUnitSubtypeDataInternal(const FGameplayTag& UnitSubtypeTag);


};
