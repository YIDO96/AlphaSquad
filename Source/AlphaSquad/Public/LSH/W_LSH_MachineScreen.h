// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_LSH_MachineScreen.generated.h"

/**
 * 
 */
UCLASS()
class ALPHASQUAD_API UW_LSH_MachineScreen : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CP_Shop;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* UGP_Grid;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_PlayerMoney;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CP_Quest;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Quest;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Shop;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Exit;
};
