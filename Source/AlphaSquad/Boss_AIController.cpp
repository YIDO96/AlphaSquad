// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_AIController.h"

#include "AlphaSquadCharacter.h"
#include "NPC_Boss.h"
#include "SOS_TestingCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

// 생성자
ABoss_AIController::ABoss_AIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}

// AIController가 Pawn(캐릭터)를 소유할 때 호출되는 함수
void ABoss_AIController::OnPossess(APawn* InPawn)
{
	// 부모 클래스의 OnPossess 함수를 호출하여 기본적인 소유 로직을 처리
	Super::OnPossess(InPawn);

	// InPawn이 ANPC_Boss 클래스의 인스턴스인지 확인하고, 맞다면 npc_boss 변수에 캐스팅하여 할당
	if(ANPC_Boss* const npc_boss = Cast<ANPC_Boss>(InPawn))
	{
		// npc_boss의 Behavior Tree가 존재하는지 확인하고, tree 변수에 할당
		if(UBehaviorTree* const tree = npc_boss->GetBehaviorTree())
		{
			UBlackboardComponent* b; // 블랙보드 컴포넌트 포인터 변수 선언

			// 블랙보드와 연관된 데이터 세트를 가져와서 b에 할당
			UseBlackboard(tree->BlackboardAsset, b);

			// b를 현재 컨트롤러의 블랙보드로 설정
			Blackboard = b;

			// Behavior Tree 실행
			RunBehaviorTree(tree);
		}
	}
}

// AI의 시각 인식 시스템을 설정하는 함수입니다.
void ABoss_AIController::SetupPerceptionSystem()
{
	// 시각 감지 설정(Sight Config) 생성
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	
	// 시각 감지 설정이 유효한 경우에만 실행
	if(SightConfig)
	{
		// AI Perception Component 생성 및 설정 (AI의 감각 시스템을 처리하는 컴포넌트)
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		// 시각 감지의 기본 설정: 감지 반경을 500 유닛으로 설정
		SightConfig->SightRadius = 1000.f;
		// 시야를 잃을 때의 반경 설정 (감지 반경 + 25 유닛)
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		// 감지 정보의 유효 시간 설정: 5초 동안 유지
		SightConfig->SetMaxAge(5.f);
		// 마지막으로 감지된 위치로부터 520 유닛 안에 있으면 자동으로 감지 성공
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 2000.f;
		// 적, 아군, 중립 캐릭터 모두 감지할 수 있도록 설정
		SightConfig->DetectionByAffiliation.bDetectEnemies = true; // 적 감지
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true; // 아군 감지
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true; // 중립 감지

		// 감지 시스템에서 기본적으로 사용할 감각을 시각(Sight)으로 설정
		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());

		// 목표가 감지되거나 업데이트될 때 호출될 이벤트 바인딩
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ABoss_AIController::OnTargetDetected);

		// 시각 감지 설정을 Perception Component에 적용
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}


// AI가 목표(Actor)를 감지했을 때 호출되는 함수입니다.
void ABoss_AIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	// 감지된 Actor가 ASOS_TestingCharacter 타입인지 확인 (플레이어 캐릭터일 가능성)
	if (auto* const ch = Cast<ASOS_TestingCharacter>(Actor))
	{
		// AI의 블랙보드에서 "CanSeePlayer"라는 키 값을 업데이트
		// Stimulus.WasSuccessfullySensed()는 AI가 이 캐릭터를 감지했는지 여부를 반환 (true/false)
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
		
	}
}

