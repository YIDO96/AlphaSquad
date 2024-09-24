// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SOS_TestingCharacter.generated.h"

// 전방선언
class UInputMappingContext;
class UInputAction;

UCLASS()
class ALPHASQUAD_API ASOS_TestingCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASOS_TestingCharacter();

	// spring and camera 선언
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* boss_springArmComp;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* boss_cameraComp;

	// InputContext를 선언
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = Input)
	UInputAction* Boss_MoveIA;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = Input)
	UInputAction* Boss_LookupIA;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = Input)
	UInputAction* Boss_TurnIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = Input)
	FVector MoveDirection;
	
	// 움직임 Function 을 선언
	void Boss_Move(const FInputActionValue& vluae);

	void Boss_Lookup(const FInputActionValue& vluae);

	void Boss_Turn(const FInputActionValue& vluae);
	
	
	// 매핑 컨텍스트를 헤더 파일에 프로퍼티로 노출
	UInputMappingContext* BossMappingContext;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
