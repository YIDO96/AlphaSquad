// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_AIController.h"

#include "NPC_Boss.h"

// 생성자
ABoss_AIController::ABoss_AIController(FObjectInitializer const& ObjectInitializer)
{
	
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