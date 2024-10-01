// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW_State_Attack1.h"

void UCSW_State_Attack1::EnterState()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack1 Enter"));
}

void UCSW_State_Attack1::UpdateState()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack1 Update"));
}

void UCSW_State_Attack1::ExitState()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack1 Exit"));
}
