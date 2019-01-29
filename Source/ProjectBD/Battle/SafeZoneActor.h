// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SafeZoneActor.generated.h"

UCLASS()
class PROJECTBD_API ASafeZoneActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASafeZoneActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initial")
	float InitialRadius = 10000;

	float Scale1Unit = 0.02;
	float CurrentRadius = 0.0f;
	float TargetRadius = 0.0f;
	FVector TargetCenter;
	FVector CurrentCenter;
	float InterpSpeedRadius = 0.0f;
	float InterpSpeedCenter = 0.0f;
	float PhazeTime = 5.0f;
	bool bZoneMove = false;

	FTimerHandle PainTimer;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetRadius(float NewRadius);
	void PainOutside();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetZoneMovePhaze(float NewPageTime, FVector NewCenter, float NewTargetRadius);
	void S2A_SetZoneMovePhaze_Implementation(float NewPageTime, FVector NewCenter, float NewTargetRadius);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetWaitPhaze(float NewPageTime);
	void S2A_SetWaitPhaze_Implementation(float NewPageTime);

	FVector GetRandomLocationInRadius(const FVector & Origin, const float Radius);
};
