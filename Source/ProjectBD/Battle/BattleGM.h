// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleGM.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ABattleGM : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	UFUNCTION()
	void LoginEvent(AGameModeBase * GameMode, APlayerController * NewPlayer);
	UFUNCTION()
	void LogoutEvent(AGameModeBase* GameMode, AController* Exiting);
	int CountConnection();
	int CountAlive();

	bool CheckFinish();
};
