// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_BattleCheck.h"
#include "Zombie/Zombie.h"
#include "Zombie/ZombieAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTService_BattleCheck::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	AZombie* Zombie = Cast<AZombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (Zombie)
	{
		AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Player")));
		float Distance = FVector::Distance(Zombie->GetActorLocation(),
			Player->GetActorLocation());
		if (Distance > Zombie->AttackRange)
		{
			Zombie->SetState(EZombieState::Chase);
		}
			
	}


}
