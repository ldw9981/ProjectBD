// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ChangeSpeed.h"
#include "Zombie/ZombieAIController.h"
#include "Zombie/Zombie.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_ChangeSpeed::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AZombieAIController* AIC = Cast<AZombieAIController>(OwnerComp.GetAIOwner());

	if (AIC)
	{
		AZombie* Pawn = Cast<AZombie>(AIC->GetPawn());
		if (Pawn)
		{
			EZombieState CurrentState = (EZombieState)(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(GetSelectedBlackboardKey()));

			Pawn->SetSpeed(CurrentState);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
