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
	InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::World); // ���忡 ���� ���·� ǥ�� (Space�� ȭ��)
	InteractionWidgetComponent->SetDrawSize(FVector2D(200.0f, 50.0f)); // ���� ũ�� ����

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
	//TODO:��ȣ�ۿ� �߰�

	// ī�޶� ��Ŀ��
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);

	// ī�޶��� ������ �ٸ� Actor�� �ε巴�� ��ȯ�ϴµ� ���Ǵ� �Լ��̴�.
	// �ַ� �÷��̾ Ư�� ���ͳ� ī�޶󿡼� ������ �ٲٰų� ��ȯ�Ҷ� ���
	playerController->SetViewTargetWithBlend(this, 1);
	playerController->SetInputMode(FInputModeUIOnly());
	playerController->SetShowMouseCursor(true);

	HideInteractionWidget();

	if (ItemShopWidgetClass)
	{
		ItemShopWidget = CreateWidget<UUserWidget>(GetWorld(), ItemShopWidgetClass);

		FName PropertyName(TEXT("OwnerNPC"));
		// 'Expose on Spawn'�� Ȱ��ȭ�� NPC ������ ����
		// �������Ʈ ������ 'OwnerNPC'��� �̸����� ������ ����(Expose on Spawn �ɼ��� Ȱ��ȭ�� ����)�� ã�´�.
		// �� ������ NPC ��ü(ANPC Ŭ����)�� ����Ų��.
		FProperty* npcProperty = ItemShopWidget->GetClass()->FindPropertyByName(PropertyName);

		// npcProperty�� ��ȿ���� Ȯ��
		// ���� FindPropertyByName �Լ��� ���������� 'OwnerNPC' ������ ã�Ҵ��� Ȯ��
		// ���� OwnerNPC��� ������ �������Ʈ���� ã�� ���ߴٸ�, npcProperty�� nullptr�� �ȴ�.
		if (npcProperty)
		{
			// ã�� npcProperty ������ FObjectProperty�� ĳ�����Ѵ�.
			// npcProperty�� ��ü Ÿ���� ������ ��쿡�� �̸� FObjectProperty�� ĳ������ �� �ִ�.
			// ���⼭ OwnerNPC�� ANPŸ���� ��ü �����̹Ƿ� FObjectProperty�� ĳ������ �õ��Ѵ�.
			FObjectProperty* objectProperty = CastField<FObjectProperty>(npcProperty);

			// objectProperty�� ��ȿ���� Ȯ���ϰ�, ������ ANPC Ŭ������ ��ü�� ����Ű���� Ȯ���Ѵ�.
			// objectProperty�� �����ϰ�, �� ������ ��Ȯ�� ANPC Ŭ������ ��ü�� ��쿡�� �Ʒ� �ڵ� ����
			if (objectProperty && objectProperty->PropertyClass == ALSH_Machine::StaticClass())
			{
				// ã�� ObjectPorperty ������ ���� NPC ��ü(this)�� �Ҵ�.
				// 'SetObjectPropertyValue_InContainer' �Լ��� �־��� ��ü(ItemShopWidget)�� Ư�� ��ü ����(ObjectProperty)�� ���� �������ش�.
				// ���⼭ ItemShopWidget ������ OwnerNPC ������ ���� NPC ��ü(this)�� �����ϴ� ������ �Ѵ�.
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

