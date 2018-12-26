// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SelectRandomPatrolPoint.h"
#include "Zombie/AI/PatrolPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTTask_SelectRandomPatrolPoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	TArray<AActor*> OutActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APatrolPoint::StaticClass(), OutActor);

	if (OutActor.Num() > 0)
	{
		//랜덤 위치 설정
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), OutActor[FMath::RandRange(0, OutActor.Num() - 1)]);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
