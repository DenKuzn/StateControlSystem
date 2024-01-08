// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ObjectDescriptionStruct.generated.h"


/** Base Struct for Objects Description like Items, Weapons, Abilities, etc */
USTRUCT( BlueprintType )
struct OBJECTDESCRIPTIONPLUGIN_API FObjectDescription
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FText MainName = FText::FromString( "Default Name" );

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FText ShortDescription = FText::FromString( "Default Short Description" );

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FText FullDescription = FText::FromString( "Default Full Description" );

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> ItemImage = nullptr;
};