// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindePlayerLocation_Boss.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// 생성자: Task의 이름을 설정합니다.
UBTTask_FindePlayerLocation_Boss::UBTTask_FindePlayerLocation_Boss(FObjectInitializer const& ObjectInitializer)
{
	// 이 노드의 이름을 "Find Player Location"으로 설정
	NodeName = TEXT("Find Player Location");
}

// 태스크 실행 함수: 플레이어의 위치를 찾고, AI가 이를 기반으로 행동하게 합니다.
EBTNodeResult::Type UBTTask_FindePlayerLocation_Boss::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// UGameplayStatics를 사용하여 0번 인덱스(싱글 플레이어 모드에서는 0번)를 기준으로 플레이어 캐릭터를 가져옵니다.
	// 이 방식은 멀티플레이어일 경우 각 플레이어에 따라 달라집니다.
	if(auto* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(),0))
	{
		// 플레이어의 현재 위치를 가져와서 이후 로직에서 사용하기 위해 저장합니다.
		auto const PlayerLocation = Player->GetActorLocation();

		// SearchRandom 플래그가 참이면, 플레이어 근처의 무작위 위치를 찾습니다.
		if(SearchRandom)
		{
			FNavLocation Loc; // 네비게이션 시스템에서 얻은 랜덤 위치를 저장할 변수

			// 네비게이션 시스템을 가져옵니다. 이는 AI가 이동할 수 있는 영역을 정의하는 시스템입니다.
			if(auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				// 플레이어 위치를 기준으로 지정된 반경(SearchRadis) 내에서 랜덤한 내비게이션 가능 위치를 찾습니다.
				if(NavSys->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadis, Loc))
				{
					// 블랙보드에 선택된 키에 해당하는 값을 이 랜덤한 위치로 설정합니다.
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);

					// 태스크를 성공으로 끝냅니다.
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
		else
		{
			// SearchRandom이 false일 경우, 플레이어의 정확한 위치를 블랙보드에 저장합니다.
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
		}
	}

	// 태스크가 실패한 경우를 반환
	return EBTNodeResult::Failed;
}
