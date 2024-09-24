// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "NPC_Boss.generated.h"

UCLASS()
class ALPHASQUAD_API ANPC_Boss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC_Boss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// BehaviorTree 선언
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* Tree;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Behavior Tree Function
	UBehaviorTree* GetBehaviorTree();
	
};
