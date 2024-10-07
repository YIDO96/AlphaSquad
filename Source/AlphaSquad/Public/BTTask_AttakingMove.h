// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_AttakingMove.generated.h"

/**
 * 
 */
UCLASS()
class ALPHASQUAD_API UBTTask_AttakingMove : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_AttakingMove(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	// 타깃
	UPROPERTY(VisibleAnywhere, Category=FSM)
	class ATPSPlayer* target;

	// 소유 액터
	UPROPERTY()
	class ANPC_Boss* me;
};
