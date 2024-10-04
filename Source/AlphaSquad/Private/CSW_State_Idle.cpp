// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW_State_Idle.h"

void UCSW_State_Idle::EnterState()
{
	UE_LOG(LogTemp, Warning, TEXT("Idle Enter"));
}

void UCSW_State_Idle::UpdateState()
{
	UE_LOG(LogTemp, Warning, TEXT("Idle Update"));
}

void UCSW_State_Idle::ExitState()
{
	UE_LOG(LogTemp, Warning, TEXT("Idle Exit"));
}
