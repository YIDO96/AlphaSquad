#include "Boss_AnimNotifyState_SandNiagara.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// 스폰한 액터를 저장할 변수
AActor* SpawnedActor = nullptr;

void UBoss_AnimNotifyState_SandNiagara::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	//UE_LOG(LogTemp, Warning, TEXT("Let's Combo Pang!"));
	
	if (MeshComp && MeshComp->GetWorld())
	{
		// 스폰할 위치 및 회전 설정
		FVector SpawnLocation = MeshComp->GetComponentLocation() + Location;  // 메시 컴포넌트의 현재 위치
		FRotator SpawnRotation = MeshComp->GetComponentRotation() + Rotation; // 메시 컴포넌트의 현재 회전

		// BP_Explosion_Sand1 액터 생성
		FActorSpawnParameters SpawnParams;
		SpawnedActor = MeshComp->GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
	}
	
	// 부모 클래스의 NotifyBegin 호출
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UBoss_AnimNotifyState_SandNiagara::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, TotalDuration, EventReference);
}

void UBoss_AnimNotifyState_SandNiagara::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	// 스폰된 액터가 유효하다면 제거
	if (SpawnedActor)
	{
		SpawnedActor->Destroy();
		SpawnedActor = nullptr; // 스폰된 액터 포인터를 초기화
	}

	// 부모 클래스의 NotifyEnd 호출
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
