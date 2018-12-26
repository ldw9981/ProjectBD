// Fill out your copyright notice in the Description page of Project Settings.

#include "TitleWidgetBase.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "BDGameInstance.h"


void UTitleWidgetBase::NativeConstruct()
{
	UserID = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("UserID")));
	Password = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("Password")));
	ServerIP = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ServerIP")));
	MakeRoomButton = Cast<UButton>(GetWidgetFromName(TEXT("MakeRoomButton")));
	ConnectGameButton = Cast<UButton>(GetWidgetFromName(TEXT("ConnectGameButton")));
	if (MakeRoomButton)
	{
		MakeRoomButton->OnClicked.AddDynamic(this, &UTitleWidgetBase::MakeRoom);
	}
	if (ConnectGameButton)
	{
		ConnectGameButton->OnClicked.AddDynamic(this, &UTitleWidgetBase::ConnectGame);
	}
}

void UTitleWidgetBase::MakeRoom()
{
	SetUserID(UserID->GetText().ToString());
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Lobby")), true, TEXT("listen"));
}

void UTitleWidgetBase::ConnectGame()
{
	FString IP = ServerIP->GetText().ToString();
	if (!IP.IsEmpty())
	{
		SetUserID(UserID->GetText().ToString());
		UGameplayStatics::OpenLevel(GetWorld(), FName(*IP));
	}
}

void UTitleWidgetBase::SetUserID(FString NewUserID)
{
	UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI && NewUserID.Len() > 0)
	{
		GI->UserID = NewUserID;
	}
}
