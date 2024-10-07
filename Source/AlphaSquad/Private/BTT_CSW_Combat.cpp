// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_CSW_Combat.h"
#include "CSW_Enemy.h"
#include "CSW_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"



UBTT_CSW_Combat::UBTT_CSW_Combat(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("CSW_CombatMode");
}

EBTNodeResult::Type UBTT_CSW_Combat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 전투 모드 돌입
	UE_LOG(LogTemp, Warning, TEXT("Combat Mode"));

	if (auto* const cont = Cast<ACSW_AIController>(OwnerComp.GetAIOwner()))
	{
		if (auto* enemy = Cast<ACSW_Enemy>(cont->GetPawn()))
		{
			enemy->CombateStateExcute();
		}
	}

	return EBTNodeResult::InProgress;
}
