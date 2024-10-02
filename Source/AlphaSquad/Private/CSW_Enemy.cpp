// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW_Enemy.h"

// Sets default values
ACSW_Enemy::ACSW_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACSW_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACSW_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACSW_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBehaviorTree* ACSW_Enemy::GetBeHaviorTree() const
{
	return Tree;
}

