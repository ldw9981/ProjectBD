// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyWidgetBase.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby/LobbyPC.h"
#include "BDGameInstance.h"
#include "Lobby/LobbyGS.h"

void ULobbyWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	//PublicMessage = Cast<UTextBlock>(GetWidgetFromName(TEXT("PublicMessage")));
	AliveCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("AliveCount")));;
	ChatScroll = Cast<UScrollBox>(GetWidgetFromName(TEXT("ChatScroll")));;
	StartGameButton = Cast<UButton>(GetWidgetFromName(TEXT("StartGameButton")));;
	ChatInput = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ChatInput")));;

	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &ULobbyWidgetBase::StartGame);
	}

	if (ChatInput)
	{
		ChatInput->OnTextCommitted.AddDynamic(this, &ULobbyWidgetBase::OnTextCommittedProcess);
	}
}

void ULobbyWidgetBase::StartGame()
{
	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->StartGame();
	}
}

void ULobbyWidgetBase::AddMessage(FText NewMessage)
{
	if (ChatScroll)
	{
		UTextBlock* NewTextBlock = NewObject<UTextBlock>(ChatScroll);
		if (NewTextBlock)
		{
			NewTextBlock->SetText(NewMessage);
			NewTextBlock->Font.Size = 18;
			ChatScroll->AddChild(NewTextBlock);
			ChatScroll->ScrollToEnd();
		}
	}
}

void ULobbyWidgetBase::OnTextCommittedProcess(const FText & Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		if (!Text.IsEmpty())
		{
			UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			if (PC && GI)
			{
				FString Message = FString::Printf(TEXT("%s :  %s"), *GI->UserID, *Text.ToString());
				PC->C2S_SendChatMessage(FText::FromString(Message));
				ChatInput->SetText(FText::FromString(TEXT("")));
			}
		}
	}
	else if (CommitMethod == ETextCommit::OnCleared)
	{
		ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PC)
		{
			ChatInput->SetUserFocus(PC);
		}
	}
}

void ULobbyWidgetBase::ShowStartGameButton(bool bShow)
{
	if (StartGameButton)
	{
		if (bShow)
		{
			StartGameButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			StartGameButton->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void ULobbyWidgetBase::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//ALobbyGS* GS = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetWorld()));
	//if (GS)
	//{
	//	if (AliveCount)
	//	{
	//		AliveCount->SetText(FText::FromString(FString::Printf(TEXT("%d명 접속"), GS->AliveCount)));
	//	}
	//}
}

void ULobbyWidgetBase::UpdateAliveCount()
{
	ALobbyGS* GS = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		if (AliveCount)
		{
			AliveCount->SetText(FText::FromString(FString::Printf(TEXT("%d명 접속"), GS->AliveCount)));
		}
	}
}
