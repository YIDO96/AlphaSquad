// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW_Enemy.h"

#include "CSW_State_Attack1.h"
#include "CSW_State_Attack2.h"
#include "CSW_State_Attack3.h"
#include "CSW_State_Idle.h"


// Sets default values
ACSW_Enemy::ACSW_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACSW_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
	ParsedPatterns = ParsePatternString(Pattern);

	for (const FString str : ParsedPatterns)
	{
		UE_LOG(LogTemp, Warning, TEXT("Parsing Str : %s"), *str);
	}

	//State Mapping setting
	StateMap.Add("A1", UCSW_State_Attack1::StaticClass());
	StateMap.Add("A2", UCSW_State_Attack2::StaticClass());
	StateMap.Add("A3", UCSW_State_Attack3::StaticClass());
	StateMap.Add("Idle", UCSW_State_Idle::StaticClass());
}

// Called every frame
void ACSW_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACSW_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBehaviorTree* ACSW_Enemy::GetBeHaviorTree() const
{
	return Tree;
}

void ACSW_Enemy::CombateStateExcute()
{
	CurrentState = NewObject<UCSW_State_Idle>(this);

	for (const FString& pattern : ParsedPatterns)
	{
		if (CurrentState)
		{
			CurrentState->ExitState();
		}

		if (StateMap.Contains(pattern))
		{
			UE_LOG(LogTemp, Warning, TEXT("current State[%s]"), *pattern);

			


			if (pattern == "A1")
			{
				CurrentState = NewObject<UCSW_State_Attack1>(this);

				CurrentState->EnterState();
				CurrentState->UpdateState();
			}
			else if (pattern == "A2")
			{
				CurrentState = NewObject<UCSW_State_Attack2>(this);

				CurrentState->EnterState();
				CurrentState->UpdateState();
			}
			else if (pattern == "A3")
			{
				CurrentState = NewObject<UCSW_State_Attack3>(this);

				CurrentState->EnterState();
				CurrentState->UpdateState();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to create state for: %s"), *pattern);
			}

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unknown state: %s"), *Pattern);
		}
	}

	//CurrentState->ExitState();
}


TArray<FString> ACSW_Enemy::ParsePatternString(const FString& PatternString)
{
	TArray<FString> m_ParsedPatterns;
	// ,로 구분해서 파싱
	PatternString.ParseIntoArray(m_ParsedPatterns, TEXT(","), true);
	return m_ParsedPatterns;
}

