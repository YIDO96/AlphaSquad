// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW_State_Attack3.h"
#include "CSW_Enemy.h"
#include "CSW_EnemyAnim.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"


void UCSW_State_Attack3::EnterState()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack3 Enter"));
	
	// 몽타주 플레이 하기
	ACharacter* OwnerEnemy = Cast<ACharacter>(GetOuter());
	if (OwnerEnemy)
	{
		ACSW_Enemy* enemy = Cast<ACSW_Enemy>(OwnerEnemy);

		if (enemy && enemy->Pattern_Montage_A3)
		{
			UAnimInstance* AnimInstance = enemy->GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->Montage_Play(enemy->Pattern_Montage_A3);
				UE_LOG(LogTemp, Warning, TEXT("Playing Attack Montage"));

			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("enemy and Montage NULL"));
		}
	}
}

void UCSW_State_Attack3::UpdateState()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack3 Update"));
}

void UCSW_State_Attack3::ExitState()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack3 Exit"));
}
