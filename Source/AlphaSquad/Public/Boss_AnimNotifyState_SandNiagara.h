// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Boss_AnimNotifyState_SandNiagara.generated.h"

/**
 * 
 */
UCLASS()
class ALPHASQUAD_API UBoss_AnimNotifyState_SandNiagara : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	// Notify가 발생했을 때 호출되는 함수
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	
	// 생성할 Actor 클래스 지정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	FRotator Rotation;
	
	
};
