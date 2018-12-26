// Fill out your copyright notice in the Description page of Project Settings.

#include "Zombie.h"
#include "Components/SkeletalMeshComponent.h"
#include "Zombie/ZombieAIController.h"
#include "ConstructorHelpers.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Basic/BasicCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	static ConstructorHelpers::FClassFinder<AAIController> BP_ZombieAIController(TEXT("Blueprint'/Game/Blueprints/Zombie/BP_ZombieAIController.BP_ZombieAIController_C'"));
	if (BP_ZombieAIController.Succeeded())
	{
		//AIControllerClass = AZombieAIController::StaticClass();
		AIControllerClass = BP_ZombieAIController.Class;
	}

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSesing"));
	PawnSensing->SightRadius = 400.0f;
	PawnSensing->SetPeripheralVisionAngle(60);
	PawnSensing->HearingThreshold = 1000.0f;
	PawnSensing->SensingInterval = 0.1f;
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();

	PawnSensing->OnSeePawn.AddDynamic(this, &AZombie::OnSeePawn);
	PawnSensing->OnHearNoise.AddDynamic(this, &AZombie::OnHearNoise);

	CurrentHP = MaxHP;
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AZombie::OnSeePawn(APawn * Pawn)
{
	ABasicCharacter* Player = Cast<ABasicCharacter>(Pawn);
	if (Player && !Player->IsDead())
	{
		AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
		if (AIC && AIC->BBComponent && CurrentState == EZombieState::Normal)
		{
			SetState(EZombieState::Chase);
			AIC->BBComponent->SetValueAsObject(FName(TEXT("Player")), Pawn);

			UE_LOG(LogClass, Warning, TEXT("See %s"), *(Pawn->GetName()));
		}
	}
}

void AZombie::SetState(EZombieState NewState)
{
	CurrentState = NewState;
	AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
	if (AIC && AIC->BBComponent)
	{
		AIC->BBComponent->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)CurrentState);
	}
}

void AZombie::OnHearNoise(APawn * Pawn, const FVector & Location, float Volume)
{
}

bool AZombie::SetSpeed(EZombieState NewState)
{
	switch (CurrentState)
	{
		case EZombieState::Normal:
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			return true;
		case EZombieState::Chase:
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
			return true;
		default:
			GetCharacterMovement()->MaxWalkSpeed = 0;
			return true;
	}

	return false;
}

float AZombie::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (CurrentHP <= 0)
	{
		return 0;
	}

	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID)) //범위데미지
	{
		UE_LOG(LogClass, Warning, TEXT("Radial Damage %f"), DamageAmount);
	}
	else if (DamageEvent.IsOfType(FPointDamageEvent::ClassID)) //점데미지
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);

		UE_LOG(LogClass, Warning, TEXT("Point Damage %f %s"), DamageAmount, *(PointDamageEvent->HitInfo.BoneName.ToString()));

		if (PointDamageEvent->HitInfo.BoneName.Compare(TEXT("head")) == 0)
		{
			CurrentHP = 0;
		}
		else
		{
			CurrentHP -= DamageAmount;
		}
	}
	else if (DamageEvent.IsOfType(FDamageEvent::ClassID)) //일반데미지
	{
		UE_LOG(LogClass, Warning, TEXT("Damage %f"), DamageAmount);
		CurrentHP -= DamageAmount;
	}

	if (CurrentHP <= 0)
	{
		CurrentHP = 0;
		SetState(EZombieState::Dead);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetLifeSpan(10.0f);
	}

	return DamageAmount;
}

