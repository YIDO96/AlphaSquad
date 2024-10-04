// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_LSH_QuickSlotandInGameUI.generated.h"

/**
 * 
 */
UCLASS()
class ALPHASQUAD_API UW_LSH_QuickSlotandInGameUI : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	class UBorder* B_QuickNum3;
	UPROPERTY(meta = (BindWidget))
	class UBorder* B_QuickNum4;
	UPROPERTY(meta = (BindWidget))
	class UBorder* B_QuickNum5;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_QuickNum3;
	UPROPERTY(meta = (BindWidget))
	class UImage* Img_QuickNum4;
	UPROPERTY(meta = (BindWidget))
	class UImage* Img_QuickNum5;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_QuickNum3;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_QuickNum4;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_BulletCount;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_Hp;

public:
	class AAlphaSquadCharacter* TestPlayerCharacter;
	FName WeaponBulletName;
	FName QuickNum3Name = TEXT("Generade");
	FName QuickNum4Name = TEXT("Band");
	FName QuickNum5Name = TEXT("None");


public:
	UFUNCTION(BlueprintCallable)
	void InitFunction();

	void Tick(FGeometry MyGeometry, float InDeltaTime);

	UFUNCTION(BlueprintCallable)
	FText Get_TBBulletCount_Text_C();

	UFUNCTION(BlueprintCallable)
	UTexture2D* Get_ImgQuickNum3_Brush_C();

	UFUNCTION(BlueprintCallable)
	UTexture2D* Get_ImgQuickNum4_Brush_C();

	UFUNCTION(BlueprintCallable)
	FText Get_TBQuickNum3_Text_C();

	UFUNCTION(BlueprintCallable)
	FText Get_TBQuickNum4_Text_C();
};
