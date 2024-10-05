// Fill out your copyright notice in the Description page of Project Settings.


#include "BossFSM.h"

#include "TPSPlayer.h"
#include "AlphaSquad/NPC_Boss.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UBossFSM::UBossFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBossFSM::BeginPlay()
{
	Super::BeginPlay();

	// 월드에서 ATPSPlayer 타깃 찾아오기
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());
	// ATPSPlayer 타입으로 캐스팅
	target = Cast<ATPSPlayer>(actor);
	// 소유 객체 가져오기
	me = Cast<ANPC_Boss>(GetOwner());
	
	
}


// Called every frame
void UBossFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 실행창에 상태 메시지 출력하기
	FString logMsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0,1,FColor::Cyan,logMsg);
	
	//mState의 값에 따라 선언해 준 상태 함수가 호출 되도록 처리
	switch(mState)
	{
	case EBossState::Idle:
		IdleState();
		break;
	case EBossState::Move:
		MoveState();
		break;
	case EBossState::Attack:
		AttackState();
		break;
	case EBossState::Damage:
		DamageState();
		break;
	case EBossState::Die:
		DieState();
		break;
	}
	
}

void UBossFSM::IdleState()
{
	// 1. 시간이 흘렀으니까
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. 만약 경과 시간이 대기 시간을 초과했다면
	if(currentTime> idleDelayTime)
	{
		// 3. 이동 상태로 전환하고 싶다.
		mState = EBossState::Move;
		//경과 시간 초기화
		currentTime = 0;
	}
}

void UBossFSM::MoveState()
{
	/*
	// 1. 타깃 목적지가 필요하다.
	FVector destination = target->GetActorLocation();
	// 2. 방향이 필요하다.
	FVector dir = destination - me->GetActorLocation();
	// 방향으로 이동하고 싶다.
	me->AddMovementInput(dir.GetSafeNormal());

	// 타깃과 가까워지면 공격 상태로 전환하고 싶다.
	// 1. 만약 거리가 공격 범위 안에 들어오면
	if(dir.Size()<attackRange)
	{
		// 2. 공격 상태로 전환하고 싶다
		mState = EBossState::Attack;
	}
	*/
}

// 공격 상태
void UBossFSM::AttackState()
{	
	//목표:일정 시간에 한 번씩 공격하고 싶다.
	//1. 시간이 흘러야 한다.
	currentTime += GetWorld()->DeltaTimeSeconds;
	//2. 공격 시간이 됐으니까
	if(currentTime > attackDelayTime)
	{
		// 3. 공격하고 싶다.
		UE_LOG(LogTemp, Warning , TEXT("Attack"));
		//경과 시간 초기화
		currentTime = 0;
	}

	//목표: 타깃이 공격 범위를 벗어나면 상태를 이동으로 전환하고 싶다.
	// 1. 타깃과의 거리가 필요하다.
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	// 2. 타깃과의 거리가 공격 범위를 벗어났으니까
	if(distance>attackRange)
	{
		// 3. 상태를 이동으로 전환하고 싶다.
		mState = EBossState::Move;
		
	}
	
}

void UBossFSM::DamageState()
{
}

void UBossFSM::DieState()
{
}

void UBossFSM::OnDamageProcess()
{
	
	// me->Destroy();
}

