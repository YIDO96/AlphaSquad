// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocation_Boss.h"
#include "NavigationSystem.h"
#include "Boss_AIController.h"
#include "NPC_Boss.h"
#include "BehaviorTree/BlackboardComponent.h"

// 생성자: 이 Task의 이름을 "Find Random Location In NavMesh"로 설정
UBTTask_FindRandomLocation_Boss::UBTTask_FindRandomLocation_Boss(FObjectInitializer const& ObjectInitializer)
{
	// Task의 노드 이름 설정 (Behavior Tree 편집기에서 보일 이름)
	NodeName = "Find Random Location In NavMesh";
}

// Task 실행 함수: Behavior Tree에서 이 Task가 실행될 때 호출됨
EBTNodeResult::Type UBTTask_FindRandomLocation_Boss::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// AI Controller와 해당 NPC(Pawn)를 가져옴
	if (ABoss_AIController* const cont = Cast<ABoss_AIController>(OwnerComp.GetAIOwner())) // AI Controller 캐스팅
	{
		// AI Controller가 제어하는 Pawn(NPC)을 가져옴
		if (auto* const npc_boss = cont->GetPawn()) 
		{
			// NPC의 현재 위치를 임의 위치 검색의 시작점으로 설정
			auto const Origin = npc_boss->GetActorLocation(); 
            
			// UNavigationSystemV1을 통해 현재 월드의 네비게이션 시스템을 가져옴
			if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation Loc; // 네비게이션 메쉬 상에서 찾은 위치를 저장할 변수
                
				// 주어진 반경(SearchRadius) 내에서 임의의 위치를 NavMesh 상에서 찾음
				if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, Loc))
				{
					// 찾은 위치를 블랙보드에 저장하여 Behavior Tree에서 사용 가능하게 함
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
				}

				// Task 성공으로 마침
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); 
				return EBTNodeResult::Succeeded;
			}
		}
	}
    
	// 실패 시 부모 클래스의 기본 Task 실행 결과를 반환
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
