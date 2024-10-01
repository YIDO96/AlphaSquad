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

	// 여기 부터 추가 내역

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* cameraComp;

	// 매핑 컨텍스트 헤더 파일에 프로퍼티로 노출
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

	void TPSMove(const FInputActionValue& Value);

	void LookUp(const FInputActionValue& Value);
	
	void Turn(const FInputActionValue& Value);

	void TPSJump(const FInputActionValue& Value);

	FVector MoveDirection;

	bool isInvertLookUp;


private:
	// player perception script
	class UAIPerceptionStimuliSourceComponent* StimulusSource;
	void SetupStimulusSource();
};
