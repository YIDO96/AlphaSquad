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

	void ExcutePatternWithDelay(int32 PatternIndex);
	void OnPatternExcutionComplate();

	FTimerHandle TimerHandle;

	float MontageDuration;
	float CurrentPatternIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool isBoss;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float attackRang = 2000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float Hp = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	int32 EnemyMoney = 20;



	// 데미지 받았을때 ?
	UFUNCTION()
	void OnTakeDamage(float Damage);

private:
	TMap<FString, TSubclassOf<UCSW_EnemyCombatInterface>> StateMap;

	// ���� ����
	ICSW_EnemyCombatInterface* CurrentState;

	TArray<FString> ParsePatternString(const FString& PatternString);
	
	TArray<FString> ParsedPatterns;


	

public:

#pragma region MontageCollection

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* Pattern_Montage_A1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* Pattern_Montage_A2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* Pattern_Montage_A3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* Pattern_Montage_B1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* Pattern_Montage_B2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* Pattern_Montage_B3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* Pattern_Montage_B4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* Pattern_Montage_B5;
#pragma endregion

};
