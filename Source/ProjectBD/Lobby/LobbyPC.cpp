// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyPC.h"
#include "Lobby/LobbyWidgetBase.h"
#include "Kismet/GameplayStatics.h"

void ALobbyPC::BeginPlay()
{
	Super::BeginPlay();

	if (LobbyWidgetClass && IsLocalPlayerController())
	{
		LobbyWidget = CreateWidget<ULobbyWidgetBase>(this, LobbyWidgetClass);
		if (LobbyWidget)
		{
			LobbyWidget->AddToViewport();
			bShowMouseCursor = true;
			SetInputMode(FInputModeGameAndUI());

			if (HasAuthority())
			{
				LobbyWidget->ShowStartGameButton(true);
			}
			else
			{
				LobbyWidget->ShowStartGameButton(false);
			}
		}
	}
}

void ALobbyPC::S2C_AddMessage_Implementation(const FText& NewMessage)
{
	LobbyWidget->AddMessage(NewMessage);
}

bool ALobbyPC::C2S_SendChatMessage_Validate(const FText& NewMessage)
{
	return true;
}

void ALobbyPC::C2S_SendChatMessage_Implementation(const FText & NewMessage)
{
	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		ALobbyPC* PC = Cast<ALobbyPC>(*Iter);
		if (PC)
		{
			PC->S2C_AddMessage(NewMessage);
		}
	}
}

void ALobbyPC::StartGame()
{
	if (HasAuthority())
	{
		 GetWorld()->ServerTravel(TEXT("Battle"));
	}
}
