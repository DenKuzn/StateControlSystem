// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#include "BoxStrategyNativeGameplayTags.h"

#include "GameplayTagsManager.h"


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
	ADD_NEW_TAG_TO_GAME(NativeGameplayTag_Example_DeepestTag, "Example.");
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
