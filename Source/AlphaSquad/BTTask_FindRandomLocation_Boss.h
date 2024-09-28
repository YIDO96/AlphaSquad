// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindRandomLocation_Boss.generated.h"

/**
 * 
 */
UCLASS()
class ALPHASQUAD_API UBTTask_FindRandomLocation_Boss : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_FindRandomLocation_Boss(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	// Search able range
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta = (AllowPrivateAccess = "true"))
	float SearchRadius = 5000.f;
	
};
