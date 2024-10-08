// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LSH_InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULSH_InteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALPHASQUAD_API ILSH_InteractableInterface
{
	GENERATED_BODY()
public:
	virtual void DisplayInteractionWidget() = 0;
	virtual void HideInteractionWidget() = 0;
	virtual void Interact() = 0;
};
