// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LSH_ItemData.h"
#include "W_LSH_Item.generated.h"

/**
 * 
 */
UCLASS()
class ALPHASQUAD_API UW_LSH_Item : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USizeBox* SB_Item; //Width Override : 200 Height Override : 200

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TB_ItemName; //ItemName으로 바인딩

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Btn_Purchase; // OnClicked

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Img_Thumbnail; // ItemThumbnail로 바인딩

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TB_Price; // ItemPrice로 바인딩



public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemData ItemData;


	UFUNCTION(BlueprintCallable)
	void BtnClickFunction();


	// Get함수
	UFUNCTION(BlueprintCallable, BlueprintPure)
	USizeBox* GetSB_Item() { return SB_Item; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTextBlock* GetTB_ItemName() { return TB_ItemName; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UButton* GetBtn_Purchase() { return Btn_Purchase; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UImage* GetImg_Thumbnail() { return Img_Thumbnail; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTextBlock* GetTB_Price() { return TB_Price; }
};
