// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Boss.h"

// Sets default values
ANPC_Boss::ANPC_Boss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANPC_Boss::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPC_Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPC_Boss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBehaviorTree* ANPC_Boss::GetBehaviorTree()
{
		return Tree;
}

