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

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* cameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* MinimapspringArmComp;


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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* RollIA;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* GrenadeGunIA;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* SniperGunIA;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* SniperIA;
	
	UPROPERTY(BlueprintReadOnly, Category="Input")
	bool bUsingGrenadeGun = true;
	
	void ChangeToGrenadeGun(const struct FInputActionValue& inputValue);
	void ChangeToSniperGun(const struct FInputActionValue& inputValue);
	
	void TPSMove(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);
	void TPSJump(const FInputActionValue& Value);

	FVector MoveDirection;

	bool isInvertLookUp;

	void InputFire(const FInputActionValue& Value);
	void ResetFire();
	void SniperAim(const struct FInputActionValue& inputValue);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="GunMesh")
	class USkeletalMeshComponent* gunMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="GunMesh")
	class UStaticMeshComponent* sniperGunComp;

	UPROPERTY(EditDefaultsOnly, Category=SniperUI)
	TSubclassOf<class UUserWidget> sinperUIFactory;

	UPROPERTY()
	class UUserWidget* _sniperUI;

	UPROPERTY(EditDefaultsOnly, Category=BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;

	UPROPERTY(blueprintReadWrite, Category="BulletFactory")
	bool bCanFire = true;
	
	bool bSniperAim = false;
	FTimerHandle FireRateHandle;

	// Effect
	UPROPERTY(EditDefaultsOnly, Category="BulletEffect")
	class UParticleSystem* bulletEffectFactory;

	// Muzzle Fire Effect
	UPROPERTY(EditAnywhere, Category = "MuzzleEffects")
	UParticleSystem* MuzzleFlashEffect;

	UPROPERTY(EditDefaultsOnly, Category=CameraMotion)
	TSubclassOf<class UCameraShakeBase> cameraShake;

	// Sound
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	class USoundBase* RifleSound;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	class USoundBase* SniperSound;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* ReloadSound;

	// Hit
	UFUNCTION(BlueprintCallable, Category = "Health")
	void OnHitEvent(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float  hp = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float initialHp = 100;

	// GameOver
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
	void OnGameOver();

private:
	// player perception script
	class UAIPerceptionStimuliSourceComponent* StimulusSource;
	void SetupStimulusSource();

	float DamageValue;


	/* 인벤토리*/
	// 변수들
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

	AActor* CachedInteractableActor;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsEButtonClick = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int64 CurrentMoney = 9999;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float Health = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 ARMagazineBullet = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 SniperMagazineBullet = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	class UInventoryComponent* InventoryComponent;


	// 함수들
protected:
	/** Called for looking input */
	void InteractionFunc(const FInputActionValue& Value);
	void ReloadFunc(const FInputActionValue& Value);

	void PerformInteractionTrace();

public:
	UFUNCTION(BlueprintCallable)
	void UpdateMoney(int64 inputVal, FName ItemName);

	UFUNCTION(BlueprintCallable)
	bool bIsBuyItem(FName ItemName);
};
