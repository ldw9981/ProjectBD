// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState_ZombieWalkSpeed.h"
#include "Zombie/Zombie.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimNotifyState_ZombieWalkSpeed::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	AZombie* Zombie = Cast<AZombie>(MeshComp->GetOwner());
	if (Zombie)
	{
		Zombie->GetCharacterMovement()->MaxWalkSpeed = SlowSpeed;
	}
}

void UAnimNotifyState_ZombieWalkSpeed::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
}

void UAnimNotifyState_ZombieWalkSpeed::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	AZombie* Zombie = Cast<AZombie>(MeshComp->GetOwner());
	if (Zombie)
	{
		Zombie->GetCharacterMovement()->MaxWalkSpeed = Zombie->WalkSpeed;
		if (Zombie->CurrentState == EZombieState::Chase)
		{
			Zombie->GetCharacterMovement()->MaxWalkSpeed = Zombie->RunSpeed;
		}
	}

}
