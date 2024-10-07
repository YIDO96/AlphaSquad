// Fill out your copyright notice in the Description page of Project Settings.


#include "NSK/TPSPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "NSK/Bullet.h"
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
	//springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetupAttachment(GetMesh());
	springArmComp->SetRelativeLocationAndRotation(FVector(-35, 0, 150), FRotator(0, 0, 0));
	springArmComp->TargetArmLength = 120;
	springArmComp->bUsePawnControlRotation = true;

	// Camera Component
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	cameraComp->SetupAttachment(springArmComp);
	cameraComp->bUsePawnControlRotation = false;

	MinimapspringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("MinimapspringArmComp"));
	MinimapspringArmComp->SetupAttachment(RootComponent);
	MinimapspringArmComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;

	// for AIControl Perception
	SetupStimulusSource();

	FName WeaponSocket = TEXT("Rifle"); // 총을 장착할 소켓 이름
	if (GetMesh()->DoesSocketExist(WeaponSocket)) // Mesh에 Rifle이라는 이름의 소켓이 존재한다면
	{
		// 총 스켈레탈메시 컴포넌트 등록
		gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
		// 부모 컴포넌틀를 Mesh 컴포넌트로 설정
		gunMeshComp->SetupAttachment(GetMesh(), WeaponSocket);
		// 스켈레탈메시 데이터 로드
		ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4'"));
		// 데이터로드가 성공했다면
		if (TempGunMesh.Succeeded())
		{
			gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
			//gunMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
		}

		sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
		sniperGunComp->SetupAttachment(GetMesh(), WeaponSocket);
		ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("/Script/Engine.StaticMesh'/Game/NSK/SniperGun/sniper1.sniper1'"));

		// 스나이퍼 메쉬 가져오기에 성공했다면
		if (TempSniperMesh.Succeeded())
		{
			sniperGunComp->SetStaticMesh(TempSniperMesh.Object);
			sniperGunComp->SetRelativeLocation(FVector(0, 30, 5));
			sniperGunComp->SetRelativeScale3D(FVector(0.15f));
		}
	}

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->MaxInventorySize = 30;

	// Rifle Sound
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("/Script/Engine.SoundWave'/Game/NSK/SniperGun/Rifle.Rifle'"));
	if (tempSound.Succeeded())
	{
		RifleSound = tempSound.Object;
	}

	// Reload Sound
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound2(TEXT("/Script/Engine.SoundWave'/Game/MilitaryWeapSilver/Sound/Rifle/Wavs/Rifle_Reload02.Rifle_Reload02'"));
	if(tempSound2.Succeeded())
	{
		ReloadSound = tempSound2.Object;
	}
	// Sniper Sound
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound3(TEXT("/Script/Engine.SoundWave'/Game/MilitaryWeapSilver/Sound/Shotgun/Wavs/ShotgunA_Fire04.ShotgunA_Fire04'"));
	if(tempSound3.Succeeded())
	{
		SniperSound = tempSound3.Object;
	}
}
	
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

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

	hp = initialHp;
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

		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &ATPSPlayer::InteractionFunc);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ATPSPlayer::ReloadFunc);
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
	Jump();
}

void ATPSPlayer::InputFire(const FInputActionValue& Value)
{
	if(bUsingGrenadeGun)
	{
		// 총알이 없다면 발사금지
		if (ARMagazineBullet <= 0)
		{
			return;
		}
		
		// Rifle
		if(bCanFire)
		{
			// 유탄 관련 코드
			//FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
			//GetWorld()->SpawnActor<ABullet>(bulletFactory,firePosition);

			// Muzzle Fire Effect
			if(gunMeshComp)
			{
				FTransform muzzleTransform = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlashEffect, muzzleTransform);
			}

			// CameraShake
			auto controller = GetWorld()->GetFirstPlayerController();
			controller->PlayerCameraManager->StartCameraShake(cameraShake);

			// Sound
			UGameplayStatics::PlaySound2D(GetWorld(), RifleSound);

			FVector startPoint = cameraComp->GetComponentLocation();
			FVector endPoint = startPoint + cameraComp->GetForwardVector() * 10000.0f;
			FHitResult hitOut;

			FCollisionQueryParams traceParams;
			traceParams.AddIgnoredActor(this);

			bool bHit = GetWorld()->LineTraceSingleByChannel(hitOut, startPoint, endPoint, ECC_GameTraceChannel2, traceParams);

			if (bHit)
			{
					FTransform bulletTrans;
					bulletTrans.SetLocation(hitOut.ImpactPoint);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);
			}
			else
			{
				// 트레이스 전체 범위에 디버그 라인 그리기
				//DrawDebugLine(GetWorld(), startPoint, endPoint, FColor::Blue, false, 5.0f, 0, 2.0f);
			}

			bCanFire = false;

			GetWorldTimerManager().SetTimer(FireRateHandle,this,&ATPSPlayer::ResetFire,0.1f,false);
		}
	}
	// Sniper
	else
	{
		// 총알이 없다면 발사금지
		if (SniperMagazineBullet <= 0)
		{
			return;
		}

		// 조준경 확대 시 만 사격 가능하게
		if (bSniperAim == false)
		{
			return;
		}

		if(bCanFire)
		{
			// Muzzle Fire Effect
			if (gunMeshComp)
			{
				FTransform muzzleTransform = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlashEffect, muzzleTransform);
			}
			
			// CameraShake
			auto controller = GetWorld()->GetFirstPlayerController();
			controller->PlayerCameraManager->StartCameraShake(cameraShake);
			
			// Sound
			UGameplayStatics::PlaySound2D(GetWorld(), SniperSound);
			
			FVector startPos = cameraComp->GetComponentLocation();
			FVector endPos = cameraComp->GetComponentLocation() + cameraComp->GetForwardVector() * 20000;
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

			if (bHit)
			{
				// 히트된 위치에 디버그 스피어 그리기
				DrawDebugSphere(GetWorld(), hitInfo.ImpactPoint, 10.0f, 12, FColor::Yellow, false, 5.0f);
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
	if (bUsingGrenadeGun)
	{
		//InventoryComponent->Inventory[FName("ARMagazine")].Quantity--;
		ARMagazineBullet--;
	}
	else // 스나이퍼일때
	{
		//InventoryComponent->Inventory[FName("SniperMagazine")].Quantity--;
		SniperMagazineBullet--;
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

void ATPSPlayer::ReloadFunc(const FInputActionValue& Value)
{
	if (bUsingGrenadeGun) // AR일때
	{
		if (InventoryComponent->Inventory.Contains(FName("ARMagazine")))
		{
		// 총알이 없다면 장전 불가
			if (InventoryComponent->Inventory[FName("ARMagazine")].Quantity <= 0)
			{
				return;
			}

			// 총알이 30발이라면 장전 불가
			if (ARMagazineBullet == 30)
			{
				return;
			}

			// 총알 전체 탄수가 30보다 작거나 같으면
			if (InventoryComponent->Inventory[FName("ARMagazine")].Quantity + ARMagazineBullet <= 30)
			{
				ARMagazineBullet += InventoryComponent->Inventory[FName("ARMagazine")].Quantity;
				InventoryComponent->Inventory[FName("ARMagazine")].Quantity = 0;
			}
			else
			{ // ARMagazineBullet 27 Quantity 10 -> 
				InventoryComponent->Inventory[FName("ARMagazine")].Quantity -= (30 -ARMagazineBullet);
				ARMagazineBullet = 30;
			}
		}
	}
	else // 스나이퍼일때
	{
		if (InventoryComponent->Inventory.Contains(FName("SniperMagazine")))
		{
			// 총알이 없다면 장전 불가
			if (InventoryComponent->Inventory[FName("SniperMagazine")].Quantity <= 0)
			{
				return;
			}

			// 총알이 30발이라면 장전 불가
			if (SniperMagazineBullet == 10)
			{
				return;
			}

			// 총알 전체 탄수가 30보다 작거나 같으면
			if (InventoryComponent->Inventory[FName("SniperMagazine")].Quantity + SniperMagazineBullet <= 10)
			{
				SniperMagazineBullet += InventoryComponent->Inventory[FName("SniperMagazine")].Quantity;
				InventoryComponent->Inventory[FName("SniperMagazine")].Quantity = 0;
			}
			else
			{ // ARMagazineBullet 27 Quantity 10 -> 
				InventoryComponent->Inventory[FName("SniperMagazine")].Quantity -= (10 - SniperMagazineBullet);
				SniperMagazineBullet = 10;
			}
		}
	}
	// Sound
	UGameplayStatics::PlaySound2D(GetWorld(), ReloadSound);
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

	// 스나이퍼 에임 종료
	bSniperAim = false;
	_sniperUI->RemoveFromParent();
	cameraComp->SetFieldOfView(90.0f);
}

void ATPSPlayer::ChangeToSniperGun(const struct FInputActionValue& inputValue)
{
	// 인벤토리에 스나이퍼가 있다면 변경 가능
	if (InventoryComponent->Inventory.Contains(FName("Sniper")))
	{
		bUsingGrenadeGun = false;
		sniperGunComp->SetVisibility(true);
		gunMeshComp->SetVisibility(false);
	}
}

void ATPSPlayer::SniperAim(const struct FInputActionValue& inputValue)
{
	if (not bUsingGrenadeGun)
	{
		bSniperAim = not bSniperAim;
		if (bSniperAim == false)
		{
			_sniperUI->AddToViewport();
			cameraComp->SetFieldOfView(45.0f);
		}
		else
		{
			_sniperUI->RemoveFromParent();
			cameraComp->SetFieldOfView(90.0f);
		}
	}
}

void ATPSPlayer::OnHitEvent()
{
	// Damaged
	hp--;
	if (hp <= 0)
	{
		//Dead
		OnGameOver();
	}
}

void ATPSPlayer::OnGameOver_Implementation()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}