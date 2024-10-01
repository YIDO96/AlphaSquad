// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW_State_Attack2.h"

void UCSW_State_Attack2::EnterState()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack2 Enter"));
}

void UCSW_State_Attack2::UpdateState()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack2 Update"));
}

void UCSW_State_Attack2::ExitState()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack2 Exit"));
}
