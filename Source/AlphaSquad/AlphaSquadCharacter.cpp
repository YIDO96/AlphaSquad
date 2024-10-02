// Copyright Epic Games, Inc. All Rights Reserved.

#include "AlphaSquadCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "LSH/LSH_InteractableInterface.h"
#include "LSH/InventoryComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AAlphaSquadCharacter

AAlphaSquadCharacter::AAlphaSquadCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->MaxInventorySize = 30;
}

void AAlphaSquadCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	FTimerHandle traceTimerHandle;
	GetWorldTimerManager().SetTimer(traceTimerHandle, this, &AAlphaSquadCharacter::PerformInteractionTrace, 0.2f, true);
}

void AAlphaSquadCharacter::UpdateMoney(int64 inputVal, FName ItemName)
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
	}
	else
	{
		CurrentMoney = _result;
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAlphaSquadCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAlphaSquadCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAlphaSquadCharacter::Look);

		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &AAlphaSquadCharacter::InteractionFunc);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AAlphaSquadCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AAlphaSquadCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AAlphaSquadCharacter::InteractionFunc(const FInputActionValue& Value)
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

void AAlphaSquadCharacter::PerformInteractionTrace()
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
