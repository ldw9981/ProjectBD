// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API AZombieAIController : public AAIController
{
	GENERATED_BODY()
public:
	int a;
	AZombieAIController();

	virtual void Possess(APawn* InPawn) override;
	virtual void UnPossess() override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTreeComponent* BTComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UBlackboardComponent* BBComponent;
};
