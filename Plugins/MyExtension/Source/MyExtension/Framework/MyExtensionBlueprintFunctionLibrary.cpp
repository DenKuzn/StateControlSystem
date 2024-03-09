// Fill out your copyright notice in the Description page of Project Settings.


#include "MyExtensionBlueprintFunctionLibrary.h"

#include "Components/BoxComponent.h"

void UMyExtensionBlueprintFunctionLibrary::UpdateComponentOverlaps(UPrimitiveComponent* PrimitiveComponent)
{
	if(IsValid( PrimitiveComponent ))
	{
		PrimitiveComponent->UpdateOverlaps();
		UBoxComponent* BoxComponent = Cast<UBoxComponent>(PrimitiveComponent);
	}
	else
	{
		ensureAlwaysMsgf( false, TEXT("%s(). !IsValid( PrimitiveComponent ) "), *FString(__FUNCTION__) );
	}
}

void UMyExtensionBlueprintFunctionLibrary::SetDynamicObstacle(UShapeComponent* PrimitiveComponent, bool bNewDynamicObstacle)
{
	if(IsValid( PrimitiveComponent ))
	{
		PrimitiveComponent->bDynamicObstacle = bNewDynamicObstacle;
	}
	else
	{
		ensureAlwaysMsgf( false, TEXT("%s(). !IsValid( PrimitiveComponent ) "), *FString(__FUNCTION__) );
	}
}
