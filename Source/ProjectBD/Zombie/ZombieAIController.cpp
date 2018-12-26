// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Zombie/Zombie.h"
#include "BehaviorTree/BehaviorTree.h"

AZombieAIController::AZombieAIController()
{
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComponent"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBComponent"));
}

void AZombieAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	AZombie* Zombie = Cast<AZombie>(InPawn);
	if (Zombie)
	{
		if (Zombie->BehaviorTree && Zombie->BehaviorTree->BlackboardAsset)
		{
			BBComponent->InitializeBlackboard(*(Zombie->BehaviorTree->BlackboardAsset));
			BTComponent->StartTree(*(Zombie->BehaviorTree));
		}
	}

	UE_LOG(LogClass, Warning, TEXT("Possess"));
}

void AZombieAIController::UnPossess()
{
	Super::UnPossess();
}
