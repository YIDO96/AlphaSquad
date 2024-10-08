// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW_ActorManager.h"

// Sets default values
ACSW_ActorManager::ACSW_ActorManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}



// Called when the game starts or when spawned
void ACSW_ActorManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACSW_ActorManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

