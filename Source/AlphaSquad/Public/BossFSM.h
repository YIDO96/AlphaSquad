// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossFSM.generated.h"


//사용할 상태 정의
UENUM(BlueprintType)
enum class EBossState : uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALPHASQUAD_API UBossFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	//상태변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM)
	EBossState mState = EBossState::Idle;
	
	//대기 상태
	void IdleState();
	//이동 상태
	void MoveState();
	//공격 상태
	void AttackState();
	//피격 상태
	void DamageState();
	//죽음 상태
	void DieState();

	// 대기 시간
	UPROPERTY(EditDefaultsOnly, Category=FSM)
	float idleDelayTime = 0;
	// 경과 시간
	float currentTime = 0;

	// 타깃
	UPROPERTY(VisibleAnywhere, Category=FSM)
	class ATPSPlayer* target;

	// 소유 액터
	UPROPERTY()
	class ANPC_Boss* me;
	
};
