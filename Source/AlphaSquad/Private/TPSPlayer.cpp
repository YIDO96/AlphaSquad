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

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ���̷�Ż �޽� �ʱ�ȭ
	ConstructorHelpers::FObjectFinder<USkeletalMesh> InitMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/NSK/Character/Ch15_NSK_nonPBR.Ch15_NSK_nonPBR'"));

	if (InitMesh.Succeeded()) // ����� �����Դٸ�
	{
		GetMesh()->SetSkeletalMesh(InitMesh.Object);

		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
	}

	// springArm ���� - �ʱ�ȭ
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocationAndRotation(FVector(0, 0, 50), FRotator(-20, 0, 0));
	springArmComp->TargetArmLength = 450;
	springArmComp->bUsePawnControlRotation = true;

	// Camera Component �ʱ�ȭ
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
	}
}

void ATPSPlayer::TPSMove(const FInputActionValue& Value)
{
	// �Է� ���� FVector ���·� ������
	const FVector _CurrentValue = Value.Get<FVector>();
	if (Controller)
	{
		MoveDirection.Y = _CurrentValue.X; // �¿� ���� ��
		MoveDirection.X = _CurrentValue.Y; // ���� ���� ��
	}

	// ī�޶��� ���� ȸ���� ���� MoveDirection�� ��ȯ
	MoveDirection = FTransform(GetControlRotation()).TransformVector(MoveDirection);
	// ��ȯ�� MoveDirection �������� ĳ���͸� �̵�
	AddMovementInput(MoveDirection);
	// MoveDirection �ʱ�ȭ ���� ������ ���� ����
	MoveDirection = FVector::ZeroVector;
}

void ATPSPlayer::LookUp(const FInputActionValue& Value)
{
	// �Է� ���� float ���·� ������
	float _CurrentValue;

	if (isInvertLookUp)
	{
		_CurrentValue = Value.Get<float>();
	}
	else
	{
		_CurrentValue = Value.Get<float>() * -1;
	}

	// ī�޶��� Pitch ���� �����Ͽ� ���Ʒ� ���� �̵��� ����
	AddControllerPitchInput(_CurrentValue);
}

void ATPSPlayer::Turn(const FInputActionValue& Value)
{
	// �Է� ���� float ���·� ������
	const float _CurrentValue = Value.Get<float>();
	// ī�޶��� Yaw ���� �����Ͽ� �¿� ���� �̵��� ����
	AddControllerYawInput(_CurrentValue);
}

void ATPSPlayer::TPSJump(const FInputActionValue& Value)
{
	Jump();
}

void ATPSPlayer::InputFire(const FInputActionValue& Value)
{
	if(bUsingGrenmadeGun)
	{
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

void ATPSPlayer::ChangeToGrenadeGun(const struct FInputActionValue& inputValue)
{
	bUsingGrenmadeGun = true;
	sniperGunComp->SetVisibility(false);
	gunMeshComp->SetVisibility(true);
}

void ATPSPlayer::ChangeToSniperGun(const struct FInputActionValue& inputValue)
{
	bUsingGrenmadeGun = false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);
}

void ATPSPlayer::SniperAim(const struct FInputActionValue& inputValue)
{
	if(bSniperAim == false)
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
}