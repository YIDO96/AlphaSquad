// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LSH_ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Consumable,		// 소모성 아이템
	Equipment,		// 장착 가능한 아이템
	Quest,			// 퀘스트 아이템
	Material		// 재료 아이템
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	int32 ItemID;				// 아이템 고유번호 (DataTable 참조)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	EItemType ItemType;			// 아이템 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	FText ItemName;				// 아이템 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	FText ItemDescription;		// 품목 설명
    	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	UTexture2D* ItemThumbnail;	// 아이템 이미지S

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	UStaticMesh* ItemMesh;		// 아이템 메쉬

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	int32 ItemPrice;			// 아이템 가격

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	int32 MaxStackCount;		// 아이템 최대 개수

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
