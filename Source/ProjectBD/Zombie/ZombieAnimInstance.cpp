// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

void UZombieAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	AZombie* Pawn = Cast<AZombie>(TryGetPawnOwner());

	if (Pawn && Pawn->IsValidLowLevel())
	{
		CurrentState = Pawn->CurrentState;
		Speed = Pawn->GetCharacterMovement()->Velocity.Size();
	}
}

void UZombieAnimInstance::AnimNotify_NotifyTest(UAnimNotify * Notify)
{
	UE_LOG(LogClass, Warning, TEXT("NotiftTest"));
}
