// Fill out your copyright notice in the Description page of Project Settings.


#include "SOS_TestingCharacter.h"

#include "AnimationEditorViewportClient.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
ASOS_TestingCharacter::ASOS_TestingCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//스켈레탈 메쉬 초기화
	ConstructorHelpers::FObjectFinder<USkeletalMesh>
	InitMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/SOS/AI/Sci_Fi_Character_08/Mesh/Character/SK_Sci_Fi_Character_08_Full_01.SK_Sci_Fi_Character_08_Full_01'"));

	// 제데로 가져왔다면 (Valid Check)
	if(InitMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(InitMesh.Object);

		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,88),FRotator(0,-90,0));
	}

	//springArm 생성 - 초기화
	boss_springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("boss_springArmComp"));
	boss_springArmComp->SetupAttachment(RootComponent);
	boss_springArmComp->SetRelativeLocationAndRotation(FVector(0,0,50),FRotator(-20,0,0));
	boss_springArmComp->TargetArmLength = 530;	
	// boss_springArmComp->bUsePawnControlRotation = true;
	
	//camera component 초기화
	boss_cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("boss_cameraComp"));
	boss_cameraComp->SetupAttachment(boss_springArmComp);
	// boss_cameraComp->bUsePawnControlRotation = false;
	
	//bUseControllerRotationYaw = true;
	
}



// Called when the game starts or when spawned
void ASOS_TestingCharacter::BeginPlay()
{
	Super::BeginPlay();

	//EnhancedInputMapping
	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem
			=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BossMappingContext,0);
		}
	}
	
}

// Called every frame
void ASOS_TestingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASOS_TestingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// InputComponent가 맞는지 확인
	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// bind Move
		EnhancedInputComponent->BindAction(Boss_MoveIA, ETriggerEvent::Triggered, this, &ASOS_TestingCharacter::Boss_Move);
		// bind Lookup
		EnhancedInputComponent->BindAction(Boss_LookupIA, ETriggerEvent::Triggered, this, &ASOS_TestingCharacter::Boss_Lookup);
		// bind Turn
		EnhancedInputComponent->BindAction(Boss_TurnIA, ETriggerEvent::Triggered, this, &ASOS_TestingCharacter::Boss_Turn);
		
	}
}

void ASOS_TestingCharacter::Boss_Move(const FInputActionValue& Value)
{
	// 입력 값을 FVector 형태로 가져옵니다.
	const FVector _CurrentValue = Value.Get<FVector>();

	
	if(Controller)
	{
		// MoveDirection 벡터에 입력 된 X, Y 값을 설정합니다.
		MoveDirection.Y = _CurrentValue.X; // 좌우 방향 값
		MoveDirection.X = _CurrentValue.Y; // 전후 방향 값
	}

	// 카메라의 현재 회전에 맞춰 MoveDirection을 변환합니다.
	MoveDirection = FTransform(GetControlRotation()).TransformVector(MoveDirection);
	AddMovementInput(MoveDirection);
	MoveDirection = FVector(0, 0, 0);
	
}

void ASOS_TestingCharacter::Boss_Lookup(const FInputActionValue& Value)
{
	// 입력 값을 FVector 형태로 가져옵니다.
	const float _CurrentValue = Value.Get<float>();
	// 카메라의 Pitch 값을 변경하여 위아래 시점 이동을 수행합니다.
	AddControllerPitchInput(_CurrentValue);
	UE_LOG(LogTemp, Warning, TEXT("FUCK"));
	
}

void ASOS_TestingCharacter::Boss_Turn(const FInputActionValue& Value)
{
	// 입력 값을 FVector 형태로 가져옵니다.
	const float _CurrentValue = Value.Get<float>();
	// 카메라의 Pitch 값을 변경하여 위아래 시점 이동을 수행합니다.
	AddControllerYawInput(_CurrentValue);
	
}
