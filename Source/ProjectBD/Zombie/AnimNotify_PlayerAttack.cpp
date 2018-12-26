// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify_PlayerAttack.h"
#include "Components/SkeletalMeshComponent.h"
#include "Zombie/Zombie.h"
#include "Zombie/ZombieAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotify_PlayerAttack::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	AZombie* Zombie = Cast<AZombie>(MeshComp->GetOwner());
	if (Zombie)
	{
		AZombieAIController* AIC = Cast<AZombieAIController>(Zombie->GetController());
		if (AIC)
		{
			AActor* Player = Cast<AActor>(AIC->BBComponent->GetValueAsObject(FName("Player")));
			if (Player)
			{
				UGameplayStatics::ApplyDamage(Player,
					30.0f,
					AIC,
					Zombie,
					nullptr
				);
			}
		}
	}
}
