// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW_Enemy.h"

#include "CSW_State_Attack1.h"
#include "CSW_State_Attack2.h"
#include "CSW_State_Attack3.h"
#include "CSW_State_bAttack1.h"
#include "CSW_State_bAttack2.h"
#include "CSW_State_bAttack3.h"
#include "CSW_State_bAttack4.h"
#include "CSW_State_bAttack5.h"
#include "CSW_State_Idle.h"
#include "TimerManager.h"
#include "NSK/TPSPlayer.h"
#include "Kismet\GameplayStatics.h"

#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"

// Sets default values
ACSW_Enemy::ACSW_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBarComponent->SetupAttachment(RootComponent);
	HealthBarComponent->SetDrawSize(FVector2D(200.0f, 50.0f));
	HealthBarComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));
	HealthBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
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

	CurrentHealth = Hp;

	// hp 바
	UUserWidget* Widget = HealthBarComponent->GetWidget();
	if (Widget)
	{
		UProgressBar* HealthBar = Cast<UProgressBar>(Widget->GetWidgetFromName(TEXT("HP_Bar")));
		if (HealthBar)
		{
			float healthpercentage = CurrentHealth / Hp;
			HealthBar->SetPercent(healthpercentage);
			HealthBar->SetVisibility(ESlateVisibility::Hidden);
		}
	}
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
	if (!IsValid(this)) // 객체 유효성 검사
	{
		return; // 유효하지 않으면 리턴
	}

	int32 PatternIndex = 0;

	// 처음 실행하는 함수
	ExcutePatternWithDelay(PatternIndex);

}

void ACSW_Enemy::ExcutePatternWithDelay(int32 PatternIndex)
{
	// Timer Initial
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	if (ParsedPatterns.IsValidIndex(PatternIndex))
	{
		FString& pattern = ParsedPatterns[PatternIndex];

		if (CurrentState)
		{
			CurrentState->ExitState();
		}

		if (pattern == "A1")
		{
			CurrentState = NewObject<UCSW_State_Attack1>(this);
			MontageDuration = Pattern_Montage_A1->GetPlayLength()+0.5f;
		}
		else if (pattern == "A2")
		{
			CurrentState = NewObject<UCSW_State_Attack2>(this);
			MontageDuration = Pattern_Montage_A2->GetPlayLength()+0.5f;
		}
		else if (pattern == "A3")
		{
			CurrentState = NewObject<UCSW_State_Attack3>(this);
			MontageDuration = Pattern_Montage_A3->GetPlayLength()+0.5f;
		}
		else if (pattern == "B1")
		{
			CurrentState = NewObject<UCSW_State_bAttack1>(this);
			MontageDuration = Pattern_Montage_B1->GetPlayLength() + 0.5f;
		}
		else if (pattern == "B2")
		{
			CurrentState = NewObject<UCSW_State_bAttack2>(this);
			MontageDuration = Pattern_Montage_B2->GetPlayLength() + 0.5f;
		}
		else if (pattern == "B3")
		{
			CurrentState = NewObject<UCSW_State_bAttack3>(this);
			MontageDuration = Pattern_Montage_B3->GetPlayLength() + 0.5f;
		}
		else if (pattern == "B4")
		{
			CurrentState = NewObject<UCSW_State_bAttack4>(this);
			MontageDuration = Pattern_Montage_B4->GetPlayLength() + 0.5f;
		}
		else if (pattern == "B5")
		{
			CurrentState = NewObject<UCSW_State_bAttack5>(this);
			MontageDuration = Pattern_Montage_B5->GetPlayLength() + 0.5f;
		}

		// 새로운 상태 진입
		if (CurrentState)
		{
			CurrentState->EnterState();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create state for: %s"), *pattern);
		}

		
		GetWorld()->GetTimerManager().SetTimer
		(
			TimerHandle,
			this,
			&ACSW_Enemy::OnPatternExcutionComplate,
			MontageDuration,
			false
		);
		
	}
}

void ACSW_Enemy::OnPatternExcutionComplate()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	if (!IsValid(this)) // 객체 유효성 검사
	{
		return; // 유효하지 않으면 리턴
	}


	// 다음 패턴 실행
	int32 NextPatternIndex = ++CurrentPatternIndex; // 인덱스 증가
	
	if (ParsedPatterns.IsValidIndex(NextPatternIndex))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

		ExcutePatternWithDelay(NextPatternIndex);
	}
	else
	{
		// Timer Initial
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		// 모든 패턴이 끝난 후 처리
		// 1. 플레이어와의 거리가 짧을 경우 계속 패턴 유지
		
		/*float DistanceToPlayer = FVector::Dist(GetActorLocation(), player)*/
		if (auto* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		{
			float Dis = FVector::Dist(GetActorLocation(), player->GetActorLocation());
			if (Dis < attackRang)
			{
				CurrentPatternIndex = 0;
				
				CombateStateExcute();
			}
			else
			{
				GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

				UE_LOG(LogTemp, Warning, TEXT("Player is out of Range."));
			}
		}

		// 2. 아닌 경우 그냥 다시 순찰 모드
		// 추가 될 예정
		UE_LOG(LogTemp, Warning, TEXT("All patterns executed."));
	}
}


void ACSW_Enemy::OnTakeDamage(float Damage)
{
	CurrentHealth -= Damage;

	UUserWidget* Widget = HealthBarComponent->GetWidget();
	if (Widget)
	{
		UProgressBar* HealthBar = Cast<UProgressBar>(Widget->GetWidgetFromName(TEXT("HP_Bar")));
		if (HealthBar)
		{
			float healthpercentage = CurrentHealth / Hp;
			HealthBar->SetPercent(healthpercentage);
			HealthBar->SetVisibility(ESlateVisibility::Visible);
		}
	}


	if (CurrentHealth <= 0)
	{
		auto* playerClass = Cast<ATPSPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass()));

		if (playerClass)
		{
			playerClass->killCont++;
			playerClass->CurrentMoney += EnemyMoney;
			Destroy();
		}
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, Hp);
}

TArray<FString> ACSW_Enemy::ParsePatternString(const FString& PatternString)
{
	TArray<FString> m_ParsedPatterns;
	// ,�� �����ؼ� �Ľ�
	PatternString.ParseIntoArray(m_ParsedPatterns, TEXT(","), true);
	return m_ParsedPatterns;
}



