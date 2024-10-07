// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LSH_ItemData.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemData ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Quantity;

	FInventoryItem() : ItemData(), Quantity(1) {}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALPHASQUAD_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	float Colunms;
	float Rows;
		


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap<FName, FInventoryItem> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UDataTable* ItemDataTable;

	// DataTable로부터 아이템을 로드
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FItemData LoadItemFromDataTable(FName ItemRowName);

	// 아이템을 인벤토리에 추가
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItemToInventory(FName ItemRowName, int32 Quantity);

	// 인벤토리에서 아이템 제거
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItemFromInventory(FName ItemRowName, int32 Quantity);

	// 인벤토리에 가득 찼는지 확인
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsInventoryFull() const;

	void InventoryOpen();
	void InventoryClose();

	// 인벤토리 최대 크기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxInventorySize;


	UPROPERTY(EditAnywhere, Category = "Interaction")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UPROPERTY()
	UUserWidget* InventoryWidget;

	/*UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(FItemData ItemData, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(FItemData ItemData, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItemAt(int32 Index, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UseItemAt(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UseItem(FItemData ItemData);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UseItemAtIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void DropItemAt(int32 Index, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void DropItem(FItemData ItemData, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void DropItemAtIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void DropAllItems();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ClearInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetItemQuantity(FItemData ItemData);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetItemIndex(FItemData ItemData);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FInventoryItem GetItem(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetInventorySize();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasItem(FItemData ItemData);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasItemAtIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasItemWithQuantity(FItemData ItemData, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasItemWithQuantityAtIndex(int32 Index, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsInventoryFull();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsInventoryEmpty();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsItemValid(FItemData ItemData);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsItemAtIndexValid(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsItemValidWithQuantity(FItemData ItemData, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsItemAtIndexValidWithQuantity(int32 Index, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void*/



public:
	UFUNCTION(BlueprintCallable)
	int32 GetItemQuantity(FName ItemRowName) {return Inventory[ItemRowName].Quantity; }
	UFUNCTION(BlueprintCallable)
	int32 GetItemMaxStackCount(FName ItemRowName) {return Inventory[ItemRowName].ItemData.MaxStackCount; }
	UFUNCTION(BlueprintCallable)
	UTexture2D* GetItemThumbnail(FName ItemRowName) { return Inventory[ItemRowName].ItemData.ItemThumbnail; }
};