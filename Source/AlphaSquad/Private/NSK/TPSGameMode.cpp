// Fill out your copyright notice in the Description page of Project Settings.


#include "NSK/TPSGameMode.h"
#include "LSH/W_MainStartUI.h"
#include "Kismet/GameplayStatics.h"

void ATPSGameMode::BeginPlay()
{
	Super::BeginPlay();
	// Create Widget
	//if (W_MainStartUIClass)
	//{
	//	UW_MainStartUI* MainStartUI = Cast<UW_MainStartUI>(CreateWidget(GetWorld(), W_MainStartUIClass));
	//	if (MainStartUI)
	//	{
	//		MainStartUI->AddToViewport();
	//	}
	//}

	//APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	//
	//
	//playerController->SetViewTargetWithBlend(this, 0);
	//playerController->SetInputMode(FInputModeUIOnly());
	//playerController->SetShowMouseCursor(true);
}

void ATPSGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}
