// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGS.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ALobbyGS : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(ReplicatedUsing = "AliveCount_OnRep")
	int AliveCount = 0;
	

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void AliveCount_OnRep();

	UPROPERTY(ReplicatedUsing = "PublicMessage_OnRep")
	FString PublicMessage;

	UFUNCTION()
	void PublicMessage_OnRep();

};
