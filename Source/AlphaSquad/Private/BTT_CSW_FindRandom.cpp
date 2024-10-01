// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_CSW_FindRandom.h"

#include "NavigationSystem.h"
#include "CSW_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_CSW_FindRandom::UBTT_CSW_FindRandom(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "CSW_FindRandom Location In NavMesh";
}

EBTNodeResult::Type UBTT_CSW_FindRandom::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get AI Controller and its enemy
	if (auto* const cont = Cast<ACSW_AIController>(OwnerComp.GetAIOwner()))
	{

		if (auto* const enemy = cont->GetPawn())
		{
			// obtain enemy location to use as an origin
			auto const Origin = enemy->GetActorLocation();

			// get the navigation system and generate a random locaiton
			if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation Loc;
				if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, Loc))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
				}

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}
