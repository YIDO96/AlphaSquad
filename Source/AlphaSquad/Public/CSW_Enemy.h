// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "CSW_EnemyCombatInterface.h"
#include "CSW_Enemy.generated.h"

UCLASS()
class ALPHASQUAD_API ACSW_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACSW_Enemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UBehaviorTree* GetBeHaviorTree() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UBehaviorTree* Tree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern")
	FString Pattern;

	void CombateStateExcute();

private:
	TMap<FString, TSubclassOf<UCSW_EnemyCombatInterface>> StateMap;

	// 현재 상태
	ICSW_EnemyCombatInterface* CurrentState;

	TArray<FString> ParsePatternString(const FString& PatternString);
	
	TArray<FString> ParsedPatterns;

};
