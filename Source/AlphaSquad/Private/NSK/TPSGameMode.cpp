// Fill out your copyright notice in the Description page of Project Settings.


#include "NSK/TPSGameMode.h"
#include "LSH/W_MainStartUI.h"


void ATPSGameMode::BeginPlay()
{
	Super::BeginPlay();
	// Create Widget
	if (W_MainStartUIClass)
	{
		UW_MainStartUI* MainStartUI = Cast<UW_MainStartUI>(CreateWidget(GetWorld(), W_MainStartUIClass));
		if (MainStartUI)
		{
			MainStartUI->AddToViewport();
		}
	}
}
