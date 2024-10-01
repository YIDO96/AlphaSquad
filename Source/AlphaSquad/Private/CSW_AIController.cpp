// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW_AIController.h"
#include "CSW_Enemy.h"
#include "Perception\AIPerceptionComponent.h"
#include "Perception\AISenseConfig_Sight.h"
#include "TPSPlayer.h"

ACSW_AIController::ACSW_AIController(FObjectInitializer const& objectInitializer)
{
	SetupPerceptionSystem();
}

void ACSW_AIController::OnPossess(APawn* Inpawn)
{
	Super::OnPossess(Inpawn);



	if (ACSW_Enemy* const enemy = Cast<ACSW_Enemy>(Inpawn))
	{
		if (UBehaviorTree* const tree = enemy->GetBeHaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(tree);
		}
	}

}

void ACSW_AIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Comp")));
		SightConfig->SightRadius = 500.0f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.0f;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;
		SightConfig->SetMaxAge(5.0f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.0f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;


		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ACSW_AIController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense((*SightConfig));
	}
}

void ACSW_AIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("check dis "));


	if (auto* const ch = Cast<ATPSPlayer>(Actor))
	{
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());

		//float DistanceToPlayer = FVector::Dist(ch->GetActorLocation(), GetPawn()->GetActorLocation());
		//float CombatRange = 200.0f;

		//UE_LOG(LogTemp, Warning, TEXT("Distance %f"), DistanceToPlayer);

		//if (DistanceToPlayer <= CombatRange)
		//{
		//	GetBlackboardComponent()->SetValueAsBool("CombatMode", true);
		//}
		//else
		//{
		//	GetBlackboardComponent()->SetValueAsBool("CombatMode", false);
		//}

	}
}
