// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW_grenade.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ACSW_grenade::ACSW_grenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = collisionComp;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	MeshComp->SetupAttachment(collisionComp);
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));

	movementComp->ProjectileGravityScale = 1.0f;
	movementComp->InitialSpeed = 1000.0f;
	movementComp->MaxSpeed = 1000.0f;
	movementComp->bRotationFollowsVelocity = false;
	movementComp->bShouldBounce = false;

}

// Called when the game starts or when spawned
void ACSW_grenade::BeginPlay()
{
	Super::BeginPlay();
	

	GetWorld()->GetTimerManager().SetTimer
	(
		ExplosionTimerHandle,
		this,
		&ACSW_grenade::ExplostionGrenade, 
		ExplosionDeley,
		false);
}

// Called every frame
void ACSW_grenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACSW_grenade::ExplostionGrenade()
{
	UGameplayStatics::PlaySound2D(GetWorld(), exploreSound);

	if (grenade_effect)
	{
		UGameplayStatics::SpawnEmitterAtLocation
		(
			GetWorld(),
			grenade_effect,
			GetActorLocation(),
			GetActorRotation()
		);
	}

	Destroy();
}

