// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW_Bullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ACSW_Bullet::ACSW_Bullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = collisionComp;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	MeshComp->SetupAttachment(collisionComp);

	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	movementComp->SetUpdatedComponent(collisionComp);
	movementComp->InitialSpeed = bullet_Speed;
	movementComp->MaxSpeed = bullet_Speed;

}

// Called when the game starts or when spawned
void ACSW_Bullet::BeginPlay()
{
	Super::BeginPlay();

	
	FTimerHandle deathTimer;
	GetWorld()->GetTimerManager().SetTimer
	(
		deathTimer, 
		FTimerDelegate::CreateLambda([this]()->void {Destroy(); }),
		2.0f,
		false
	);
	

}

// Called every frame
void ACSW_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

