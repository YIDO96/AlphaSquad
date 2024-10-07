// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystem.h"
#include "CSW_ActorManager.generated.h"

UCLASS()
class ALPHASQUAD_API ACSW_ActorManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACSW_ActorManager();

	UPROPERTY(EditAnywhere,  Category = "Actors")
	TSubclassOf<AActor> bulletActor;
	UPROPERTY(EditAnywhere, Category = "Actors")
	TSubclassOf<AActor> grenadeActor;


	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* ammunition_effect;
	UPROPERTY(EditAnywhere,  Category = "Particles")
	UParticleSystem* grenade_effect;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
