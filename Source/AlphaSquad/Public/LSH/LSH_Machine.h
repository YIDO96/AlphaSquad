// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LSH_InteractableInterface.h"
#include "LSH_Machine.generated.h"

UCLASS()
class ALPHASQUAD_API ALSH_Machine : public AActor, public ILSH_InteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALSH_Machine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void DisplayInteractionWidget() override;
	virtual	void HideInteractionWidget() override;
	virtual void Interact() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TArray<FName> ItemOnSale;


	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Shop")
	int32 NumberType;

	UFUNCTION(BlueprintCallable)
	FText GetQuestText();


protected:
	UPROPERTY(EditAnywhere, Category = "Interaction")
	TSubclassOf<UUserWidget> InteractionWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	TSubclassOf<UUserWidget> ItemShopWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	UUserWidget* ItemShopWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	class UWidgetComponent* InteractionWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	class UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	class UStaticMeshComponent* Mesh;


};
