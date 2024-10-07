// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_CSW_ChangeSpeed.h"

#include "CSW_AIController.h"
#include "CSW_Enemy.h"
#include "GameFramework\CharacterMovementComponent.h"

UBTS_CSW_ChangeSpeed::UBTS_CSW_ChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("CSW_Change_Speed");
}

void UBTS_CSW_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (auto const cont = OwnerComp.GetAIOwner())
	{
		if (auto* const enemy = Cast<ACSW_Enemy>(cont->GetPawn()))
		{
			enemy->GetCharacterMovement()->MaxWalkSpeed = chaseSpeed;
		}
	}
}
