// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;

/** Main Goal - add native tags, that can be used in C++.
 *	Problem:
 *		- Tags, that are added in C++ can't be used in Pack files. You will need to recompile entire project and replace exe-file to use new tags.

 *	Decision:
		- NativeTags can be used only for core systems that will be updated very rarely. For example State Controls. Or when using GameplayMessageRouter in C++.
		- Use only BP Tags that are stored in *.ini files and can be easily changed or added in project in runtime without recompile project.
 * */

struct BOXSTRATEGY_API FBoxStrategyNativeGameplayTags
{

private:
    static FBoxStrategyNativeGameplayTags GameplayTags;

public:
	static const FBoxStrategyNativeGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeTags();

protected:

	void AddAllTags(UGameplayTagsManager& Manager);

	void AddTag(FGameplayTag& OutTag, FString StringTagName, const FString& TagComment);

public:

	// Example how to use native tags in C++.
	FGameplayTag NativeGameplayTag_Example_DeepestTag;

};


