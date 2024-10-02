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
	// DataTable�� ��ȿ���� Ȯ��
	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid ItemDataTable!"));
		// ItemDataTable�� ��ȿ���� �ʴٸ� �⺻�� ��ȯ
		return FItemData();
	}

	FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemRowName, TEXT("Item Lookup"));
	if (ItemData)
	{
		return *ItemData;
	}
	else
	{
		// ItemData�� ��ȿ���� �ʴٸ� �⺻�� ��ȯ
		UE_LOG(LogTemp, Error, TEXT("Item with RowName %s not found in DataTable!"), *ItemRowName.ToString());
		return FItemData();
	}
}

void UInventoryComponent::AddItemToInventory(FName ItemRowName, int32 Quantity)
{
	if (IsInventoryFull())
	{
		// �α� ����� �Լ� return
		UE_LOG(LogTemp, Warning, TEXT("Inventory is full!"));
		return;
	}

	if (Inventory.Contains(ItemRowName))
	{
		FInventoryItem& ExistingItem = Inventory[ItemRowName];
		if (ExistingItem.ItemData.MaxStackCount > ExistingItem.Quantity)
		{
			ExistingItem.Quantity += Quantity;
		}
	}
	else
	{
		if (ItemDataTable)
		{
			FItemData ItemData = LoadItemFromDataTable(ItemRowName);
			FInventoryItem NewItem;
			NewItem.ItemData = ItemData;

			if (ItemData.MaxStackCount == 1)
			{
				NewItem.Quantity = 1;
			}
			else if (ItemData.MaxStackCount > Quantity)
			{
				NewItem.Quantity = Quantity;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Item stack count is less than quantity!"));
				return;
			}

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
		// �κ��丮�� �ش� �������� ������ �α� ���
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

