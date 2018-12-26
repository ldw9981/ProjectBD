// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGM.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ALobbyGM : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void BeginPlay() override;

	FTimerHandle LeftTimerHandle;

	UFUNCTION()
	void LeftTimeFunc();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int LeftTime = 10;

	virtual void Logout(AController* Exiting) override;

};
