// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_CSW_Combat.h"

UBTT_CSW_Combat::UBTT_CSW_Combat(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("CSW_CombatMode");
}

EBTNodeResult::Type UBTT_CSW_Combat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 전투 모드 돌입
	UE_LOG(LogTemp, Warning, TEXT("Combat Mode"));

	return EBTNodeResult::InProgress;
}
