// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_IsDead.h"
#include "Basic/BasicCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Zombie/Zombie.h"
#include "Zombie/ZombieAIController.h"

EBTNodeResult::Type UBTTask_IsDead::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ABasicCharacter* Player = Cast<ABasicCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));

	if (Player && Player->IsDead())
	{
		AZombie* Zombie = Cast<AZombie>(OwnerComp.GetAIOwner()->GetPawn());
		if (Zombie)
		{
			Zombie->SetState(EZombieState::Normal);
			return EBTNodeResult::Failed;
		}
	}

	return EBTNodeResult::Succeeded;
}
