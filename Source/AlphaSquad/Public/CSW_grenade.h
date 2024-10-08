// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSW_grenade.generated.h"

UCLASS()
class ALPHASQUAD_API ACSW_grenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACSW_grenade();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FTimerHandle ExplosionTimerHandle;

	void ExplostionGrenade();


public:
	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UProjectileMovementComponent* movementComp;
	UPROPERTY(VisibleAnywhere, Category = Collision)
	class USphereComponent* collisionComp;
	UPROPERTY(VisibleAnywhere, Category = BodyMesh)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	class USoundBase* exploreSound;
	// exploreSound

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float ExplosionDeley = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Explosion")
	UParticleSystem* grenade_effect;

};
