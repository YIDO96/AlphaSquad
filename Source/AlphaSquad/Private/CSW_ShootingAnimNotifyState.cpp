// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW_ShootingAnimNotifyState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystem.h"

void UCSW_ShootingAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    if (MeshComp && MeshComp->GetWorld())
    {
        UE_LOG(LogTemp, Warning, TEXT("NotifyBegin"));

        // 월드에서 첫 번째 ACSW_ActorManager 인스턴스를 가져옵니다.
        ACSW_ActorManager* ActorManager = Cast<ACSW_ActorManager>(
            UGameplayStatics::GetActorOfClass(MeshComp->GetWorld(), ACSW_ActorManager::StaticClass()));

        if (ActorManager && ActorManager->bulletActor)
        {
            BulletClass = ActorManager->bulletActor;
            auto* m_bulletParticle = ActorManager->ammunition_effect;

            UE_LOG(LogTemp, Warning, TEXT("bullet Actor Class retrieved successfully."));
            
            USkeletalMeshComponent* WeaponMeshComp = MeshComp->GetOwner()->FindComponentByClass<USkeletalMeshComponent>();

            if (WeaponMeshComp)
            {
				MeshComp->GetWorld()->SpawnActor<AActor>
				(
					BulletClass,
					WeaponMeshComp->GetChildComponent(0)->GetSocketLocation(TEXT("FirePosition")),
					WeaponMeshComp->GetChildComponent(0)->GetSocketRotation(TEXT("FirePosition"))
				);

                UGameplayStatics::SpawnEmitterAtLocation
                (
                    MeshComp->GetWorld(),
                    m_bulletParticle,
                    WeaponMeshComp->GetChildComponent(0)->GetSocketLocation(TEXT("FirePosition")),
                    WeaponMeshComp->GetChildComponent(0)->GetSocketRotation(TEXT("FirePosition"))
                );
            }

        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No valid Actor Manager or grenade actor found."));
        }
    }
}

void UCSW_ShootingAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
}

void UCSW_ShootingAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	UE_LOG(LogTemp, Warning, TEXT("NotifyEnd"));
}
