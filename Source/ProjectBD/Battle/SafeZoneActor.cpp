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
	
	SetRadius(InitialRadius);
}

// Called every frame
void ASafeZoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PhazeTime <= 0.0f || CurrentRadius <= 1.0f)
	{
		return;
	}

	ABasicPC* PC = Cast<ABasicPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PhazeTime -= DeltaTime;
	if (PC && PC->IsLocalPlayerController())
	{	
		PC->UI_SetPhazeTime(PhazeTime);
	}

	
	if (bZoneMove)
	{
		CurrentRadius = FMath::FInterpConstantTo(CurrentRadius, TargetRadius, DeltaTime, InterpSpeedRadius);
		CurrentCenter = FMath::VInterpConstantTo(CurrentCenter, TargetCenter, DeltaTime, InterpSpeedCenter);
		SetRadius(CurrentRadius);
		SetActorLocation(CurrentCenter);

		// 이동중일때 갱신
		
		if (PC && PC->IsLocalPlayerController())
		{
			float Progress = 1 - ((CurrentRadius - TargetRadius)/ DifferenceRadius);
			PC->UI_SetSafeZoneProgress(Progress);
		}
	}

	if (PhazeTime <= 0.0f)
	{
		PhazeTime = 0;

		if (GetWorld()->IsServer())
		{
			if (!bZoneMove)
			{
				float PrevRadius = CurrentRadius;
				float NewRadius = CurrentRadius * 0.5f;
				FVector NewCenter = GetRandomLocationInRadius(CurrentCenter, PrevRadius-NewRadius);

				S2A_SetZoneMovePhaze(60, NewCenter, NewRadius);
			}
			else
			{
				S2A_SetWaitPhaze(60);
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


		FVector ActorLocationXY = Character->GetActorLocation();
		ActorLocationXY.Z = 0;
		FVector ZoneLocationXY = GetActorLocation();
		ZoneLocationXY.Z = 0;
		
		if (FVector(ActorLocationXY - ZoneLocationXY).Size() <= CurrentRadius)
		{
			continue;
		}

		UGameplayStatics::ApplyDamage(Character, 1, nullptr, this, UBulletDamageType::StaticClass());
	}

}



void ASafeZoneActor::S2A_SetZoneMovePhaze_Implementation(float NewPageTime, FVector NewCenter, float NewTargetRadius)
{
	bZoneMove = true;
	PhazeTime = NewPageTime;
	TargetRadius = NewTargetRadius;
	TargetCenter = NewCenter;
	
	DifferenceRadius = (CurrentRadius - TargetRadius);
	InterpSpeedRadius = DifferenceRadius / PhazeTime;		// 1초에 줄어야할 반지름 크기
	//UE_LOG(LogClass, Warning, TEXT(" %f, %f , %f"), CurrentRadius, TargetRadius, PhazeTime);
	InterpSpeedCenter = FVector(TargetCenter - CurrentCenter).Size()/ PhazeTime;	//1초에 이동해야할 크기
//	UE_LOG(LogClass, Warning, TEXT("InterpSpeedRadius %f,InterpSpeedCenter %f"), InterpSpeedRadius, InterpSpeedCenter);

	
}

void ASafeZoneActor::S2A_SetWaitPhaze_Implementation(float NewPageTime)
{
	bZoneMove = false;
	PhazeTime = NewPageTime;

	ABasicPC* PC = Cast<ABasicPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && PC->IsLocalPlayerController())
	{
		PC->UI_SetSafeZoneProgress(0.0f);
	}
}

FVector ASafeZoneActor::GetRandomLocationInRadius(const FVector & Origin,const float Radius)
{
	const float RandomAngle = 2.f * PI * FMath::FRand();
	const float U = FMath::FRand() + FMath::FRand();
	const float RandomRadius = Radius * (U > 1 ? 2.f - U : U);
	const FVector RandomOffset(FMath::Cos(RandomAngle) * RandomRadius, FMath::Sin(RandomAngle) * RandomRadius, 0);
	FVector RandomLocationInRadius = Origin + RandomOffset;
	return RandomLocationInRadius;
}

