﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"

#include "UnitOrderAbilityData.h"
#include "UnitOrderUnitTypeData.generated.h"

/** DA собирается для каждого типа юнита со своим тегом и набором способностей.
 *  Unit - это управляемая игроком единица игры, которая может быть чем угодно:
 *		- Строение.
 *		- Человек.
 *		- Техника.
 *		- Животное.
 *	Unit - это базовый тип единицы, который не может визуально существовать в игре. Конечным продуктом (управляемой единицей) является Unit Subtype.
 *	Unit хранит в себе всю основную информацию для всех своих подтипов, когда как подтипы хранят в себе только свои уникальные данные.
 *	Например, Unit DA хранит сами абилки, а Unit Subtype DA хранит только теги абилок, которые могут быть у конкретно этого подтипа.
 */

struct FUnitOrderObjectDescription;
struct FUnitOrderAbilityData;
struct FUnitOrderUnitSubtype;
class UUnitOrderGameplayAbility;

/** DA, которое хранит в себе общее описание типа юнита.
 *  В UnitOrderSystem используется структура юнит-подъюнит, в которой:
 *  - Unit Type - это базовый абстрактный тип, у которого есть описание, но нет игрового представления.
 *  - Unit Subtype - это подтип юнита, который используется в игре в качестве игрового объекта.
 *
 *  Например:
 *  В игре есть поселенцы - это общий тип юнита (Unit Type) - все люди в игре - это поселенцы.
 *  Рабочий, военный, торговец - это подтипы юнита (Unit Subtype) - это конкретные люди в игре, которые представлены в виде объектов (моделей).

 ABILITY:
 *  У каждого подтипа есть свои абилки, свои навыки и возможности.
 *  Хранение и подключение классов абилок происходит через DA UUnitOrderUnitTypeAbilityData. Через эти DA подключаются все абилки для этого типа.
 *  Когда Unit определяется с Subtype, то подгружаются не его абилки, а теги абилок через DA UUnitOrderUnitSubtypeAbilityData.
 *  Таким образом, все классы абилок всегда доступны в игре, но подключаются только те, которые нужны для конкретного подтипа юнита.

 TAGS vs DataAssets.
 *  При использовании DA в качестве определения классов и подклассов можно избегать совпадений. Это почти так же, как тегировать, только с помощью классов.
 *  Теги при этом можно использовать везде, не подключая DA ссылки в любой части игры.
 *
 *  Использовать DA - это значит, прокидывать ссылки на DA во все места, где они используются.
 *  С Тегами проще - класс доступен в любой части движка (Это основной аргумент, почему DA не так удобны, как Tags).
 */



/** DA, которое хранит в себе общее описание типа юнита. */
UCLASS()
class UNITORDERSYSTEM_API UUnitOrderUnitTypeData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	// For what Type of Unit this AbilityDataAsset is.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag UnitTypeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FObjectDescription UnitTypeDescription;

	bool ValidateDataAsset()
	{
		bool bDataAssetValid = true;
		if ( UnitTypeTag == FGameplayTag::EmptyTag )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). DataAsset (%s) has no UnitTypeTag"), *FString(__FUNCTION__), *GetName() );
			bDataAssetValid = false;
		}

		return bDataAssetValid;
	};

};


/** DA которое отвечает за описание типа юнита и хранит все абилки для него (для всех подтипов так же).
 *	Зачем:
 *		- Описание типа юнита.
 *		- Указывать какие абилки могут быть у юнита (совокупность всех подтипов).
 *			- Указываются SubclassOf абилок, полное их описание и теги.
 *			- Именно из этого списка берется список абилок для подтипа юнита.
 *		- Хранить настройки абилки отдельно от самой абилки:
 *			- Тег абилки.
 *			- Можно ли вызывать через UI.
 *		- Получать описание абилки по Тегу для:
 *			- UI.
 *			- Для AbilitySystemComponent, когда юнит меняется, чтобы понимать какие абилки нужно добавить, а какие удалить.


 *  Хранится 1 CDO в GAS на протяжении всей жизни юнита, так как это список всех возможных действий юнита вообще.
 *  Например. Юнит человек - может быть 3 подтипов:
 *		- Воин.
 *			- Стрелять.
 *			- Идти.
 *		- Строитель.
 *			- Строить.
 *			- Идти.
 *		- Торговец.
 *			- Торговать.
 *			- Идти.
 *
 *	Это DA будет хранить в себе все абилки - идти, атаковать, строить, торговать.
 *	Когда сменяется подтип юнита - то сменяются его !!активные абилки!!, но UUnitOrderAbilityDataAsset всегда остается один и не меняется.
 *	Удобно тем, что мы всегда можем получить из Юнита его список абилок и получить их и описания, и иконки и прочее.


 *	Для каждого типа может быть любое число DataAsset.
 *	Это используется для динамического добавления абилок при разработке игры, чтобы не пересобирать всю игру.
 */
UCLASS()
class UNITORDERSYSTEM_API UUnitOrderUnitTypeAbilityData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	// For what Type of Unit this AbilityDataAsset is.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag UnitTypeTag;

	// Full Ability Specs for this UnitType.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FUnitOrderAbilityData> AllUnitAbilities;

public:
	bool ValidateDataAsset()
	{
		bool bDataAssetValid = true;
		if ( UnitTypeTag == FGameplayTag::EmptyTag )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). DataAsset (%s) has no UnitTypeTag"), *FString(__FUNCTION__), *GetName() );
			bDataAssetValid = false;
		}

		if ( AllUnitAbilities.Num() == 0 )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). DataAsset (%s) has no abilities"), *FString(__FUNCTION__), *GetName() );
			bDataAssetValid = false;
		}

		return bDataAssetValid;
	};
};


/** DA, которое описывает подтип юнита и хранит теги абилок по умолчанию. НЕ все обилки. ТОЛЬКО по умолчанию..
 *	Зачем:
 *		- Хранит описание подтипа юнита.
 *		- Хранит абилки по умолчанию.
 *		- Определяет уникальность подтипа юнита. То есть, в проекте не может использоваться для одного и того же подтипа два разных DA.

 *  Юнит человек - может быть 3 подтипов:
 *		- Воин.
 *			- Стрелять.
 *			- Идти.
 *		- Строитель.
 *			- Строить.
 *			- Идти.

 *	Эта DA будет хранить только список параметров для конкретного подтипа юнита, например, Воина, или Строителя.
 *
 *  Для каждого типа юнита свой тег, свои абилки по умолчанию.
 *	Так же хранит прямое описание типа юнита FUnitOrderObjectDescription.
 *	Все остальные абилки, которые доступны для юнита, указываются в другом DA - UUnitOrderUnitSubtypeAbilityData */
UCLASS()
class UNITORDERSYSTEM_API UUnitOrderUnitSubtypeData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag UnitSubtypeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FObjectDescription UnitSubtypeDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag DefaultAbilityTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AutoAbilityTag;

	bool ValidateDataAsset(bool bFullChecking = false)
	{
		bool bDataAssetValid = true;
		if ( UnitSubtypeTag == FGameplayTag::EmptyTag )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). DataAsset (%s) has no UnitSubtypeTag"), *FString(__FUNCTION__), *GetName() );
			bDataAssetValid = false;
		}

		if(bFullChecking)
		{
			if ( DefaultAbilityTag == FGameplayTag::EmptyTag )
			{
				ensureAlwaysMsgf( false, TEXT("%s(). DataAsset (%s) has no DefaultAbilityTag"), *FString(__FUNCTION__), *GetName() );
				bDataAssetValid = false;
			}
			if ( AutoAbilityTag == FGameplayTag::EmptyTag )
			{
				ensureAlwaysMsgf( false, TEXT("%s(). DataAsset (%s) has no AutoAbilityTag"), *FString(__FUNCTION__), *GetName() );
				bDataAssetValid = false;
			}
		}

		return bDataAssetValid;
	};
};

/** DA, которое хранит список тегов абилок для подтипа.
 *  Для одного подтипа может быть любое число DA. Они используются для динамического добавления абилок при разработке игры, чтобы не пересобирать всю игру.
 *  Если разработчик добавит случайно один и тот же тег в DA два раза, то ничего не произойдет,
 *  так как в AbilitySystemComponent будет происходить проверка и отсечение дубликатов.
 */
UCLASS()
class UNITORDERSYSTEM_API UUnitOrderUnitSubtypeAbilityData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	// For what Type of Unit this SubUnitData is.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag UnitSubtypeTag;

	// List of Ability Tags for this SubUnit.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGameplayTag> AvailableAbilityTags;

	bool ValidateDataAsset()
	{
		bool bDataAssetValid = true;

		if ( UnitSubtypeTag == FGameplayTag::EmptyTag )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). DataAsset (%s) has no UnitSubtypeTag"), *FString(__FUNCTION__), *GetName() );
			bDataAssetValid = false;
		}
		if ( AvailableAbilityTags.Num() == 0 )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). DataAsset (%s) has no AvailableAbilityTags"), *FString(__FUNCTION__), *GetName() );
			bDataAssetValid = false;
		}

		return bDataAssetValid;
	};

};


/* This DA reflects data from DA UUnitOrderUnitSubtypeData and UUnitOrderUnitSubtypeAbilityData. It is used for validating and storing data in Runtime. */
USTRUCT(BlueprintType, Blueprintable)
struct UNITORDERSYSTEM_API FUnitOrderUnitSubtype
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly)
	TObjectPtr<UUnitOrderUnitSubtypeData> UnitSubtypeData = nullptr;

	// List of Ability Tags for this SubUnit.
	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly)
	TArray<FGameplayTag> AvailableAbilityTags;

	FUnitOrderUnitSubtype(UUnitOrderUnitSubtypeData* NewUnitSubtypeData): UnitSubtypeData(NewUnitSubtypeData)
	{
		if(!IsValid(UnitSubtypeData))
		{
			ensureAlwaysMsgf( false, TEXT("%s(). New UnitSubtypeData invalid"), *FString(__FUNCTION__) );
		}
	}

	/** DO NOT USE IT. STRUCT CANNOT BE COMPILED WITHOUT DEFAULT CONSTRUCTOR FOR BLUEPRINTS */
	FUnitOrderUnitSubtype()
	{
		UE_LOG(LogTemp, Warning, TEXT("Don't use Default Constructor FUnitOrderUnitSubtype()"));
	};
};


/* This DA reflects data from DA UUnitOrderUnitTypeData and UUnitOrderUnitTypeAbilityData. It is used for validating and storing data in Runtime. */
USTRUCT(BlueprintType, Blueprintable)
struct UNITORDERSYSTEM_API FUnitOrderUnitType
{
	GENERATED_BODY()
	// For what Type of Unit this AbilityDataAsset is.
	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly)
	TObjectPtr<UUnitOrderUnitTypeData> UnitTypeData = nullptr;

	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly)
	TArray<FUnitOrderAbilityData> AllUnitAbilities;

	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly)
	TArray<FUnitOrderUnitSubtype> UnitSubtypes;

	FUnitOrderUnitType(UUnitOrderUnitTypeData* NewUnitTypeData): UnitTypeData(NewUnitTypeData)
	{
		if(!IsValid(UnitTypeData))
		{
			ensureAlwaysMsgf( false, TEXT("%s(). New UnitTypeData invalid"), *FString(__FUNCTION__) );
		}
	}

	/** DO NOT USE IT. STRUCT CANNOT BE COMPILED WITHOUT DEFAULT CONSTRUCTOR FOR BLUEPRINTS */
	FUnitOrderUnitType()
	{
		UE_LOG(LogTemp, Warning, TEXT("Don't use Default Constructor FUnitOrderUnitType()"));
	};
};
