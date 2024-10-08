// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CSW_EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class ALPHASQUAD_API UCSW_EnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	// �÷��̾� �̵�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerAnim")
	float speed = 0;
	// �÷��̾� �¿� �̵� �ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerAnim")
	float direction = 0;

	// �������� ���ŵǴ� �Լ�
	// �������Ʈ�� Event Blueprint Update Animation ���� �����Ǵ� �Լ� NativeUpdateAnimation
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
