// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/LSH_Machine.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALSH_Machine::ALSH_Machine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Interaction"));
	CameraComp->SetupAttachment(RootComponent);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidgetComponent"));
	InteractionWidgetComponent->SetupAttachment(RootComponent);

	InteractionWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
	InteractionWidgetComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::World); // 월드에 고성된 형태로 표시 (Space는 화면)
	InteractionWidgetComponent->SetDrawSize(FVector2D(200.0f, 50.0f)); // 위젯 크기 설정

	if (InteractionWidgetClass)
	{
		InteractionWidgetComponent->SetWidgetClass(InteractionWidgetClass);
	}
}

// Called when the game starts or when spawned
void ALSH_Machine::BeginPlay()
{
	Super::BeginPlay();
	if (InteractionWidgetClass)
	{
		InteractionWidgetComponent->SetWidgetClass(InteractionWidgetClass);
		InteractionWidgetComponent->SetVisibility(false);
	}
}

// Called every frame
void ALSH_Machine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALSH_Machine::DisplayInteractionWidget()
{
	if (InteractionWidgetComponent)
	{
		InteractionWidgetComponent->SetVisibility(true);
	}
}

void ALSH_Machine::HideInteractionWidget()
{
	if (InteractionWidgetComponent)
	{
		InteractionWidgetComponent->SetVisibility(false);
	}
}

void ALSH_Machine::Interact()
{
	//TODO:상호작용 추가

	// 카메라 포커스
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);

	// 카메라의 시점을 다른 Actor로 부드럽게 전환하는데 사용되는 함수이다.
	// 주로 플레이어가 특정 액터나 카메라에서 시점을 바꾸거나 전환할때 사용
	playerController->SetViewTargetWithBlend(this, 1);
	playerController->SetInputMode(FInputModeUIOnly());
	playerController->SetShowMouseCursor(true);

	HideInteractionWidget();

	if (ItemShopWidgetClass)
	{
		ItemShopWidget = CreateWidget<UUserWidget>(GetWorld(), ItemShopWidgetClass);

		FName PropertyName(TEXT("OwnerNPC"));
		// 'Expose on Spawn'가 활성화된 NPC 변수를 설정
		// 블루프린트 위젯에 'OwnerNPC'라는 이름으로 설정된 변수(Expose on Spawn 옵션이 활성화된 변수)를 찾는다.
		// 이 변수는 NPC 객체(ANPC 클래스)를 가리킨다.
		FProperty* npcProperty = ItemShopWidget->GetClass()->FindPropertyByName(PropertyName);

		// npcProperty가 유효한지 확인
		// 위의 FindPropertyByName 함수가 정상적으로 'OwnerNPC' 변수를 찾았는지 확인
		// 만약 OwnerNPC라는 변수를 블루프린트에서 찾지 못했다면, npcProperty는 nullptr이 된다.
		if (npcProperty)
		{
			// 찾는 npcProperty 변수를 FObjectProperty로 캐스팅한다.
			// npcProperty가 객체 타입의 변수인 경우에만 이를 FObjectProperty로 캐스팅할 수 있다.
			// 여기서 OwnerNPC가 ANP타입의 객체 변수이므로 FObjectProperty로 캐스팅을 시도한다.
			FObjectProperty* objectProperty = CastField<FObjectProperty>(npcProperty);

			// objectProperty이 유효한지 확인하고, 변수가 ANPC 클래스의 객체를 가리키는지 확인한다.
			// objectProperty가 유요하고, 그 변수가 정확히 ANPC 클래스의 객체인 경우에만 아래 코드 실행
			if (objectProperty && objectProperty->PropertyClass == ALSH_Machine::StaticClass())
			{
				// 찾은 ObjectPorperty 변수에 현재 NPC 객체(this)를 할당.
				// 'SetObjectPropertyValue_InContainer' 함수는 주어진 객체(ItemShopWidget)의 특정 객체 변수(ObjectProperty)에 값을 설정해준다.
				// 여기서 ItemShopWidget 위젯의 OwnerNPC 변수에 현재 NPC 객체(this)를 설정하는 역할을 한다.
				objectProperty->SetObjectPropertyValue_InContainer(ItemShopWidget, this);
			}
		}

		if (ItemShopWidget) ItemShopWidget->AddToViewport();
	}
}

FText ALSH_Machine::GetQuestText()
{
	switch(NumberType)
	{
	case 0:
		return FText();
		break;
	case 1:
		return FText::FromString(TEXT("Kill 50 Enemy"));
		break;
	case 2:
		return FText::FromString(TEXT("Kill All Enemy"));
		break;
	case 3:
		return FText::FromString(TEXT("Kill 70 Enemy"));
		break;
	case 4:
		return FText::FromString(TEXT("Kill Boss"));
		break;
	default:
		return FText();
		break;
	}
}

