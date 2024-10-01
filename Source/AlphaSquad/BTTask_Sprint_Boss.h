// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Sprint_Boss.generated.h"

/**
 * 
 */
UCLASS()
class ALPHASQUAD_API UBTTask_Sprint_Boss : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_Sprint_Boss(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
};
