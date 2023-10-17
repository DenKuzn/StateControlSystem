// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

//#define GAME_DEBUG_BUILDS WITH_EDITOR || UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#define GAME_DEBUG_BUILDS !UE_BUILD_SHIPPING

//#define GAME_DEBUG_POINTER_NOT_VALID(PointerName) UE_LOG(LogTemp, Error, TEXT("%s(). %s invalid"), *FString(__FUNCTION__), *FString(PointerName))
#define GAME_DEBUG_POINTER_NOT_VALID(PointerName) ensureAlwaysMsgf( false, TEXT("%s(). %s invalid"), *FString(__FUNCTION__), *FString(PointerName))

// Если указатель не валиден, то функция закрывается с возвращаемым типом (пустой тип - void);
#define GAME_DEBUG_CHECK_RAWPOINTER_WITH_RETURN(Pointer, ReturnData) if (!Pointer) { GAME_DEBUG_POINTER_NOT_VALID(#Pointer); return ReturnData; }
#define GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN(Pointer, ReturnData) if (!IsValid(Pointer)) { GAME_DEBUG_POINTER_NOT_VALID(#Pointer); return ReturnData; }

#define FUNCTION_START_LOG()	UE_LOG(LogTemp, Warning, TEXT("%s() Started"), *FString(__FUNCTION__))
#define FUNCTION_END_LOG()		UE_LOG(LogTemp, Warning, TEXT("%s() Ended"), *FString(__FUNCTION__))

#define TEXT_TRUE_FALSE(bValue) bValue ? TEXT("true") : TEXT("false")