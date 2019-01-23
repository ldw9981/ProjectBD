// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletActor.generated.h"

UCLASS()
class PROJECTBD_API ABulletActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect") 
	class UMaterialInstance*	BulletDecal;

	float Damage = 1.0f;
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(NetMulticast, Reliable)
	void S2A_HitEffectBlood(FVector Point, FRotator Rotation);
	void S2A_HitEffectBlood_Implementation(FVector Point, FRotator Rotation);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_HitEffectBlock(FVector Point, FRotator Rotation);
	void S2A_HitEffectBlock_Implementation(FVector Point, FRotator Rotation);

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void Destroyed() override;
};
