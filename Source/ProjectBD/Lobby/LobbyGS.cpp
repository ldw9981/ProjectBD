// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGS.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby/LobbyPC.h"
#include "Lobby/LobbyWidgetBase.h"

void ALobbyGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGS, AliveCount);
	DOREPLIFETIME(ALobbyGS, PublicMessage);

	//DOREPLIFETIME_CONDITION(ALobbyGS, AliveCount, COND_Custom);
	//SetCustomIsActiveOverride()
}

void ALobbyGS::AliveCount_OnRep()
{
	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		if (PC->LobbyWidget)
		{
			PC->LobbyWidget->UpdateAliveCount();
		}
	}
}

void ALobbyGS::PublicMessage_OnRep()
{
	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && PC->LobbyWidget)
	{
		PC->LobbyWidget->PublicMessageData = PublicMessage;
	}
}
