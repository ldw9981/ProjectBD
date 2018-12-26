// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGM.h"
#include "Engine/World.h"
#include "Lobby/LobbyGS.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby/LobbyPC.h"
#include "Lobby/LobbyWidgetBase.h"
#include "TimerManager.h"

void ALobbyGM::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int Count = 0;
	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		Count++;
	}


	ALobbyGS* GS = GetGameState<ALobbyGS>();
	if (GS)
	{
		GS->AliveCount = Count;
		GS->AliveCount_OnRep(); //Server
	}
}

void ALobbyGM::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(LeftTimerHandle,
		this,
		&ALobbyGM::LeftTimeFunc,
		1.0f,
		true,
		1.0f);
}

void ALobbyGM::LeftTimeFunc()
{
	ALobbyGS* GS = GetGameState<ALobbyGS>();

	LeftTime--;

	if (LeftTime == 0)
	{
		if (GS)
		{
			GS->PublicMessage = FString::Printf(TEXT("게임을 시작합니다."));
		}
	}
	else if (LeftTime < 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(LeftTimerHandle);
		GetWorld()->ServerTravel(TEXT("Battle"));
	}
	else
	{
		if (GS)
		{
			GS->PublicMessage = FString::Printf(TEXT("%d초 후에 게임을 시작합니다."), LeftTime);
		}
	}

	GS->PublicMessage_OnRep(); //For Server
}

void ALobbyGM::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	int Count = 0;
	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		Count++;
	}

	Count--;

	ALobbyGS* GS = GetGameState<ALobbyGS>();
	if (GS)
	{
		GS->AliveCount = Count;
		GS->AliveCount_OnRep(); //Server
	}
}
