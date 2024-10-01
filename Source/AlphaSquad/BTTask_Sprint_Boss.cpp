// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Sprint_Boss.h"
#include "Boss_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_Sprint_Boss::UBTTask_Sprint_Boss(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Chase Player with Sprint");
}

EBTNodeResult::Type UBTTask_Sprint_Boss::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// AIController를 가져옵니다. 캐스팅하여 Boss_AIController로 변환
	if(auto* const cont = Cast<ABoss_AIController>(OwnerComp.GetAIOwner()))
	{
		// 블랙보드에서 현재 선택된 키(플레이어 위치)를 벡터 값으로 가져옵니다.
		auto const PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
		
		// AI 캐릭터를 가져옵니다. Boss_AIController에서 제어하는 캐릭터입니다.
		ACharacter* const AIBoss = Cast<ACharacter>(cont->GetPawn());
		if(AIBoss)
		{
			// AI 캐릭터의 기본 이동 속도를 스프린트 속도로 설정합니다. (예: 600.f)
			AIBoss->GetCharacterMovement()->MaxWalkSpeed = 3000.f;

			// 플레이어의 위치로 AI를 이동시킵니다.
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, PlayerLocation);

				
			// 일정 시간이 지난 후, 다시 기본 속도로 되돌리기 위해서 타이머를 설정할 수 있습니다.
			FTimerHandle TimerHandle;
			AIBoss->GetWorldTimerManager().SetTimer(TimerHandle, [AIBoss]()
			{
				// AI 캐릭터의 이동 속도를 원래 속도로 복원합니다. (예: 300.f)
				AIBoss->GetCharacterMovement()->MaxWalkSpeed = 1000.f;
			}, 1.0f, false); // 1초 후에 원래 속도로 돌아옵니다.
		}
		
		// 작업을 성공으로 마칩니다.
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;		
	}
	
	// 만약 AIController를 가져오지 못하면 실패로 처리합니다.
	return EBTNodeResult::Failed;
}