// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttakingMove.h"

#include "AlphaSquad/NPC_Boss.h"
#include "NSK/TPSPlayer.h"

UBTTask_AttakingMove::UBTTask_AttakingMove(FObjectInitializer const& ObjectInitializer)
{
	// 이 노드의 이름을 "Find Player Location"으로 설정
	NodeName = TEXT("Attacking Move");
}

EBTNodeResult::Type UBTTask_AttakingMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	// 1. 타깃 목적지가 필요하다.
	FVector destination = target->GetActorLocation();
	// 2. 방향이 필요하다.
	FVector dir = destination - me->GetActorLocation();
	// 방향으로 이동하고 싶다.
	me->AddMovementInput(dir.GetSafeNormal());

	// 타깃과 가까워지면 공격 상태로 전환하고 싶다.
	// 1. 만약 거리가 공격 범위 안에 들어오면
	/*
	if(dir.Size()<attackRange)
	{
		// 2. 공격 상태로 전환하고 싶다
		mState = EBossState::Attack;
	}
	*/
	return Super::ExecuteTask(OwnerComp, NodeMemory);
	
}
