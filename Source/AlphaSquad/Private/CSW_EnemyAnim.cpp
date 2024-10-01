// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW_Enemy.h"
#include "CSW_EnemyAnim.h"

void UCSW_EnemyAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto* EnemyPawn = TryGetPawnOwner();

	if (EnemyPawn)
	{
		FVector velocity = EnemyPawn->GetVelocity();
		FVector forwardVector = EnemyPawn->GetActorForwardVector();
		speed = FVector::DotProduct(forwardVector, velocity);
	}
}
