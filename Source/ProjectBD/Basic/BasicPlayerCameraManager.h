// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "BasicPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ABasicPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	ABasicPlayerCameraManager();
	virtual void BeginPlay() override;
	virtual void UpdateCamera(float DeltaTime) override;

	virtual void Tick(float DeltaTime) override;

	float NormalFOV = 90.0f;
	float IronsightFOV = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Curve")
	class UCurveFloat* FloatCurve;

	bool bIsOnIronsight;
	float TotalTime = 0;

	class UTimelineComponent* MyTimeline;

	UFUNCTION()
	void TimelineCallback(float val);

	UFUNCTION()
	void TimelineFinishedCallback();

	void PlayTimeline();

	float CurrentZ;

	FVector CurrentSpringArm;
};
