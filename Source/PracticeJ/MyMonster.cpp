// Fill out your copyright notice in the Description page of Project Settings.

#include "MyMonster.h"
#include "MyAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "MyHpWidget.h"


// Sets default values
AMyMonster::AMyMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//HealthComponent 생성
	HealthComponent = CreateDefaultSubobject<UMyHealthComponent>(TEXT("HealthComponent"));
	
	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"));
	HealthBarWidgetComponent->SetupAttachment(GetMesh());
	HealthBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 220.0f));
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComponent->SetDrawSize(FVector2D(150.0f, 50.0f));
	HealthBarWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/*
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/WB_Hp.WB_Hp"));
	if (UI_HUD.Succeeded())
	{
		HealthBarWidgetComponent->SetWidgetClass(UI_HUD.Class);
	}*/
	
}

// Called when the game starts or when spawned
void AMyMonster::BeginPlay()
{
	Super::BeginPlay();

	

	if (HealthComponent)
	{
		HealthComponent->OnDamage.AddDynamic(this, &AMyMonster::HandleOnDamage);
		HealthComponent->OnDeath.AddDynamic(this, &AMyMonster::HandleOnDeath);
		UpdateHealthBar(); // 초기 체력바 업데이트
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("HEALTHCOMPONENT IS NULL"));
	}
	UE_LOG(LogTemp, Warning, TEXT("maxhealth: %d, curhealth: %f"), HealthComponent->MaxHealth, HealthComponent->CurrentHealth);

}

// Called every frame
void AMyMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AMyMonster::HandleOnDamage()
{
	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Monster took Damage!"));
	
	auto Anim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if(Anim) Anim->PlayHitMontage();
	UpdateHealthBar();
}

void AMyMonster::HandleOnDeath()
{
	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Monster died!!"));
	
	auto Anim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if (Anim) Anim->PlayDieMontage();
	//Destroy();
}

void AMyMonster::UpdateHealthBar()
{
	if (HealthBarWidgetComponent)
	{
		if (UMyHpWidget* HealthBarWidget = Cast<UMyHpWidget>(HealthBarWidgetComponent->GetUserWidgetObject()))
		{
			float HealthPercent = HealthComponent ? (HealthComponent->CurrentHealth / HealthComponent->MaxHealth) : 0.0f;
			HealthBarWidget->SetHealthPercent(HealthPercent);
		}
	}
}

// Called to bind functionality to input
/*void AMyMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}*/




