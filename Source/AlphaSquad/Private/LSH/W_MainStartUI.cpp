// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/W_MainStartUI.h"

#include "Components/Button.h"

void UW_MainStartUI::NativeConstruct()
{
	Super::NativeConstruct();
	// Bind Delegates
	if (Btn_Start)
	{
		Btn_Start->OnClicked.AddDynamic(this, &UW_MainStartUI::OnStartButtonClicked);
	}
	if (Btn_Exit)
	{
		Btn_Exit->OnClicked.AddDynamic(this, &UW_MainStartUI::OnExitButtonClicked);
	}
}

void UW_MainStartUI::OnStartButtonClicked()
{
}

void UW_MainStartUI::OnExitButtonClicked()
{
}
