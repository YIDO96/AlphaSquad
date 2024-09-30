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
	// 플레이어 이동
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerAnim")
	float speed = 0;
	// 플레이어 좌우 이동 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerAnim")
	float direction = 0;

	// 매프레임 갱신되는 함수
	// 블루프린트에 Event Blueprint Update Animation 노드와 대응되는 함수 NativeUpdateAnimation
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
