// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Boss_AIController.generated.h"

/**
 * 
 */
UCLASS()
class ALPHASQUAD_API ABoss_AIController : public AAIController
{
	GENERATED_BODY()
public:
	explicit ABoss_AIController(FObjectInitializer const& ObjectInitializer);
protected:
	virtual void OnPossess(APawn* InPawn) override;
};
