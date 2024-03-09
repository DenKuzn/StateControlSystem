// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
#include "AbilitySystemComponent.h"

#include "ObjectDescriptionPlugin/Framework/ObjectDescriptionStruct.h"
#include "UnitOrderSystem/Framework/UnitOrderSubsystem.h"
#include "UnitOrderAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FAbilitySystemDelegateWithTag, FGameplayTag, AbilityTag );


class UUnitOrderAbilitySettings;
class UUnitOrderUnitSubtypeData;
class UUnitOrderUnitTypeData;
struct FUnitOrderAbilityData;


/** Управление происходит через Стейты (Player actions), Абилки (current ability flow), and Unit Order Controllers (AI Controllers that can be added to Unit dynamically).

 *  Компонент не может вызывать сам новые стейты игры, так как это юнит, и он не должен иметь такого доступа - это не игрок.
 *  Юнит может только получать приказы и выполнять их, но не может сам запускать стейты игрока.

 *  Default Order. Стейт запрашивает тег дефолтной абилки, создает дефолтный сеттинг класс, передает его и запускает абилку. */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNITORDERSYSTEM_API UUnitOrderAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UUnitOrderAbilitySystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled) override;

private:
	//TODO Переписать весь код таким образом, чтобы он теперь работал с UnitOrderSubsystem.

	// Current Unit Type. Cannot be modified in runtime.
	UPROPERTY( EditDefaultsOnly, Category = "UnitOrder" )
	TObjectPtr<UUnitOrderUnitTypeData> BaseUnitType = nullptr;

	// Current subtype. Must be selected like default type. Can be modified in runtime.
	UPROPERTY( EditAnywhere, Category = "UnitOrder" )
	TObjectPtr<UUnitOrderUnitSubtypeData> CurrentUnitSubtype = nullptr;

	// Default Unit Order Type:
	UPROPERTY( EditDefaultsOnly, Category = "UnitOrder" )
	FGameplayTag DefaultOrderControllerTypeTag;

	// Current Unit Order Type:
	FGameplayTag CurrentOrderControllerTypeTag;

	// Current Unit Orders.
	UPROPERTY()
	TArray<UUnitOrderAbilitySettings*> OrderSettingsList;

public:
	UPROPERTY( BlueprintAssignable, Category = "UnitOrder" )
	FAbilitySystemDelegateWithTag OnUnitTypeChanged;

	// Used by other controllers to know when Ability is ended.
	UPROPERTY( BlueprintAssignable, Category = "UnitOrder" )
	FAbilitySystemDelegateWithTag OnAbilityEndedWithComponentNonDefaultState;


// GENERAL SECTION.
public:
	// Returns the health component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "UnitOrder|UnitAbilitySystemComponent")
	static UUnitOrderAbilitySystemComponent* FindUnitOrderAbilitySystemComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UUnitOrderAbilitySystemComponent>() : nullptr); }

	UFUNCTION( BlueprintPure, Category = "UnitOrder|UnitTypeInformaton" )
	UUnitOrderUnitTypeData* GetUnitTypeData();

	UFUNCTION( BlueprintPure, Category = "UnitOrder|UnitTypeInformaton" )
	FGameplayTag GetUnitTypeTag();

	UFUNCTION( BlueprintCallable, Category = "UnitOrder|UnitTypeInformaton" )
	bool GetUnitTypeDescription(FObjectDescription& OutUnitTypeDescription);

	UFUNCTION( BlueprintCallable, Category = "UnitOrder|UnitTypeInformaton" )
	bool GetUnitAbilityData(FUnitOrderAbilityData& OutAbilityData, const FGameplayTag& SearchedAbilityTag);


// SUBTYPE SETTINGS SECTION.
public:
	UFUNCTION( BlueprintCallable, Category = "UnitOrder|UnitInformaton" )
	void SetNewUnitSubtype(const FGameplayTag& NewUnitSubtype, bool bStartAutoAbility);

	UFUNCTION( BlueprintPure, Category = "UnitOrder|UnitInformaton" )
	const FGameplayTag& GetCurrentUnitSubtypeTag() const;

	UFUNCTION( BlueprintPure, Category = "UnitOrder|UnitInformaton" )
	const UUnitOrderUnitSubtypeData* GetCurrentUnitSubtypeData();

	UFUNCTION( BlueprintCallable, Category = "UnitOrder|UnitTypeInformaton" )
	bool GetCurrentUnitSubtypeDescription(FObjectDescription& OutUnitSubtypeDescription);

	// Return only Subtype Abilities (not including Object Abilities, like granades etc).
	UFUNCTION( BlueprintPure, Category = "UnitOrder|UnitInformaton" )
	void GetCurrentUnitSubtypeAbilityTags(TArray<FGameplayTag>& OutAbilityTags);


// ORDER SECTION.
public:

	UFUNCTION( BlueprintCallable, Category = "UnitOrder|AbilityOrderControl" )
	void SetNewOrderType(const FGameplayTag& NewOrderType);

	UFUNCTION( BlueprintCallable, Category = "UnitOrder|AbilityOrderControl" )
	const FGameplayTag& GetCurrentOrderTypeTag() const;

	/** Add new order with Settings. For new orders that must be exlusive use bClearOrderList.
	 * Can be called from States and Abilities. */
	UFUNCTION( BlueprintCallable, Category = "UnitOrder|AbilityOrderControl" )
	void AddNextUnitAbilityOrderSettings(UUnitOrderAbilitySettings* NewAbilitySettings, bool bClearOrderList = false);

	/** There is no situation when Ability will not started from last Settings. Exept AutoAbilities. */
	UFUNCTION( BlueprintCallable, Category = "UnitOrder|AbilityOrderControl" )
	const UUnitOrderAbilitySettings* GetLastUnitAbilitySettings() const;

	/** If Order List is empty then start an automatical ability */
	UFUNCTION( BlueprintCallable, Category = "UnitOrder|AbilityOrderControl" )
	void ExecuteNextUnitOrder();

	UFUNCTION( BlueprintCallable, Category = "UnitOrder|AbilityOrderControl" )
	void ExecuteDefaultUnitOrder(UUnitOrderAbilitySettings* AbilitySettings);

	/** For more faster call New Order. Dont need to set AbilityTag to Settings. It will be implemented in this function:
	*	AbilitySettings->SetAbilityTag( UnitAbilityTag );
	*	SetNextUnitAbilityOrderSettings( AbilitySettings, bClearOrderList );
	*	ExecuteNextUnitOrder();	*/
	UFUNCTION( BlueprintCallable, Category = "UnitOrder|AbilityOrderControl" )
	void ExecuteNewUnitOrder(UUnitOrderAbilitySettings* AbilitySettings, const FGameplayTag& UnitAbilityTag, bool bClearOrderList = true);


private:

	UUnitOrderGameplayAbility* FindAbilityInstanceByTag(const FGameplayTag& AbilityTag);

	bool IsAbilityAlreadyLoaded(TSubclassOf<UGameplayAbility> AbilityClass);

	void UnitSubtypeDataAssetesLoaded();

	bool ValidateSettings();

	UUnitOrderSubsystem* GetUnitOrderSubsystem();

	void AddNewUnitAbility(const FUnitOrderAbilityData& AbilityData);

	UFUNCTION()
	void InitializeUnitAbilityComponent();

	/*
	// Useful for resources that add new ability. Granades, Rocket Launchers etc.
	UFUNCTION( BlueprintCallable, Category = "UnitOrder|Abilities" )
	void AddNewUnitAbility(FGameplayTag AbilityTag);

	UFUNCTION( BlueprintCallable, Category = "UnitOrder|Abilities" )
	void RemoveUnitAbility(FGameplayTag AbilityTag);
	*/
};