// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHealthComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "MyHpWidget.h"


// Sets default values for this component's properties
UMyHealthComponent::UMyHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	/*HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"));
	
	if (AActor* Owner = this->GetOwner())
	{
		if (ACharacter* CharacterOwner = Cast<ACharacter>(Owner))
		{
			if (USkeletalMeshComponent* MeshComp = CharacterOwner->GetMesh())
			{
				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Setup Widget to Actors"));

				HealthBarWidgetComponent->SetupAttachment(MeshComp);
			}
		}

	}
	
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));  // 몬스터 머리 위 100 유닛
	HealthBarWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 방지
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT(" / Game / UI / WB_Hp.WB_Hp"));
	if (UI_HUD.Succeeded())
	{
		HealthBarWidgetComponent->SetWidgetClass(UI_HUD.Class);
		HealthBarWidgetComponent->SetDrawSize(FVector2D(150.0f, 50.0f));
	}
	//HealthBarWidgetComponent->SetDrawSize(FVector2D(200, 50)); // 위젯 크기 설정
	*/
}


// Called when the game starts
void UMyHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	
	//UpdateHealthBar();
}


void UMyHealthComponent::TakeDamage(float DamageAmount)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Damage function called"));
	
	UE_LOG(LogTemp, Warning, TEXT("damageamount: %f, curhealth: %f"), DamageAmount, CurrentHealth);
	if (DamageAmount <= 0.0f || CurrentHealth <= 0.0f)
	{
		return;
	}


	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);
	//UpdateHealthBar();

	OnDamage.Broadcast();

	if (CurrentHealth <= 0.0f)
	{
		OnDeath.Broadcast();
	}
}


/*void UMyHealthComponent::UpdateHealthBar()
{
	//if (UMyHpWidget* HealthBarWidget = Cast<UMyHpWidget>(HealthBarWidgetComponent->GetUserWidgetObject()))
	//{
	//	float HealthPercent = CurrentHealth / MaxHealth;
	//	HealthBarWidget->SetHealthPercent(HealthPercent);
	//}

	
	if (AActor* Owner = GetOwner())
	{
		if (ACharacter* CharacterOwner = Cast<ACharacter>(Owner))
		{
			if (UWidgetComponent* WidgetComp = CharacterOwner->FindComponentByClass<UWidgetComponent>())
			{
				if (UMyHpWidget* HealthBarWidget = Cast<UMyHpWidget>(WidgetComp->GetUserWidgetObject()))
				{
					float HealthPercent = CurrentHealth / MaxHealth;
					HealthBarWidget->SetHealthPercent(HealthPercent);
				}
			}
		}
	}
}*/


