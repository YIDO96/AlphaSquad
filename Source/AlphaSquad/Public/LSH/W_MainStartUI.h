// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_MainStartUI.generated.h"

/**
 * 
 */
UCLASS()
class ALPHASQUAD_API UW_MainStartUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UBorder* B_MainStartUI;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Start;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Exit;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_Start;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_Exit;
};
