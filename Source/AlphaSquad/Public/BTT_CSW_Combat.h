// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_CSW_Combat.generated.h"

/**
 * 
 */
UCLASS()
class ALPHASQUAD_API UBTT_CSW_Combat : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public: 
	explicit UBTT_CSW_Combat(FObjectInitializer const& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
