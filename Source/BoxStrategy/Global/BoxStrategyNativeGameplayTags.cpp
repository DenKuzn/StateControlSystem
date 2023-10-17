// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#include "BoxStrategyNativeGameplayTags.h"

#include "GameplayTagsManager.h"

DEFINE_LOG_CATEGORY_STATIC( BoxStrategyGameplayTags, Log, All );

#define ADD_NEW_TAG_TO_GAME(TagVariableName, Description) AddTag(TagVariableName, #TagVariableName, Description)

FBoxStrategyNativeGameplayTags FBoxStrategyNativeGameplayTags::GameplayTags;


void FBoxStrategyNativeGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();
	GameplayTags.AddAllTags(TagsManager);
	TagsManager.DoneAddingNativeTags();
}

void FBoxStrategyNativeGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	ADD_NEW_TAG_TO_GAME(BoxStrategy_Unit_UnitType_Warrior, "Warrior Type.");

	ADD_NEW_TAG_TO_GAME(BoxStrategy_Ability_Unit_Misc_MoveTo, "Ability to move to point or object.");
	ADD_NEW_TAG_TO_GAME(BoxStrategy_Ability_Unit_Misc_MoveToSight, "Ability to move to object while unit dont see it.");

	ADD_NEW_TAG_TO_GAME(BoxStrategy_Ability_Unit_Warrior_Auto_SearchEnemies, "Search Enemies when nothing else to do.");
	ADD_NEW_TAG_TO_GAME(BoxStrategy_Ability_Unit_Warrior_Default_MoveOrAttack, "Default Behavior.");
	ADD_NEW_TAG_TO_GAME(BoxStrategy_Ability_Unit_Warrior_Other_StandardAttack, "Unit Standard Attack.");

	ADD_NEW_TAG_TO_GAME(BoxStrategy_Items_Weapons_Hand_Rifle, "Rifle.");

	ADD_NEW_TAG_TO_GAME(BoxStrategy_DamageType_Kinetic, "Standard Bullets.");

	ADD_NEW_TAG_TO_GAME(StateControl_BaseInputs_MainAction, "Main Input");
	ADD_NEW_TAG_TO_GAME(StateControl_BaseInputs_SecondAction, "Main Input");
	ADD_NEW_TAG_TO_GAME(StateControl_BaseInputs_BackAction, "Main Input");


	ADD_NEW_TAG_TO_GAME(StateControl_Orders_MoveTo, "Order to Move to point or object.");
	ADD_NEW_TAG_TO_GAME(StateControl_Orders_Attack, "Order to Attack object.");

	ADD_NEW_TAG_TO_GAME(StateControl_States_DefaultState, "Defaulat State of the game");

	ADD_NEW_TAG_TO_GAME(StateControl_States_SelectionState_Building, "State for Building Selection.");
	ADD_NEW_TAG_TO_GAME(StateControl_States_SelectionState_Unit_ControlledUnit_MultiSelection, "State for Unit Selection.");
	ADD_NEW_TAG_TO_GAME(StateControl_States_SelectionState_Unit_Worker, "State for Wroker Unit Selection.");

}

void FBoxStrategyNativeGameplayTags::AddTag(FGameplayTag& OutTag, FString StringTagName, const FString& TagComment)
{

	for(wchar_t* Char = StringTagName.GetCharArray().GetData(); *Char; ++Char)
	{
		if(*Char == '_')
		{
			*Char = '.';
		}
	}

	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag( FName(StringTagName), FString( TEXT( "(Native) " ) ) + TagComment );
}
