// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BattleGS.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ABattleGS : public AGameStateBase
{
	GENERATED_BODY()
public:

	UPROPERTY(ReplicatedUsing="AliveCount_OnRep")
	int AliveCount = 0;

	UFUNCTION()
	void AliveCount_OnRep();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
