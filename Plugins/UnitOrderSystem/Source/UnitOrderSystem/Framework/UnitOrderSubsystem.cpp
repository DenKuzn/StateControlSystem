// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitOrderSubsystem.h"
#include "Engine/AssetManager.h"

#include "UnitData/UnitOrderUnitTypeData.h"

#include "GeneralDebugMacroses/Framework/DebugMacroses.h"

DEFINE_LOG_CATEGORY_STATIC( LogUnitOrderSystem, Log, All );

void UUnitOrderSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize( Collection );


	// Нам нужна последовательность загрузки:

	// 1. Загрузка всех UnitTypeData.
	// 2. Загрузка всех UnitTypeAbilityData.

	// 3. Загрузка всех UnitSubtypeData.
	// 4. Загрузка всех UnitSubtypeAbilityTagsData.
	// Так как загрузка происходит асинхронно, то все ассеты будут подгружаться цепочкой по списку (выше).


	LoadingUnitTypeData();
}

bool UUnitOrderSubsystem::GetUnitAbilityData(const FGameplayTag& UnitTypeTag,
                                             const FGameplayTag& AbilityTag,
                                             FUnitOrderAbilityData& OutAbilityData)
{
	for ( auto& UnitTypeAbilityData : *GetAllUnitTypeAbilityData( UnitTypeTag ) )
	{
		if ( UnitTypeAbilityData.AbilityTag == AbilityTag )
		{
			OutAbilityData = UnitTypeAbilityData;
			return true;
		}
	}

	return false;
}

UUnitOrderUnitSubtypeData* UUnitOrderSubsystem::GetUnitSubtypeData(const FGameplayTag& UnitSubtypeTag)
{
	{
#if GAME_DEBUG_BUILDS
		if ( UnitSubtypeTag == FGameplayTag::EmptyTag )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). UnitSubtypeTag is EmptyTag."), *FString(__FUNCTION__) );
			return nullptr;
		}
#endif
	} // Check for empty tag.

	if ( !bAllUnitDataInitialized )
	{
		ensureAlwaysMsgf( false, TEXT("%s(). !bAllUnitDataInitialized"), *FString(__FUNCTION__) );
		return nullptr;
	}

	for ( auto& UnittypeData : AllFullUnitTypeData )
	{
		// Find unit tyoe.
		if ( UnitSubtypeTag.MatchesTag( UnittypeData.UnitTypeData->UnitTypeTag ) )
		{
			for ( auto& UnitSubtypeData : UnittypeData.UnitSubtypes )
			{
				// Find unit subtype.
				if ( UnitSubtypeData.UnitSubtypeData->UnitSubtypeTag == UnitSubtypeTag )
				{
					return UnitSubtypeData.UnitSubtypeData;
				}
			}
		}
	}

	ensureAlwaysMsgf( false, TEXT("%s(). Unit SUBTYPE not found. Searched tag: %s"), *FString(__FUNCTION__), *UnitSubtypeTag.ToString() );
	return nullptr;
}

bool UUnitOrderSubsystem::GetUnitSubtypeAbilityTags(const FGameplayTag& UnitSubtypeTag,
                                                    TArray<FGameplayTag>& OutAbilityTags)
{
	OutAbilityTags.Reset( 20 );

	{
#if GAME_DEBUG_BUILDS
		if ( UnitSubtypeTag == FGameplayTag::EmptyTag )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). UnitSubtypeTag is EmptyTag."), *FString(__FUNCTION__) );
			return false;
		}
#endif
	} // Check for empty tag.


	if ( !bAllUnitDataInitialized )
	{
		ensureAlwaysMsgf( false, TEXT("%s(). !bAllUnitDataInitialized"), *FString(__FUNCTION__) );
		return false;
	}

	for ( auto& UnittypeData : AllFullUnitTypeData )
	{
		// Find unit tyoe.
		if ( UnitSubtypeTag.MatchesTag( UnittypeData.UnitTypeData->UnitTypeTag ) )
		{
			for ( auto& UnitSubtypeData : UnittypeData.UnitSubtypes )
			{
				// Find unit subtype.
				if ( UnitSubtypeData.UnitSubtypeData->UnitSubtypeTag == UnitSubtypeTag )
				{
					OutAbilityTags = UnitSubtypeData.AvailableAbilityTags;
				}
			}
		}
	}

	ensureAlwaysMsgf( false, TEXT("%s(). Unit SUBTYPE not found. Searched tag: %s"), *FString(__FUNCTION__), *UnitSubtypeTag.ToString() );
	return false;
}

const TArray<FUnitOrderAbilityData>* UUnitOrderSubsystem::GetAllUnitTypeAbilityData(const FGameplayTag& UnitTypeTag)
{
	{
#if GAME_DEBUG_BUILDS
		if ( UnitTypeTag == FGameplayTag::EmptyTag )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). UnitTypeTag is EmptyTag."), *FString(__FUNCTION__) );
			return nullptr;
		}
#endif
	}

	for ( auto& UnittypeData : AllFullUnitTypeData )
	{
		// Find unit tyoe.
		if ( UnitTypeTag == UnittypeData.UnitTypeData->UnitTypeTag )
		{
			return &UnittypeData.AllUnitAbilities;
		}
	}

	ensureAlwaysMsgf( false, TEXT("%s(). Unit TYPE not found. Searched tag: %s"), *FString(__FUNCTION__), *UnitTypeTag.ToString() );
	return nullptr;
}

void UUnitOrderSubsystem::GetAllUnitDataCopy(TArray<FUnitOrderUnitType>& OutAllFullUnitTypeData)
{
	OutAllFullUnitTypeData = AllFullUnitTypeData;
}

void UUnitOrderSubsystem::LoadingUnitTypeData()
{
	const FPrimaryAssetType UnitTypeDataAssetType = FPrimaryAssetType{UUnitOrderUnitTypeData::StaticClass()->GetFName()};
	UAssetManager::Get().LoadPrimaryAssetsWithType( UnitTypeDataAssetType, TArray<FName>(), FStreamableDelegate::CreateLambda( [this]()
	{
		UAssetManager& AssetManager = UAssetManager::Get();
		TArray<FAssetData> UnitTypeDataAssetNames;
		AssetManager.GetPrimaryAssetDataList( FPrimaryAssetType{UUnitOrderUnitTypeData::StaticClass()->GetFName()}, UnitTypeDataAssetNames );
		{
			if ( UnitTypeDataAssetNames.Num() == 0 )
			{
				UE_LOG( LogUnitOrderSystem, Warning, TEXT("%s(). No UUnitOrderUnitTypeData found"), *FString( __FUNCTION__ ) );

				InitializationUnitDataCompllite();
				return;
			}
		}

		AllFullUnitTypeData.Reset( UnitTypeDataAssetNames.Num() );
		for ( auto& LoadedAssetDataName : UnitTypeDataAssetNames )
		{
			UUnitOrderUnitTypeData* DataAsset = Cast<UUnitOrderUnitTypeData>( AssetManager.GetPrimaryAssetObject( LoadedAssetDataName.GetPrimaryAssetId() ) );
			{
				{
					if ( !IsValid( DataAsset ) )
					{
						UE_LOG( LogUnitOrderSystem, Warning, TEXT("DataAssetName (%s) invalid"), *LoadedAssetDataName.AssetName.ToString() );
						continue;
					}
					if ( !DataAsset->ValidateDataAsset() )
					{
						UE_LOG( LogUnitOrderSystem, Warning, TEXT("DataAsset (%s) has invaid Data"), *LoadedAssetDataName.AssetName.ToString() );
						continue;
					}
				}

				bool bExists = false;
				for ( auto& ExistingUnitType : AllFullUnitTypeData )
				{
					if ( IsValid( ExistingUnitType.UnitTypeData ) && ( ExistingUnitType.UnitTypeData->UnitTypeTag == DataAsset->UnitTypeTag ) )
					{
						UE_LOG( LogUnitOrderSystem, Warning,
						        TEXT("%s(). UnitType with tag (%s) already exists. DataAsset Name: %s."),
						        *FString(__FUNCTION__),
						        *DataAsset->UnitTypeTag.ToString(),
						        *DataAsset->GetName() );
						bExists = true;
						break;
					}
				}
				if ( bExists )
				{
					continue;
				}
			}

			AllFullUnitTypeData.Add( FUnitOrderUnitType{DataAsset} );
		}

		// 2. Загрузка всех UnitTypeAbilityData.
		//UE_LOG( LogUnitOrderSystem, Warning, TEXT("Async LoadingUnitTypeData(). Ended") );
		LoadingUnitTypeAbilityData();
	} ) );
}

void UUnitOrderSubsystem::LoadingUnitTypeAbilityData()
{
	const FPrimaryAssetType UnitTypeDataAssetType = FPrimaryAssetType{UUnitOrderUnitTypeAbilityData::StaticClass()->GetFName()};
	UAssetManager::Get().LoadPrimaryAssetsWithType( UnitTypeDataAssetType, TArray<FName>(), FStreamableDelegate::CreateLambda( [this]()
	{
		UAssetManager& AssetManager = UAssetManager::Get();
		TArray<FAssetData> UnitTypeAbilityDataAssetNames;
		AssetManager.GetPrimaryAssetDataList( FPrimaryAssetType{UUnitOrderUnitTypeAbilityData::StaticClass()->GetFName()}, UnitTypeAbilityDataAssetNames );
		{
			if ( UnitTypeAbilityDataAssetNames.Num() == 0 )
			{
				// 3. Загрузка всех UnitSubtypeData.
				UE_LOG( LogUnitOrderSystem, Warning, TEXT("%s(). Ended. UUnitOrderUnitTypeAbilityData Not Found."), *FString(__FUNCTION__) );
				LoadingUnitSubtypeData();
				return;
			}
		}

		for ( auto& LoadedAssetDataName : UnitTypeAbilityDataAssetNames )
		{
			UUnitOrderUnitTypeAbilityData* DataAsset = Cast<UUnitOrderUnitTypeAbilityData>( AssetManager.GetPrimaryAssetObject( LoadedAssetDataName.GetPrimaryAssetId() ) );
			{
				if ( !IsValid( DataAsset ) )
				{
					UE_LOG( LogUnitOrderSystem, Warning, TEXT("DataAssetName (%s) invalid"), *LoadedAssetDataName.AssetName.ToString() );
					continue;
				}
				if ( !DataAsset->ValidateDataAsset() )
				{
					ensureAlwaysMsgf( false, TEXT("%s(). DataAsset (%s) has invalid data"), *FString(__FUNCTION__), *LoadedAssetDataName.AssetName.ToString() );
					continue;
				}
			} // This check cannot be moved from shipping build. Otherwise it will bring to crash.

			// Add new abilities to existing UnitTypeData.
			bool bCheckForError_TagNotFound = true;
			for ( auto& ExistingUnitTypeData : AllFullUnitTypeData )
			{
				{
					if ( !IsValid( ExistingUnitTypeData.UnitTypeData ) )
					{
						ensureAlwaysMsgf( false, TEXT("%s(). !IsValid(ExistingUnitTypeData.UnitTypeData)."), *FString(__FUNCTION__) );
						continue;
					}
				}
				if ( ExistingUnitTypeData.UnitTypeData->UnitTypeTag == DataAsset->UnitTypeTag )
				{
					bCheckForError_TagNotFound = false;

					// Проверяем все новые абилки на наличие в списке старых абилок.
					for ( auto& NewAbilityData : DataAsset->AllUnitAbilities )
					{
						// Check if new ability already exists.
						{
							bool bAbilityExists = false;
							for ( auto& ExistingAbilityData : ExistingUnitTypeData.AllUnitAbilities )
							{
								// Если они уже есть, то это ошибка.
								if ( NewAbilityData.AbilityTag == ExistingAbilityData.AbilityTag )
								{
									UE_LOG( LogUnitOrderSystem, Warning,
									        TEXT("%s(). Ability with tag (%s) already exists. DataAsset Name: %s"),
									        *FString(__FUNCTION__),
									        *ExistingAbilityData.AbilityTag.ToString(),
									        *DataAsset->GetName() );
									bAbilityExists = true;
									break;
								}

								if ( NewAbilityData.UnitAbilityClass == ExistingAbilityData.UnitAbilityClass )
								{
									UE_LOG( LogUnitOrderSystem, Warning,
									        TEXT("%s(). UnitAbilityClass (%s) already exists. DataAsset Name: %s"),
									        *FString(__FUNCTION__),
									        *NewAbilityData.UnitAbilityClass.ToString(),
									        *DataAsset->GetName() );
									bAbilityExists = true;
									break;
								}
							}
							if ( bAbilityExists )
							{
								continue;
							};
						}

						ExistingUnitTypeData.AllUnitAbilities.Add( NewAbilityData );
					}

					// Если нет, то добавляем абилку в список.
					break;
				}
			}

			if ( bCheckForError_TagNotFound )
			{
				UE_LOG( LogUnitOrderSystem, Warning,
				        TEXT("%s(). UnitTypeTag Not Found. Serached Tag: %s. DataAsset Name: %s"),
				        *FString(__FUNCTION__),
				        *DataAsset->UnitTypeTag.ToString(),
				        *DataAsset->GetName() );
			}
		}


		// 3. Загрузка всех UnitSubtypeData.
		//UE_LOG( LogUnitOrderSystem, Warning, TEXT("Async LoadingUnitTypeAbilityData(). Ended") );
		LoadingUnitSubtypeData();
	} ) );
}

void UUnitOrderSubsystem::LoadingUnitSubtypeData()
{
	const FPrimaryAssetType UnitTypeDataAssetType = FPrimaryAssetType{UUnitOrderUnitSubtypeData::StaticClass()->GetFName()};
	UAssetManager::Get().LoadPrimaryAssetsWithType( UnitTypeDataAssetType, TArray<FName>(), FStreamableDelegate::CreateLambda( [this]()
	{
		UAssetManager& AssetManager = UAssetManager::Get();
		TArray<FAssetData> UnitTypeDataAssetNames;
		AssetManager.GetPrimaryAssetDataList( FPrimaryAssetType{UUnitOrderUnitSubtypeData::StaticClass()->GetFName()}, UnitTypeDataAssetNames );
		{
			if ( UnitTypeDataAssetNames.Num() == 0 )
			{
				UE_LOG( LogUnitOrderSystem, Warning, TEXT("%s(). No UUnitOrderUnitSubtypeData found"), *FString( __FUNCTION__ ) );
				InitializationUnitDataCompllite();
				return;
			}
		} // Check Array for empty state.

		for ( auto& LoadedAssetDataName : UnitTypeDataAssetNames )
		{
			UUnitOrderUnitSubtypeData* NewDataAsset = Cast<UUnitOrderUnitSubtypeData>( AssetManager.GetPrimaryAssetObject( LoadedAssetDataName.GetPrimaryAssetId() ) );
			{
				{
					if ( !IsValid( NewDataAsset ) )
					{
						UE_LOG( LogUnitOrderSystem, Warning,
						        TEXT("LoadingUnitSubtypeData()::Lambda. DataAssetName (%s) invalid"),
						        *LoadedAssetDataName.AssetName.ToString() );
						continue;
					}
					if ( !NewDataAsset->ValidateDataAsset() )
					{
						UE_LOG( LogUnitOrderSystem, Warning,
						        TEXT("DataAsset (%s) has invaid Data"),
						        *LoadedAssetDataName.AssetName.ToString() );
						continue;
					}
				} // Check NewDataAsset pointer and Validate stored Data.

				bool bUnitTypeNotFound = true;
				for ( auto& ExistingUnitType : AllFullUnitTypeData )
				{
					if ( IsValid( ExistingUnitType.UnitTypeData ) && NewDataAsset->UnitSubtypeTag.MatchesTag( ExistingUnitType.UnitTypeData->UnitTypeTag ) )
					{
						bUnitTypeNotFound = false;

						{
							bool bUnitSubtypeExists = false;
							for ( auto& UnitSubtypeData : ExistingUnitType.UnitSubtypes )
							{
								{
									if ( !IsValid( UnitSubtypeData.UnitSubtypeData ) )
									{
										ensureAlwaysMsgf( false, TEXT("%(). !IsValid(UnitSubtypeData.UnitSubtypeData). "), *FString(__FUNCTION__) );
										continue;
									}
								}

								if ( UnitSubtypeData.UnitSubtypeData->UnitSubtypeTag == NewDataAsset->UnitSubtypeTag )
								{
									bUnitSubtypeExists = true;
									UE_LOG( LogUnitOrderSystem, Warning,
									        TEXT("%s(). UnitSubtype (%s) is Already Exists. DataAsset Name: %s."),
									        *FString(__FUNCTION__),
									        *NewDataAsset->UnitSubtypeTag.ToString(),
									        *NewDataAsset->GetName() );
									break;
								}
							}
							if ( bUnitSubtypeExists )
							{
								continue;
							}
						} // Check. All units data must be unique. Is new UnitSubtype already existing in the array?

						ExistingUnitType.UnitSubtypes.Add( FUnitOrderUnitSubtype{NewDataAsset} );
					}
				}
				if ( bUnitTypeNotFound )
				{
					UE_LOG( LogUnitOrderSystem, Error,
					        TEXT("LoadingUnitSubtypeData()::Async::Lambda. Cannot find UnitType for Subtype by tag: %s. DataAsset Name: %s."),
					        *NewDataAsset->UnitSubtypeTag.ToString(),
					        *NewDataAsset->GetName() );
				}
			}
		}

		// 4. Загрузка всех UnitTypeAbilityData.
		//UE_LOG( LogUnitOrderSystem, Warning, TEXT("Async LoadingUnitSubtypeData(). Ended") );
		LoadingUnitSubtypeAbilityData();
	} ) );
}

void UUnitOrderSubsystem::LoadingUnitSubtypeAbilityData()
{
	//TODO Last check and find ability tags and add it to the subtypes Data.
	const FPrimaryAssetType UnitTypeDataAssetType = FPrimaryAssetType{UUnitOrderUnitSubtypeAbilityData::StaticClass()->GetFName()};
	UAssetManager::Get().LoadPrimaryAssetsWithType( UnitTypeDataAssetType, TArray<FName>(), FStreamableDelegate::CreateLambda( [this]()
	{
		UAssetManager& AssetManager = UAssetManager::Get();
		TArray<FAssetData> UnitTypeDataAssetNames;
		AssetManager.GetPrimaryAssetDataList( FPrimaryAssetType{UUnitOrderUnitSubtypeAbilityData::StaticClass()->GetFName()}, UnitTypeDataAssetNames );
		{
			if ( UnitTypeDataAssetNames.Num() == 0 )
			{
				UE_LOG( LogUnitOrderSystem, Warning, TEXT("%s(). No UUnitOrderUnitSubtypeAbilityData found. Ended"), *FString( __FUNCTION__ ) );
				InitializationUnitDataCompllite();
				return;
			}
		} // Check Array for empty state.

		for ( auto& LoadedAssetDataName : UnitTypeDataAssetNames )
		{
			UUnitOrderUnitSubtypeAbilityData* NewDataAsset = Cast<UUnitOrderUnitSubtypeAbilityData>( AssetManager.GetPrimaryAssetObject( LoadedAssetDataName.GetPrimaryAssetId() ) );
			{
				{
					if ( !IsValid( NewDataAsset ) )
					{
						UE_LOG( LogUnitOrderSystem, Warning, TEXT("DataAssetName (%s) invalid"), *LoadedAssetDataName.AssetName.ToString() );
						continue;
					}
					if ( !NewDataAsset->ValidateDataAsset() )
					{
						UE_LOG( LogUnitOrderSystem, Warning, TEXT("DataAsset (%s) has invaid Data"), *LoadedAssetDataName.AssetName.ToString() );
						continue;
					}
				} // Check NewDataAsset pointer and Validate stored Data.

				bool bUnitTypeNotFound = true;

				// Ищем существующий тип юнита. Он должен уже существовать. Если нет, то это ошибка.
				for ( auto& ExistingUnitType : AllFullUnitTypeData )
				{
					// Нашли тип юнита, к которому будут относиться новые теги абилок.
					if ( IsValid( ExistingUnitType.UnitTypeData ) && NewDataAsset->UnitSubtypeTag.MatchesTag( ExistingUnitType.UnitTypeData->UnitTypeTag ) )
					{
						{
							bUnitTypeNotFound = false;
						} // UnitType was found.

						// Поиск подтипа юнита, к которому будут относиться новые теги абилок.
						bool bUnitSubtypeNotFound = true;
						for ( auto& ExistingUnitSubtypeData : ExistingUnitType.UnitSubtypes )
						{
							{
								if ( !IsValid( ExistingUnitSubtypeData.UnitSubtypeData ) )
								{
									ensureAlwaysMsgf( false, TEXT("%(). !IsValid(UnitSubtypeData.UnitSubtypeData). "), *FString(__FUNCTION__) );
									continue;
								}
							}

							// Нашли Подтип - начинаем добавлять теги доступных абилок для подтипа.
							if ( ExistingUnitSubtypeData.UnitSubtypeData->UnitSubtypeTag == NewDataAsset->UnitSubtypeTag )
							{
								bUnitSubtypeNotFound = false;

								// Прогоняем все новые абилки на совпадение в уже существующих абилках.
								// Если совпадение есть, то это ошибка.
								for ( auto& NewAbilityTag : NewDataAsset->AvailableAbilityTags )
								{
									{
										bool bAbilityTagAleradyAdded = false;
										for ( auto& ExistingAbilityTag : ExistingUnitSubtypeData.AvailableAbilityTags )
										{
											if ( NewAbilityTag == ExistingAbilityTag )
											{
												UE_LOG( LogUnitOrderSystem, Warning,
												        TEXT("%s(). AbilityTag (%s) already added to UnitSubtype (%s). DataAsset Name: %s"),
												        *FString(__FUNCTION__),
												        *NewAbilityTag.ToString(),
												        *NewDataAsset->UnitSubtypeTag.ToString(),
												        *NewDataAsset->GetName() );
												bAbilityTagAleradyAdded = true;
												break;
											}
										}

										if ( bAbilityTagAleradyAdded )
										{
											continue;
										}
									} // Check for doubles. If double found, then it is error. Otherwise add new ability tag to the array.

									ExistingUnitSubtypeData.AvailableAbilityTags.Add( NewAbilityTag );
								}

								break;
							}
						}


						{
							if ( bUnitSubtypeNotFound )
							{
								UE_LOG( LogUnitOrderSystem, Warning,
								        TEXT("%s(). Cannot find Subtype (%s) for Abilities. DataAsset Name: %s"),
								        *FString(__FUNCTION__),
								        *NewDataAsset->UnitSubtypeTag.ToString(),
								        *NewDataAsset->GetName() );
							}
						} // Checking if UnitSubtype was found.
					}
				}

				{
					if ( bUnitTypeNotFound )
					{
						UE_LOG( LogUnitOrderSystem, Warning,
						        TEXT("%s(). Cannot find UnitType for Subtype (%s). DataAsset Name: %s"),
						        *FString(__FUNCTION__),
						        *NewDataAsset->UnitSubtypeTag.ToString(),
						        *NewDataAsset->GetName() );
					}
				} // Checking if UnitType was found.
			}
		}

		// Вс загрузка завершена. Делегат на сообщение о завершении загрузки.
		//UE_LOG( LogUnitOrderSystem, Warning, TEXT("Async LoadingUnitSubtypeAbilityData(). Ended") );
		InitializationUnitDataCompllite();
	} ) );
}

void UUnitOrderSubsystem::InitializationUnitDataCompllite()
{
	bAllUnitDataInitialized = true;
	if ( LoadingUnitDataComplited.IsBound() )
	{
		LoadingUnitDataComplited.Broadcast();
	};

	UE_LOG( LogUnitOrderSystem, Warning, TEXT("%s(). Ended"), *FString(__FUNCTION__) );
}
