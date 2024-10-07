// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW_ShootingAnimNotifyState.h"

void UCSW_ShootingAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	// coll cude colliting 
	UE_LOG(LogTemp, Warning, TEXT("Fire in the hall"));
}

void UCSW_ShootingAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
}

void UCSW_ShootingAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// Actor off 
}
