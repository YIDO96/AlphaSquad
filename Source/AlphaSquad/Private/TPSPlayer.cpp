// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Bullet.h"
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include "Perception\AIPerceptionStimuliSourceComponent.h"
#include "Perception\AISense_Sight.h"

#include "LSH/InventoryComponent.h"
#include "LSH/LSH_InteractableInterface.h"


// Sets default values
ATPSPlayer::ATPSPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> InitMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/NSK/Character/Ch15_NSK_nonPBR.Ch15_NSK_nonPBR'"));

	if (InitMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(InitMesh.Object);

		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
	}

	// springArm
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocationAndRotation(FVector(0, 0, 50), FRotator(-20, 0, 0));
	springArmComp->TargetArmLength = 450;
	springArmComp->bUsePawnControlRotation = true;

	// Camera Component
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	cameraComp->SetupAttachment(springArmComp);
	cameraComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;

	// for AIControl Perception
	SetupStimulusSource();

	// 총 스켈레탈메시 컴포넌트 등록
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	// 부모 컴포넌틀를 Mesh 컴포넌트로 설정
	gunMeshComp->SetupAttachment(GetMesh());
	// 스켈레탈메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4'"));
	// 데이터로드가 성공했다면
	if(TempGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		gunMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
	}
	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	sniperGunComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("/Script/Engine.StaticMesh'/Game/NSK/SniperGun/sniper1.sniper1'"));
	if(TempSniperMesh.Succeeded())
	{
		sniperGunComp->SetStaticMesh(TempSniperMesh.Object);
		sniperGunComp->SetRelativeLocation(FVector(-22, 55, 120));
		sniperGunComp->SetRelativeScale3D(FVector(0.15f));
	}

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->MaxInventorySize = 30;
	
}

void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	UAnimInstance* pAnimInst = GetMesh()->GetAnimInstance();
	if (pAnimInst != nullptr)
	{
		pAnimInst->OnPlayMontageNotifyBegin.AddDynamic(this, &ATPSPlayer::HandleOnMontageNotifyBegin);
	}

	_sniperUI = CreateWidget(GetWorld(), sinperUIFactory);

	ChangeToGrenadeGun(FInputActionValue());

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}

	FTimerHandle traceTimerHandle;
	GetWorldTimerManager().SetTimer(traceTimerHandle, this, &ATPSPlayer::PerformInteractionTrace, 0.2f, true);
}

void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(TPSMoveIA, ETriggerEvent::Triggered, this, &ATPSPlayer::TPSMove);

		EnhancedInputComponent->BindAction(LookUpIA, ETriggerEvent::Triggered, this, &ATPSPlayer::LookUp);

		EnhancedInputComponent->BindAction(TurnIA, ETriggerEvent::Triggered, this, &ATPSPlayer::Turn);

		EnhancedInputComponent->BindAction(TPSJumpIA, ETriggerEvent::Triggered, this, &ATPSPlayer::TPSJump);

		EnhancedInputComponent->BindAction(FireIA, ETriggerEvent::Triggered, this, &ATPSPlayer::InputFire);

		EnhancedInputComponent->BindAction(GrenadeGunIA, ETriggerEvent::Started, this, &ATPSPlayer::ChangeToGrenadeGun);

		EnhancedInputComponent->BindAction(SniperGunIA, ETriggerEvent::Started, this, &ATPSPlayer::ChangeToSniperGun);

		EnhancedInputComponent->BindAction(SniperIA, ETriggerEvent::Started, this, &ATPSPlayer::SniperAim);
		EnhancedInputComponent->BindAction(SniperIA, ETriggerEvent::Completed, this, &ATPSPlayer::SniperAim);

		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &ATPSPlayer::InteractionFunc);
		//EnhancedInputComponent->BindAction(RollIA, ETriggerEvent::Started, this, &ATPSPlayer::StartRoll);

		EnhancedInputComponent->BindAction(RollIA, ETriggerEvent::Completed, this, &ATPSPlayer::Dodge);
	}
}

void ATPSPlayer::TPSMove(const FInputActionValue& Value)
{

	const FVector _CurrentValue = Value.Get<FVector>();
	if (Controller)
	{
		MoveDirection.Y = _CurrentValue.X;
		MoveDirection.X = _CurrentValue.Y;
	}

	MoveDirection = FTransform(GetControlRotation()).TransformVector(MoveDirection);

	AddMovementInput(MoveDirection);

	MoveDirection = FVector::ZeroVector;
}

void ATPSPlayer::LookUp(const FInputActionValue& Value)
{
	float _CurrentValue;

	if (isInvertLookUp)
	{
		_CurrentValue = Value.Get<float>();
	}
	else
	{
		_CurrentValue = Value.Get<float>() * -1;
	}


	AddControllerPitchInput(_CurrentValue);
}

void ATPSPlayer::Turn(const FInputActionValue& Value)
{

	const float _CurrentValue = Value.Get<float>();

	AddControllerYawInput(_CurrentValue);
}

void ATPSPlayer::TPSJump(const FInputActionValue& Value)
{
	/*if (bIsRolling)
	{
		return;
	}*/

	Jump();
}

//void ATPSPlayer::StartRoll(const FInputActionValue& Value)
//{
//	if (!bIsRolling)
//	{
//		// 구르기 상태 활성화
//		bIsRolling = true;
//
//		// 구르기 애니메이션 재생 길이
//		float RollDuration = AM_Running_Dive_Roll_Montage->GetPlayLength();
//
//		// 구르기 애니메이션 재생
//		PlayAnimMontage(AM_Running_Dive_Roll_Montage);
//
//		// 현재 이동 방향으로 빠르게 이동 (e.g., 600유닛 정도 앞으로)
//		FVector RollDirection = GetActorForwardVector();
//		LaunchCharacter(RollDirection * 600, true, true);
//
//		// 구르기 종료 타이머 설정 (애니메이션 길이만큼 딜레이)
//		GetWorldTimerManager().SetTimer(RollTimerHandle, this, &ATPSPlayer::EndRoll, RollDuration, false);
//	}
//}

//void ATPSPlayer::EndRoll()
//{
//	bIsRolling = false;
//}

void ATPSPlayer::HandleOnMontageNotifyBegin(FName a_nNotifyName, const FBranchingPointNotifyPayload& a_pBranchingPayload)
{
	if (a_nNotifyName.ToString() == "Dodge")
	{
		m_bIsDodging = false;
	}
}

void ATPSPlayer::Dodge()
{
	if (!bCanFire && CanJump() && !m_bIsDodging)
	{
		UAnimInstance* pAnimInst = GetMesh()->GetAnimInstance();
		if (pAnimInst != nullptr)
		{
			m_bIsDodging = true;

			pAnimInst->Montage_Play(AM_Running_Dive_Roll_Montage);
			LaunchCharacter(GetActorForwardVector() * 2500, true, true);
		}
	}
}



void ATPSPlayer::InputFire(const FInputActionValue& Value)
{
	/*if (bIsRolling)
	{
		return;
	}*/

	if(bUsingGrenadeGun)
	{
		// 총알이 없다면 발사금지
		if (InventoryComponent->Inventory[FName("ARMagazine")].Quantity <= 0)
		{
			return;
		}

		if(bCanFire)
		{
			FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
			GetWorld()->SpawnActor<ABullet>(bulletFactory,firePosition);

			bCanFire = false;

			GetWorldTimerManager().SetTimer(FireRateHandle,this,&ATPSPlayer::ResetFire,0.1f,false);

			
		}
	}
	else
	{
		// 총알이 없다면 발사금지
		if (InventoryComponent->Inventory[FName("SniperMagazine")].Quantity <= 0)
		{
			return;
		}

		if(bCanFire)
		{
			FVector startPos = cameraComp->GetComponentLocation();
			FVector endPos = cameraComp->GetComponentLocation() + cameraComp->GetForwardVector() * 5000;
			FHitResult hitInfo;
			FCollisionQueryParams params;
			params.AddIgnoredActor(this);
			bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo,startPos,endPos,ECC_Visibility,params);
			if(bHit)
			{
				FTransform bulletTrans;
				bulletTrans.SetLocation(hitInfo.ImpactPoint);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);
			}

			bCanFire = false;

			GetWorldTimerManager().SetTimer(FireRateHandle,this,&ATPSPlayer::ResetFire,2.0f,false);
		}
	}
}

void ATPSPlayer::ResetFire()
{
	bCanFire = true;

	// AR일때
	if (bUsingGrenmadeGun)
	{
		InventoryComponent->Inventory[FName("ARMagazine")].Quantity--;
	}
	else // 스나이퍼일때
	{
		InventoryComponent->Inventory[FName("SniperMagazine")].Quantity--;
	}
}

void ATPSPlayer::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource)
	{
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

void ATPSPlayer::InteractionFunc(const FInputActionValue& Value)
{
	// 상호작용을 수행할 액터가 캐싱되어 있는지 확인
	if (CachedInteractableActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("InteractionFunc- CachedInteractableAction Valid"));
		ILSH_InteractableInterface* CachedInteractable = Cast<ILSH_InteractableInterface>(CachedInteractableActor);
		if (CachedInteractable)
		{
			// 상호작용 수행
			IsEButtonClick = true;
			CachedInteractable->Interact();
		}
	}
}

void ATPSPlayer::PerformInteractionTrace()
{
	FVector startPoint = GetActorLocation();
	FVector endPoint = startPoint + GetActorForwardVector() * 200.0f;
	FHitResult hitOut;

	FCollisionQueryParams traceParams;
	traceParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(hitOut, startPoint, endPoint, ECC_GameTraceChannel2, traceParams);

	// bHit == true : Trace 결과 값이 있다면
	if (bHit)
	{
		AActor* hitActor = hitOut.GetActor();
		// 맞은 친구가 있다면
		if (hitActor)
		{
			// 해당 액터에, 특정 인터페이스를 소유하고 있는지 여부를 알고싶다면, Casting 해보면 된다.
			// Cast 성공하며, 해당 인터페이스를 소유, 실패하면 소유x
			ILSH_InteractableInterface* InteractableActor = Cast<ILSH_InteractableInterface>(hitActor);

			if (InteractableActor)
			{
				// 새로운 상호작용 대상이 이전 상호작용 대상과 다르다면
				if (CachedInteractableActor != hitActor)
				{
					// 이전 상호작용 대상이 있다면 위젯을 off
					ILSH_InteractableInterface* CachedInteractable = Cast<ILSH_InteractableInterface>(CachedInteractableActor);
					if (CachedInteractable)
					{
						CachedInteractable->HideInteractionWidget();
					}
				}

				if (not IsEButtonClick)
				{
					// 새로운 액터를 캐싱하고 위젯 표시
					CachedInteractableActor = hitActor;
					InteractableActor->DisplayInteractionWidget();
				}

			}
		}
	}
	else // 노 히트일때
	{
		// 과거에 상호작용한 대상이 있다면
		if (CachedInteractableActor)
		{
			ILSH_InteractableInterface* CachedInteractable = Cast<ILSH_InteractableInterface>(CachedInteractableActor);
			if (CachedInteractable)
			{
				CachedInteractable->HideInteractionWidget();
			}

			// 상호작용중인 대상이 없기에 nullptr값 넣어주기
			CachedInteractableActor = nullptr;
		}
	}

	// DebugLine 그리기
	//if (bHit)
	//{
	//	// 히트된 위치까지의 디버그 라인 그리기
	//	DrawDebugLine(GetWorld(), startPoint, hitOut.ImpactPoint, FColor::Red, false, 5.0f, 0, 2.0f);
	//	// 히트된 위치에 디버그 스피어 그리기
	//	DrawDebugSphere(GetWorld(), hitOut.ImpactPoint, 10.0f, 12, FColor::Yellow, false, 5.0f);
	//}
	//else
	//{
	//	// 트레이스 전체 범위에 디버그 라인 그리기
	//	DrawDebugLine(GetWorld(), startPoint, endPoint, FColor::Blue, false, 5.0f, 0, 2.0f);
	//}
}

void ATPSPlayer::UpdateMoney(int64 inputVal, FName ItemName)
{
	int _result = CurrentMoney + inputVal;

	// 아이템이 있다면
	if (InventoryComponent->Inventory.Contains(ItemName))
	{
		// 구매하려는 아이템의 가질수있는 맥스 갯수보다 현재 개수가 크거나 같다면 구매 불가
		if (InventoryComponent->Inventory[ItemName].ItemData.MaxStackCount <= InventoryComponent->Inventory[ItemName].Quantity)
		{
			return;
		}
	}


	if (_result < 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Not Enough Money!"));
		return;
	}
	else
	{
		CurrentMoney = _result;
	}
}

bool ATPSPlayer::bIsBuyItem(FName ItemName)
{
	if (InventoryComponent->Inventory.Contains(ItemName))
	{
		// 구매하려는 아이템의 가질수있는 맥스 갯수보다 현재 개수가 크거나 같다면 구매 불가
		if (InventoryComponent->Inventory[ItemName].ItemData.MaxStackCount <= InventoryComponent->Inventory[ItemName].Quantity)
		{
			return false;
		}
		// 현재머니가 아이템 가격보다 적다면 구매 불가
		else if (CurrentMoney < InventoryComponent->Inventory[ItemName].ItemData.ItemPrice)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else return true;
}

void ATPSPlayer::ChangeToGrenadeGun(const struct FInputActionValue& inputValue)
{
	bUsingGrenadeGun = true;
	sniperGunComp->SetVisibility(false);
	gunMeshComp->SetVisibility(true);
}

void ATPSPlayer::ChangeToSniperGun(const struct FInputActionValue& inputValue)
{
	// 인벤토리에 스나이퍼가 있다면 변경 가능
	if (InventoryComponent->Inventory.Contains(FName("Sniper")))
	{
		bUsingGrenmadeGun = false;
		sniperGunComp->SetVisibility(true);
		gunMeshComp->SetVisibility(false);
	}
	bUsingGrenadeGun = false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);
}

void ATPSPlayer::SniperAim(const struct FInputActionValue& inputValue)
{
	if (not bUsingGrenmadeGun)
	{
		if (bSniperAim == false)
		{
			bSniperAim = true;
			_sniperUI->AddToViewport();
			cameraComp->SetFieldOfView(45.0f);
		}
		else
		{
			bSniperAim = false;
			_sniperUI->RemoveFromParent();
			cameraComp->SetFieldOfView(90.0f);
		}
		bSniperAim = true;
		_sniperUI->AddToViewport();

		/*FVector CameraLocation = cameraComp->GetComponentLocation();
		FVector ForwardVector = cameraComp->GetForwardVector();*/

		cameraComp->SetFieldOfView(45.0f);
	}
	else
	{
		bSniperAim = false;
		_sniperUI->RemoveFromParent();
		cameraComp->SetFieldOfView(90.0f);
	}
}