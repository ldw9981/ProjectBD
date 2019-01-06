// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleGM.h"
#include "Engine/World.h"
#include "Basic/BasicPC.h"
#include "Basic/BasicCharacter.h"
#include "Battle/BattleGS.h"

void ABattleGM::BeginPlay()
{
	Super::BeginPlay();

	FGameModeEvents::GameModePostLoginEvent.AddUObject(this, &ABattleGM::LoginEvent);
}

void ABattleGM::LoginEvent(AGameModeBase* GameMode, APlayerController* NewPlayer)
{
	ReCountAliveCount();
}

bool ABattleGM::CheckFinish()
{
	int Count = ReCountAliveCount();

	UE_LOG(LogClass, Warning, TEXT("----------------- %d"), Count);


	if (Count == 1)
	{
		return true;
	}

	return false;
}

//Player 죽으면 호출 
int ABattleGM::ReCountAliveCount()
{
	int Count = 0;
	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		ABasicPC* PC = Cast<ABasicPC>(*Iter);
		if (!PC)
		{
			continue;
		}
		ABasicCharacter* Character = Cast<ABasicCharacter>(PC->GetPawn());
		if (!Character)
		{
			continue;
		}
		
		if (Character->IsDead())
		{
			continue;
		}		
		Count++;		
	}

	ABattleGS* GS = GetGameState<ABattleGS>();
	if (GS)
	{
		GS->AliveCount = Count;
		//For Server
		GS->AliveCount_OnRep();
	}

	return Count;
}