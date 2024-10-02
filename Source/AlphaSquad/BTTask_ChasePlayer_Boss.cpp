
#include "BTTask_ChasePlayer_Boss.h"

#include "Boss_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


UBTTask_ChasePlayer_Boss::UBTTask_ChasePlayer_Boss(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Chase Player");
	
}


// 플레이어를 추적하는 태스크 실행 함수
EBTNodeResult::Type UBTTask_ChasePlayer_Boss::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// AIController를 가져옵니다. 캐스팅하여 Boss_AIController로 변환
	if(auto* const cont = Cast<ABoss_AIController>(OwnerComp.GetAIOwner()))
	{
		// 블랙보드에서 현재 선택된 키(플레이어 위치)를 벡터 값으로 가져옵니다.
		auto const PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

		
		
		// 플레이어의 위치로 AI를 이동시킵니다.
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, PlayerLocation);
		
		
		// 작업을 성공으로 마칩니다.
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;		

	}
	
	// 만약 AIController를 가져오지 못하면 실패로 처리합니다.
	return EBTNodeResult::Failed;	
}

