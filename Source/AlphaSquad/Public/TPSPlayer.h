// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "TPSPlayer.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class ALPHASQUAD_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ���� ���� �߰� ����

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* cameraComp;

	// ���� ���ؽ�Ʈ ��� ���Ͽ� ������Ƽ�� ����
	UPROPERTY(EditAnyWhere, Category = "Input")
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* TPSMoveIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookUpIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* TurnIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* TPSJumpIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* FireIA;

	void TPSMove(const FInputActionValue& Value);

	void LookUp(const FInputActionValue& Value);
	
	void Turn(const FInputActionValue& Value);

	void TPSJump(const FInputActionValue& Value);

	void InputFire(const FInputActionValue& Value);

	void FireBullet();

	FVector MoveDirection;

	bool isInvertLookUp;

	UPROPERTY(VisibleAnywhere, Category="GunMesh")
	class USkeletalMeshComponent* gunMeshComp;

	UPROPERTY(EditDefaultsOnly, Category=BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;

private:
	// player perception script
	class UAIPerceptionStimuliSourceComponent* StimulusSource;
	void SetupStimulusSource();
};
