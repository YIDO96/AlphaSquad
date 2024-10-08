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
	// �κ��丮�� ���� á�ٸ�
	if (IsInventoryFull())
	{
		// �α� ����� �Լ� return
		UE_LOG(LogTemp, Warning, TEXT("Inventory is full!"));
		return;
	}


	// �ش� Name�� �������� �κ��丮�� �����Ѵٸ�
	if (Inventory.Contains(ItemRowName))
	{
	    // �ش� �������� ������ ������
		FInventoryItem& ExistingItem = Inventory[ItemRowName];
		// �ش� �������� �ִ� ���� ���� ���� �������� ũ�ٸ�
		if (ExistingItem.ItemData.MaxStackCount > ExistingItem.Quantity)
		{
			// ���� ������ �߰��Ϸ��� ������ ����
			ExistingItem.Quantity += Quantity;
		}
	}
	// �ش� Name�� �������� �κ��丮�� �������� �ʴ´ٸ�
	else
	{
		// ItemDataTable�� ��ȿ�ϴٸ�
		if (ItemDataTable)
		{
			// �ش� Name�� ������ ������ ������
			FItemData ItemData = LoadItemFromDataTable(ItemRowName);
			FInventoryItem NewItem;
			NewItem.ItemData = ItemData;

			// �ش� �������� �ִ� ���� ���� 1�̶�� ������ 1�� ����
			if (ItemData.MaxStackCount == 1)
			{
				NewItem.Quantity = 1;
			}
			// �ش� �������� �ִ� ���� ���� �߰��Ϸ��� �������� ũ�ٸ� ������ �߰��Ϸ��� �������� ����
			else if (ItemData.MaxStackCount > Quantity)
			{
				NewItem.Quantity = Quantity;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Item stack count is less than quantity!"));
				return;
			}

			// �κ��丮�� �ش� ������ �߰�
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

