// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_CSW_FindPlayer.h"
#include "NavigationSystem.h"
#include "BehaviorTree\BlackboardComponent.h"
#include "GameFramework\Character.h"
#include "Kismet\GameplayStatics.h"


UBTT_CSW_FindPlayer::UBTT_CSW_FindPlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("CSW_Find Player Location");
}

EBTNodeResult::Type UBTT_CSW_FindPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get player character
	if (auto* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{

		// get player location to use as an origin
		auto const PlayerLocation = Player->GetActorLocation();
		if (SearchRandom)
		{
			FNavLocation Loc;

			// get the navigation system and generate a random location near the player
			if (auto* const Navsys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				if (Navsys->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, Loc))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

					return EBTNodeResult::Succeeded;
				}
			}
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
