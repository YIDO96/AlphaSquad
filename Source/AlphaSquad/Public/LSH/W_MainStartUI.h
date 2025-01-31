// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_MainStartUI.generated.h"

/**
 * 
 */
UCLASS()
class ALPHASQUAD_API UW_MainStartUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UBorder* B_MainStartUI;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	//UAudioComponent* AudioComponent;

protected:
	virtual void NativeConstruct() override;
};
