// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicAnimInstance.h"
#include "Basic/BasicCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Basic/BasicPC.h"
#include "Basic/WeaponComponent.h"
#include "Basic/InventoryComponent.h"

void UBasicAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ABasicCharacter* Pawn = Cast<ABasicCharacter>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		Speed = Pawn->GetCharacterMovement()->Velocity.Size();
		//Direction = CalculateDirection(Pawn->GetCharacterMovement()->Velocity,
		//	FRotator(0, Pawn->GetControlRotation().Yaw, 0));
		Direction = CalculateDirection(Pawn->GetCharacterMovement()->Velocity,
			Pawn->GetActorRotation());

		bIsSprint = Pawn->bIsSprint;
		bIsCrouched = Pawn->bIsCrouched;

		//총구 방향
		FRotator AimRotator = Pawn->GetAimOffset();
		AimPitch = AimRotator.Pitch;
		AimYaw = AimRotator.Yaw;

		bIsIronsight = Pawn->bIsIronsight;

		bIsFalling = Pawn->GetCharacterMovement()->IsFalling();

		bIsFire = Pawn->bIsFire;

		//재장전
		bIsReload = Pawn->bIsReload;
		if (bIsReload)
		{
			if (!Montage_IsPlaying(Pawn->ReloadAnimation))
			{				
				UE_LOG(LogClass, Warning, TEXT("Reload"));
				Montage_Play(Pawn->ReloadAnimation);
			}
		}

		//기울이기
		bLeftLean = Pawn->bLeftLean;
		bRightLean = Pawn->bRightLean;
		float TargetAngle = 0;
		if (bLeftLean)
		{
			TargetAngle = -Pawn->LeanAngle;
		}
		else if (bRightLean)
		{
			TargetAngle = Pawn->LeanAngle;
		}
		else if (bLeftLean && bRightLean)
		{
			TargetAngle = 0;
		}

		CurrentAngle = FMath::FInterpTo(CurrentAngle, TargetAngle, DeltaSeconds, 5.0f);

	}
}

void UBasicAnimInstance::AnimNotify_ReloadComplete(UAnimNotify* Notify)
{
	UE_LOG(LogClass, Warning, TEXT("Complete"));
	ABasicCharacter* Pawn = Cast<ABasicCharacter>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		if (GetWorld()->IsServer())
		{
			ABasicPC* BasicPC = Cast<ABasicPC>(Pawn->GetController());
			if (BasicPC)
			{
				BasicPC->Inventory->ReloadComplete();
			}
			
		}
		bIsReload = false; 
		Pawn->bIsReload = false;
	}
}
