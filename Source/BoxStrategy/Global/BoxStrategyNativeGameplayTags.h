// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;


/// В будущем, это будет хранилищем всех тегов, которые используются в игре.


/** Нужно переписать структуру следующим образом:
 * - Оставить основную структуру GameplayTags. Она будет отвечать за подгрузку тегов из других структур.
 * - Собрать другие структуры, в которые распихать теги, чтобы все теги были сконцентрированы в одной структуре по типу тега (абилки к абилкам, стейты к стейтам, и так далее). */

struct BOXSTRATEGY_API FBoxStrategyNativeGameplayTags
{

private:
    static FBoxStrategyNativeGameplayTags GameplayTags;

public:
	static const FBoxStrategyNativeGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeTags();

	//static FGameplayTag FindTagByString(FString TagString, bool bMatchPartialString = false);

protected:

	void AddAllTags(UGameplayTagsManager& Manager);

	void AddTag(FGameplayTag& OutTag, FString StringTagName, const FString& TagComment);

public:

// UNITS
	FGameplayTag BoxStrategy_Unit_UnitType_Warrior;

// ABILITIES
	FGameplayTag BoxStrategy_Ability_Unit_Misc_MoveTo;
	FGameplayTag BoxStrategy_Ability_Unit_Misc_MoveToSight;

	FGameplayTag BoxStrategy_Ability_Unit_Warrior_Auto_SearchEnemies;
	FGameplayTag BoxStrategy_Ability_Unit_Warrior_Default_MoveOrAttack;
	FGameplayTag BoxStrategy_Ability_Unit_Warrior_Other_StandardAttack;

// ITEMS
	FGameplayTag BoxStrategy_Items_Weapons_Hand_Rifle;

// DAMAGE TYPES
	FGameplayTag BoxStrategy_DamageType_Kinetic;

// STATE CONTROLS BASE INPUTS
	FGameplayTag StateControl_BaseInputs_MainAction;
	FGameplayTag StateControl_BaseInputs_SecondAction;
	FGameplayTag StateControl_BaseInputs_BackAction;

// STATE CONTROLS
	FGameplayTag StateControl_Orders_MoveTo;
	FGameplayTag StateControl_Orders_Attack;

	FGameplayTag StateControl_States_DefaultState;

	FGameplayTag StateControl_States_SelectionState_Building;
	FGameplayTag StateControl_States_SelectionState_Unit_ControlledUnit_MultiSelection;
	FGameplayTag StateControl_States_SelectionState_Unit_Worker;
};


