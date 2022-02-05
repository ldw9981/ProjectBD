// Fill out your copyright notice in the Description page of Project Settings.

#include "BulletActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstance.h"
#include "Basic/BasicCharacter.h"
#include "Basic/BulletDamageType.h"

// Sets default values
ABulletActor::ABulletActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Sphere);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));

	bReplicates = true;
}

// Called when the game starts or when spawned
void ABulletActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (UKismetSystemLibrary::IsDedicatedServer(GetWorld()))
	{
		//UE_LOG(LogClass, Warning, TEXT("ABulletActor::BeginPlay DedicatedServer %x"),this);
	}
	else
	{
		//UE_LOG(LogClass, Warning, TEXT("ABulletActor::BeginPlay Client %x"), this);		
	}	
}

// Called every frame
void ABulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABulletActor::S2A_HitEffectBlood_Implementation(FVector Point, FRotator Rotation)
{
	if (BloodEffect)
	{
		//HitEffect
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodEffect, Point, Rotation);
	}
}

void ABulletActor::S2A_HitEffectBlock_Implementation(FVector Point, FRotator Rotation)
{
	if (BulletDecal)
	{
		UDecalComponent* BulletDecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
			BulletDecal,
			FVector(5, 5, 5),
			Point,
			Rotation,
			10.0f
		);
		if (BulletDecalComponent)
		{
			BulletDecalComponent->SetFadeScreenSize(0.0001f);
		}		
	}

	//HitEffect
	if (HitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Point, Rotation);
	}
}

void ABulletActor::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (UKismetSystemLibrary::IsDedicatedServer(GetWorld()))
	{
		//UE_LOG(LogClass, Warning, TEXT("ABulletActor::OnHit DedicatedServer AActor %s"), *Other->GetName());

		if (Other->ActorHasTag(TEXT("Player")))
		{
			ABasicCharacter* BasicCharacter = Cast<ABasicCharacter>(GetOwner());
			if (BasicCharacter)
			{
				UE_LOG(LogClass, Warning, TEXT("Player."));
				//점 데미지
				UGameplayStatics::ApplyPointDamage(Other,
					Damage,
					BasicCharacter->GetActorLocation(),
					Hit,
					BasicCharacter->GetController(),
					BasicCharacter,
					UBulletDamageType::StaticClass()
				);

				S2A_HitEffectBlood(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}

		}
		else
		{
			S2A_HitEffectBlock(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
		}
	}
	else
	{
		//UE_LOG(LogClass, Warning, TEXT("ABulletActor::OnHit Client AActor %s"), *Other->GetName());
	}

	Destroy();
}

void ABulletActor::Destroyed()
{
	Super::Destroyed();
	/*
	if (UKismetSystemLibrary::IsDedicatedServer(GetWorld()))
	{
		UE_LOG(LogClass, Warning, TEXT("ABulletActor::Destroyed DedicatedServer"));
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("ABulletActor::Destroyed Client"));
	}
	*/
}
