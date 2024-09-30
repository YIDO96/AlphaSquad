// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindePlayerLocation_Boss.generated.h"

/**
 * 
 */
UCLASS()
class ALPHASQUAD_API UBTTask_FindePlayerLocation_Boss : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	//생성자 및 ExcuteTask 재정의
public:
	explicit UBTTask_FindePlayerLocation_Boss(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category="Search" ,meta=(AllowPrivateAccess="true"))
	bool SearchRandom = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category="Search" ,meta=(AllowPrivateAccess="true"))
	float SearchRadis = 10000.f;		
	
};
