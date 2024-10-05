// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSW_EnemyCombatInterface.h"
#include "UObject/NoExportTypes.h"
#include "CSW_State_bAttack4.generated.h"

/**
 * 
 */
UCLASS()
class ALPHASQUAD_API UCSW_State_bAttack4 : public UObject, public ICSW_EnemyCombatInterface
{
	GENERATED_BODY()
public:
	virtual void EnterState() override;
	virtual void UpdateState() override;
	virtual void ExitState() override;
};
