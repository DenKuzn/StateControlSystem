// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BoxStrategy/Framework/GameData/Structs/BoxStrategyObjectDescription.h"
#include "BoxStrategyAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FAbilitySystemDelegateWithTag, FGameplayTag, AbilityTag );


class UBoxStrategyAbilitySettings;
class UBoxStrategyUnitAbilityList;
class UBoxStrategyAbilityDataAsset;
struct FBoxStrategyAbilityData;

/** Управление происходит только через Стейты и Абилки.
 *  Компонент не может вызывать сам новые стейты игры, так как это юнит, и он не должен иметь такого доступа - это не игрок.
 *  Юнит может только получать приказы и выполнять их, но не может сам запускать стейты игрока.
 *
 *  Default Order. Стейт запрашивает тег дефолтной абилки, создает дефолтный сеттинг класс, передает его и запускает абилку.
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOXSTRATEGY_API UBoxStrategyAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBoxStrategyAbilitySystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled) override;

private:
	UPROPERTY( EditDefaultsOnly, Category = "BoxStrategy" )
	TObjectPtr<UBoxStrategyAbilityDataAsset> AllUnitAbilitiesDataAsset = nullptr;

	UPROPERTY( EditDefaultsOnly, Category = "BoxStrategy" )
	TArray<UBoxStrategyUnitAbilityList*> UnitTypeList;

	// On start just get first UnitType from list
	UPROPERTY()
	TObjectPtr<UBoxStrategyUnitAbilityList> CurrentUnitType = nullptr;

	UPROPERTY()
	TArray<UBoxStrategyAbilitySettings*> OrderSettingsList;

public:
	FAbilitySystemDelegateWithTag OnUnitTypeChanged;

public:
	// Returns the health component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "BoxStrategy|UnitAbilitySystemComponent")
	static UBoxStrategyAbilitySystemComponent* FindBoxStrategyAbilitySystemComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UBoxStrategyAbilitySystemComponent>() : nullptr); }

	UFUNCTION( BlueprintCallable, Category = "BoxStrategy|UnitInformaton" )
	void SetNewUnitType(FGameplayTag NewUnitType, bool bStartAutoAbility);

	UFUNCTION( BlueprintPure, Category = "BoxStrategy|UnitInformaton" )
	FGameplayTag GetCurrentUnitType();

	UFUNCTION( BlueprintCallable, Category = "BoxStrategy|UnitInformaton" )
	const TArray<UBoxStrategyUnitAbilityList*>& GetAllUnitTypeList();

	UFUNCTION( BlueprintCallable, Category = "BoxStrategy|UnitInformaton" )
	bool GetUnitTypeDescription(FBoxStrategyObjectDescription& OutUnitTypeDescription, FGameplayTag SearchedUnitTypeTag);

	UFUNCTION( BlueprintPure, Category = "BoxStrategy|UnitInformaton" )
	const UBoxStrategyUnitAbilityList* GetCurrentUnitTypeData();


	UFUNCTION( BlueprintCallable, Category = "BoxStrategy|Abilities" )
	bool GetUnitAbilityData(FBoxStrategyAbilityData& OutAbilityData, FGameplayTag SearchedAbilityTag);

	/** Useful for resources that add new ability. Granades, Rocket Launchers etc. */
	UFUNCTION( BlueprintCallable, Category = "BoxStrategy|Abilities" )
	void AddNewUnitAbility(FGameplayTag AbilityTag);

	UFUNCTION( BlueprintCallable, Category = "BoxStrategy|Abilities" )
	void RemoveUnitAbility(FGameplayTag AbilityTag);

	/** Add new order with Settings. For new orders that must be exlusive use bClearOrderList.
	 * Can be called from States and Abilities. */
	UFUNCTION( BlueprintCallable, Category = "BoxStrategy|AbilityOrderControl" )
	void AddNextUnitAbilityOrderSettings(UBoxStrategyAbilitySettings* NewAbilitySettings, bool bClearOrderList = false);

	/** There is no situation when Ability will not started from last Settings. Exept AutoAbilities. */
	UFUNCTION( BlueprintCallable, Category = "BoxStrategy|AbilityOrderControl" )
	const UBoxStrategyAbilitySettings* GetLastUnitAbilitySettings();

	/** If Order List is empty then start an automatical ability */
	UFUNCTION( BlueprintCallable, Category = "BoxStrategy|AbilityOrderControl" )
	void ExecuteNextUnitOrder();

	UFUNCTION( BlueprintCallable, Category = "BoxStrategy|AbilityOrderControl" )
	void ExecuteDefaultUnitOrder(UBoxStrategyAbilitySettings* AbilitySettings);

	/** For more faster call New Order. Dont need to set AbilityTag to Settings. It will be implemented in this function:
	*	AbilitySettings->SetAbilityTag( UnitAbilityTag );
	*	SetNextUnitAbilityOrderSettings( AbilitySettings, bClearOrderList );
	*	ExecuteNextUnitOrder();	*/
	UFUNCTION( BlueprintCallable, Category = "BoxStrategy|AbilityOrderControl" )
	void ExecuteNewUnitOrder(UBoxStrategyAbilitySettings* AbilitySettings, FGameplayTag UnitAbilityTag, bool bClearOrderList = true);


	UFUNCTION( BlueprintPure, Category = "BoxStrategy|UnitInformaton" )
	void GetAllAvailableAbilitiesTags(TArray<FGameplayTag>& OutAbilityTags);

	UFUNCTION( BlueprintPure, Category = "BoxStrategy|UnitInformaton" )
	UBoxStrategyAbilityDataAsset* GetUnitAbilitiesDataAsset();

private:
	// USE ONLY IN DEBUG BUILD!!!
	bool IsAbilityAlreadyLoaded(TSubclassOf<UGameplayAbility> AbilityClass);
};


/** When it starts all abilities are closed.
 * Default Abilities dont use State Control And Ability Settings. It must be start in same frame, when player activate it */
//UFUNCTION( BlueprintCallable, Category = "BoxStrategy|AbilityOrderControl" )
//void ExecuteDefaultUnitOrder(FHitResult& HitResult);