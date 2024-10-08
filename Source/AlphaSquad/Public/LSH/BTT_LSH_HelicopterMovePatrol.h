// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_LSH_HelicopterMovePatrol.generated.h"

/**
 * 
 */
UCLASS()
class ALPHASQUAD_API UBTT_LSH_HelicopterMovePatrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	public:
	explicit UBTT_LSH_HelicopterMovePatrol(FObjectInitializer const& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
