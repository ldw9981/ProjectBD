// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BasicAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UBasicAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsSprint = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsCrouched = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float AimPitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float AimYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsIronsight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsFalling = false;

	UFUNCTION()
	void AnimNotify_ReloadComplete(UAnimNotify* Notify);

	UFUNCTION()
	void BlendOut(UAnimMontage * Montage, bool bInterrupted);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsReload = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsFire = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bLeftLean = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bRightLean = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float CurrentAngle = 0;
};
