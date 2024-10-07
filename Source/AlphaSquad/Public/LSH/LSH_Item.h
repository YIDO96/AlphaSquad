// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LSH_ItemData.h"
#include "LSH_Item.generated.h"

/**
 * 
 */
UCLASS()
class ALPHASQUAD_API ULSH_Item : public UObject
{
	GENERATED_BODY()
	
public:
	ULSH_Item();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemData ItemData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UStaticMeshComponent* ItemMesh;


};
