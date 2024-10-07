// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/BTT_LSH_HelicopterMovePatrol.h"

UBTT_LSH_HelicopterMovePatrol::UBTT_LSH_HelicopterMovePatrol(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("LSH_HelicopterMovePatrol");
}

EBTNodeResult::Type UBTT_LSH_HelicopterMovePatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, 
uint8* NodeMemory)
{




	return EBTNodeResult::InProgress;
}