// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 스켈레탈 메쉬 초기화
	ConstructorHelpers::FObjectFinder<USkeletalMesh> InitMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/CustomContent/Character/Ch15_nonPBR.Ch15_nonPBR'"));

	if (InitMesh.Succeeded()) // 제대로 가져왔다면
	{
		GetMesh()->SetSkeletalMesh(InitMesh.Object);

		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
	}

	// springArm 생성 - 초기화
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocationAndRotation(FVector(0, 0, 50), FRotator(-20, 0, 0));
	springArmComp->TargetArmLength = 450;
	springArmComp->bUsePawnControlRotation = true;

	// Camera Component 초기화
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	cameraComp->SetupAttachment(springArmComp);
	cameraComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(TPSMoveIA, ETriggerEvent::Triggered, this, &ATPSPlayer::TPSMove);

		EnhancedInputComponent->BindAction(LookUpIA, ETriggerEvent::Triggered, this, &ATPSPlayer::LookUp);

		EnhancedInputComponent->BindAction(TurnIA, ETriggerEvent::Triggered, this, &ATPSPlayer::Turn);

		EnhancedInputComponent->BindAction(TPSJumpIA, ETriggerEvent::Triggered, this, &ATPSPlayer::TPSJump);
	}
}

void ATPSPlayer::TPSMove(const FInputActionValue& Value)
{
	// 입력 값을 FVector 형태로 가져옴
	const FVector _CurrentValue = Value.Get<FVector>();
	if (Controller)
	{
		MoveDirection.Y = _CurrentValue.X; // 좌우 방향 값
		MoveDirection.X = _CurrentValue.Y; // 전후 방향 값
	}

	// 카메라의 현재 회전에 맞춰 MoveDirection을 변환
	MoveDirection = FTransform(GetControlRotation()).TransformVector(MoveDirection);
	// 변환된 MoveDirection 방향으로 캐릭터를 이동
	AddMovementInput(MoveDirection);
	// MoveDirection 초기화 다음 프레임 누적 방지
	MoveDirection = FVector::ZeroVector;
}

void ATPSPlayer::LookUp(const FInputActionValue& Value)
{
	// 입력 값을 float 형태로 가져옴
	float _CurrentValue;

	if (isInvertLookUp)
	{
		_CurrentValue = Value.Get<float>();
	}
	else
	{
		_CurrentValue = Value.Get<float>() * -1;
	}

	// 카메라의 Pitch 값을 변경하여 위아래 시점 이동을 수행
	AddControllerPitchInput(_CurrentValue);
}

void ATPSPlayer::Turn(const FInputActionValue& Value)
{
	// 입력 값을 float 형태로 가져옴
	const float _CurrentValue = Value.Get<float>();
	// 카메라의 Yaw 값을 변경하여 좌우 시점 이동을 수행
	AddControllerYawInput(_CurrentValue);
}

void ATPSPlayer::TPSJump(const FInputActionValue& Value)
{
	Jump();
}
