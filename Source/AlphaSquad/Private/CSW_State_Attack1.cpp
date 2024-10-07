// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW_State_Attack1.h"
#include "CSW_Enemy.h"
#include "CSW_EnemyAnim.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"

void UCSW_State_Attack1::EnterState()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack1 Enter"));

	
	// 몽타주 플레이 하기
	ACharacter* OwnerEnemy = Cast<ACharacter>(GetOuter());

	if (OwnerEnemy)
	{
		ACSW_Enemy* enemy = Cast<ACSW_Enemy>(OwnerEnemy);

		if (enemy && enemy->Pattern_Montage_A1)
		{
			auto* TargetActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			// direct to player
			if (TargetActor)
			{
				FVector Dir = TargetActor->GetActorLocation() - enemy->GetActorLocation();
				Dir.Z = 0;

				FRotator newRot = Dir.Rotation();
				enemy->SetActorRotation(newRot);
			}

			UAnimInstance* AnimInstance = enemy->GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->Montage_Play(enemy->Pattern_Montage_A1);
				UE_LOG(LogTemp, Warning, TEXT("Playing Attack Montage"));

			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("enemy and Montage NULL"));
		}
	}
	
}

void UCSW_State_Attack1::UpdateState()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack1 Update"));

	
}

void UCSW_State_Attack1::ExitState()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack1 Exit"));
}
