// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/W_LSH_QuickSlotandInGameUI.h"

#include "../AlphaSquadCharacter.h"
#include "LSH/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"


void UW_LSH_QuickSlotandInGameUI::InitFunction()
{
	AAlphaSquadCharacter* testplayer = Cast<AAlphaSquadCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));

	if (testplayer)
	{
		TestPlayerCharacter = testplayer;
	}
}

void UW_LSH_QuickSlotandInGameUI::Tick(FGeometry MyGeometry, float InDeltaTime)
{
	float percent = TestPlayerCharacter->Health / TestPlayerCharacter->MaxHealth;
	PB_Hp->SetPercent(percent);

	if (percent < 0.3f)
	{
		PB_Hp->SetFillColorAndOpacity(FColor(1,0,0,0.5));
	}
	else
	{
		PB_Hp->SetFillColorAndOpacity(FColor(0.5, 0.5, 0.5, 0.5));
	}
}

FText UW_LSH_QuickSlotandInGameUI::Get_TBBulletCount_Text_C()
{
	if (TestPlayerCharacter->InventoryComponent->Inventory.Contains(WeaponBulletName))
	{
		int32 quantity = TestPlayerCharacter->InventoryComponent->GetItemQuantity(WeaponBulletName);
		int32 maxCount = TestPlayerCharacter->InventoryComponent->GetItemMaxStackCount(WeaponBulletName);

		FString text = FString::Printf(TEXT("%d / %d"), quantity, maxCount);

		return FText::FromString(text);
	}

	else return FText();
}

 UTexture2D* UW_LSH_QuickSlotandInGameUI::Get_ImgQuickNum3_Brush_C()
{
	if (TestPlayerCharacter->InventoryComponent->Inventory.Contains(QuickNum3Name))
	{
		Img_QuickNum3->SetVisibility(ESlateVisibility::Visible);
		UTexture2D* thumbnail = TestPlayerCharacter->InventoryComponent->GetItemThumbnail(QuickNum3Name);

		Img_QuickNum3->SetBrushFromTexture(thumbnail);

		return thumbnail;
	}
	else
	{
		Img_QuickNum3->SetVisibility(ESlateVisibility::Hidden);
	}

	return nullptr;
}

 UTexture2D* UW_LSH_QuickSlotandInGameUI::Get_ImgQuickNum4_Brush_C()
{ 
	if (TestPlayerCharacter->InventoryComponent->Inventory.Contains(QuickNum4Name))
	{
		Img_QuickNum4->SetVisibility(ESlateVisibility::Visible);
		UTexture2D* thumbnail = TestPlayerCharacter->InventoryComponent->GetItemThumbnail(QuickNum4Name);

		Img_QuickNum4->SetBrushFromTexture(thumbnail);

		return thumbnail;
	}
	else
	{
		Img_QuickNum4->SetVisibility(ESlateVisibility::Hidden);
	}
	return nullptr;
}

FText UW_LSH_QuickSlotandInGameUI::Get_TBQuickNum3_Text_C()
{
	if (TestPlayerCharacter->InventoryComponent->Inventory.Contains(QuickNum3Name))
	{
		int32 quantity = TestPlayerCharacter->InventoryComponent->GetItemQuantity(QuickNum3Name);

		return FText::AsNumber(quantity);
	}

	else return FText();
}

FText UW_LSH_QuickSlotandInGameUI::Get_TBQuickNum4_Text_C()
{
	if (TestPlayerCharacter->InventoryComponent->Inventory.Contains(QuickNum4Name))
	{
		int32 quantity = TestPlayerCharacter->InventoryComponent->GetItemQuantity(QuickNum4Name);

		return FText::AsNumber(quantity);
	}

	else return FText();
}
