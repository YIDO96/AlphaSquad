// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LSH_ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Consumable,		// �Ҹ� ������
	Equipment,		// ���� ������ ������
	Quest,			// ����Ʈ ������
	Material		// ��� ������
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	int32 ItemID;				// ������ ������ȣ (DataTable ����)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	EItemType ItemType;			// ������ Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	FText ItemName;				// ������ �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	FText ItemDescription;		// ǰ�� ����
    	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	UTexture2D* ItemThumbnail;	// ������ �̹���S

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	UStaticMesh* ItemMesh;		// ������ �޽�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	int32 ItemPrice;			// ������ ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	int32 MaxStackCount;		// ������ �ִ� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	int32 ItemWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	int32 ItemHeight;
};


class ALPHASQUAD_API LSH_ItemData
{
public:
	LSH_ItemData();
	~LSH_ItemData();
};
