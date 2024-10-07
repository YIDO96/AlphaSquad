// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW_ThrowGrenadeNotifyState.h"
#include "Kismet/GameplayStatics.h"
#include "CSW_ActorManager.h"

void UCSW_ThrowGrenadeNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    if (MeshComp && MeshComp->GetWorld())
    {
        UE_LOG(LogTemp, Warning, TEXT("NotifyBegin"));

        // 월드에서 첫 번째 ACSW_ActorManager 인스턴스를 가져옵니다.
        ACSW_ActorManager* ActorManager = Cast<ACSW_ActorManager>(
            UGameplayStatics::GetActorOfClass(MeshComp->GetWorld(), ACSW_ActorManager::StaticClass()));

        if (ActorManager && ActorManager->grenadeActor)
        {
            GrenadeClass = ActorManager->grenadeActor;

            MeshComp->GetWorld()->SpawnActor<AActor>
                (
                    GrenadeClass,
                    MeshComp->GetSocketLocation(TEXT("hand_rSocket")),
                    MeshComp->GetSocketRotation(TEXT("hand_rSocket"))
                );

        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No valid Actor Manager or grenade actor found."));
        }
    }
}

void UCSW_ThrowGrenadeNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
}

void UCSW_ThrowGrenadeNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
}
