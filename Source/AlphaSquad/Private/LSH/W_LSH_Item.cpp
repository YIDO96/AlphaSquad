// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/W_LSH_Item.h"
#include "LSH/LSH_ItemData.h"
#include "Engine/DataTable.h"
#include "../AlphaSquadCharacter.h"
#include "Kismet/GameplayStatics.h"

void UW_LSH_Item::BtnClickFunction()
{
	AAlphaSquadCharacter* playerCharacter = Cast<AAlphaSquadCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (playerCharacter->bIsBuyItem(ItemID))
	{
		playerCharacter->UpdateMoney(-ItemData.ItemPrice, ItemID);
	}
}
