// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW_State_Attack3.h"

void UCSW_State_Attack3::EnterState()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack3 Enter"));
}

void UCSW_State_Attack3::UpdateState()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack3 Update"));
}

void UCSW_State_Attack3::ExitState()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack3 Exit"));
}
