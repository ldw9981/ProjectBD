// Fill out your copyright notice in the Description page of Project Settings.

#include "SafeZoneActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Basic/BasicPC.h"
#include "Basic/BasicCharacter.h"
#include "Basic/BulletDamageType.h"

// Sets default values
ASafeZoneActor::ASafeZoneActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	SetRootComponent(Mesh);

	bReplicates = true;
	
	TargetRadius = 1000;
	SetRadius(TargetRadius);
	SetActorLocation(FVector(0, 0, 0));
}

// Called when the game starts or when spawned
void ASafeZoneActor::BeginPlay()
{
	Super::BeginPlay();
	if (UKismetSystemLibrary::IsServer(GetWorld()))
	{
		//1초 주기
		GetWorld()->GetTimerManager().SetTimer(
			PainTimer,
			this,
			&ASafeZoneActor::PainOutside,
			1.0f,
			true,
			0);	
	}
		
}

// Called every frame
void ASafeZoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PhazeTime <= 0.0f || TargetRadius <= 1.0f)
	{
		return;
	}

	PhazeTime -= DeltaTime;
	if (bZoneMove)
	{
		CurrentRadius = FMath::FInterpTo(CurrentRadius, TargetRadius, DeltaTime, InterpSpeed);
		SetRadius(CurrentRadius);
	}

	if (PhazeTime <= 0.0f)
	{
		PhazeTime = 0;

		if (GetWorld()->IsServer())
		{
			if (!bZoneMove)
			{
				TargetRadius = TargetRadius * 0.5f;
				S2A_SetZoneMovePhaze(5, TargetRadius);				
			}
			else
			{
				S2A_SetWaitPhaze(5);
			}
		}
	}
}

void ASafeZoneActor::SetRadius(float NewRadius)
{
	FVector Scale(NewRadius*Scale1Unit, NewRadius*Scale1Unit, 100000.0f);
	Mesh->SetWorldScale3D(Scale);
	CurrentRadius = NewRadius;
}

void ASafeZoneActor::PainOutside()
{
	

	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		ABasicPC* PC = Cast<ABasicPC>(*Iter);
		if (!PC)
		{
			continue;
		}
		ABasicCharacter* Character = Cast<ABasicCharacter>(PC->GetPawn());
		if (!Character)
		{
			continue;
		}

		if (Character->IsDead())
		{
			continue;
		}
		
		
		if (FVector(Character->GetActorLocation() - GetActorLocation()).Size() <= CurrentRadius)
		{
			continue;
		}

		//점 데미지
		UGameplayStatics::ApplyDamage(Character, 10, nullptr, this, UBulletDamageType::StaticClass());
	}

}



void ASafeZoneActor::S2A_SetZoneMovePhaze_Implementation(float NewPageTime, float NewTargetRadius)
{
	bZoneMove = true;
	PhazeTime = NewPageTime;
	TargetRadius = NewTargetRadius;
	InterpSpeed = PhazeTime / TargetRadius;
}

void ASafeZoneActor::S2A_SetWaitPhaze_Implementation(float NewPageTime)
{
	bZoneMove = false;
	PhazeTime = NewPageTime;
}

