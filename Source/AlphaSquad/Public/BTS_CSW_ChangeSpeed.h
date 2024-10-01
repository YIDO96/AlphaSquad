// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_CSW_ChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class ALPHASQUAD_API UBTS_CSW_ChangeSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	

public:
	UBTS_CSW_ChangeSpeed();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowprivateAccess = "yes"))
	float chaseSpeed = 500;
};
