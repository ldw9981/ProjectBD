// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_CheckPlayer.h"
#include "Zombie/Zombie.h"
#include "Zombie/ZombieAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

EBTNodeResult::Type UBTTask_CheckPlayer::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AZombie* Zombie = Cast<AZombie>(OwnerComp.GetAIOwner()->GetPawn());
	AZombieAIController* AIC = Cast <AZombieAIController>(OwnerComp.GetAIOwner());
	if (Zombie && AIC)
	{
		//AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Player")));
		AActor* Player = Cast<AActor>(AIC->BBComponent->GetValueAsObject(TEXT("Player")));

		if (Player)
		{
			float Distance = FVector::Distance(Zombie->GetActorLocation(), Player->GetActorLocation());

			if (Distance > Zombie->PawnSensing->SightRadius)
			{
				Zombie->SetState(EZombieState::Normal);
				return EBTNodeResult::Failed;
			}
			else if(Distance <= Zombie->AttackRange)
			{
				Zombie->SetState(EZombieState::Battle);
				return EBTNodeResult::Failed;
			}
		}
	}

	return EBTNodeResult::Succeeded;
	
}
