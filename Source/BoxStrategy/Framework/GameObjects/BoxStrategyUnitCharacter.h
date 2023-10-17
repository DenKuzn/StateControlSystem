// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BoxStrategyUnitCharacter.generated.h"

class AAIController;
class UBoxStrategyAbilitySystemComponent;
class UStateControlInteractComponent;

UCLASS()
class BOXSTRATEGY_API ABoxStrategyUnitCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoxStrategyUnitCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStateControlInteractComponent> InteractComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxStrategyAbilitySystemComponent> BoxStrategyUnitAbilitySystemComponent = nullptr;

public:
	UFUNCTION(BlueprintPure)
	UStateControlInteractComponent* GetInteractComponent();

	UFUNCTION(BlueprintPure)
	AAIController* GetAIUnitController();

	UFUNCTION(BlueprintPure)
	UBoxStrategyAbilitySystemComponent* GetBoxStrategyAbilitySystemComponent();
};
