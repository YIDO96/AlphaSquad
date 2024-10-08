// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FItemData UInventoryComponent::LoadItemFromDataTable(FName ItemRowName)
{
	// DataTable이 유효한지 확인
	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid ItemDataTable!"));
		// ItemDataTable이 유효하지 않다면 기본값 반환
		return FItemData();
	}

	FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemRowName, TEXT("Item Lookup"));
	if (ItemData)
	{
		return *ItemData;
	}
	else
	{
		// ItemData가 유효하지 않다면 기본값 반환
		UE_LOG(LogTemp, Error, TEXT("Item with RowName %s not found in DataTable!"), *ItemRowName.ToString());
		return FItemData();
	}
}

void UInventoryComponent::AddItemToInventory(FName ItemRowName, int32 Quantity)
{
	// 인벤토리가 가득 찼다면
	if (IsInventoryFull())
	{
		// 로그 출력후 함수 return
		UE_LOG(LogTemp, Warning, TEXT("Inventory is full!"));
		return;
	}


	// 해당 Name의 아이템이 인벤토리에 존재한다면
	if (Inventory.Contains(ItemRowName))
	{
	    // 해당 아이템의 정보를 가져옴
		FInventoryItem& ExistingItem = Inventory[ItemRowName];
		// 해당 아이템의 최대 스택 수가 현재 수량보다 크다면
		if (ExistingItem.ItemData.MaxStackCount > ExistingItem.Quantity)
		{
			// 현재 수량에 추가하려는 수량을 더함
			ExistingItem.Quantity += Quantity;
		}
	}
	// 해당 Name의 아이템이 인벤토리에 존재하지 않는다면
	else
	{
		// ItemDataTable이 유효하다면
		if (ItemDataTable)
		{
			// 해당 Name의 아이템 정보를 가져옴
			FItemData ItemData = LoadItemFromDataTable(ItemRowName);
			FInventoryItem NewItem;
			NewItem.ItemData = ItemData;

			// 해당 아이템의 최대 스택 수가 1이라면 수량을 1로 설정
			if (ItemData.MaxStackCount == 1)
			{
				NewItem.Quantity = 1;
			}
			// 해당 아이템의 최대 스택 수가 추가하려는 수량보다 크다면 수량을 추가하려는 수량으로 설정
			else if (ItemData.MaxStackCount > Quantity)
			{
				NewItem.Quantity = Quantity;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Item stack count is less than quantity!"));
				return;
			}

			// 인벤토리에 해당 아이템 추가
			Inventory.Add(ItemRowName, NewItem);

		}
	}
}

void UInventoryComponent::RemoveItemFromInventory(FName ItemRowName, int32 Quantity)
{
	if (Inventory.Contains(ItemRowName))
	{
		FInventoryItem& ItemData = Inventory[ItemRowName];
		ItemData.Quantity -= Quantity;

		if (ItemData.Quantity <= 0)
		{
			Inventory.Remove(ItemRowName);
			UE_LOG(LogTemp, Warning, TEXT("Removed item with ID %d from inventory"), *ItemRowName.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Decreased quantity of item with ID %s to %d"), *ItemRowName.ToString(), ItemData.Quantity);
		}
	}
	else
	{
		// 인벤토리에 해당 아이템이 없으면 로그 출력
		UE_LOG(LogTemp, Error, TEXT("Item with ID %s not found in inventory!"), *ItemRowName.ToString());
	}
}

bool UInventoryComponent::IsInventoryFull() const
{
	return Inventory.Num() >= MaxInventorySize;
}

void UInventoryComponent::InventoryOpen()
{
}

void UInventoryComponent::InventoryClose()
{
}

