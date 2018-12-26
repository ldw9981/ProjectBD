// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPC.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ALobbyPC : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ULobbyWidgetBase> LobbyWidgetClass;

	virtual void BeginPlay() override;
	
	class ULobbyWidgetBase* LobbyWidget;

	UFUNCTION(Client, Reliable)
	void S2C_AddMessage(const FText& NewMessage);
	void S2C_AddMessage_Implementation(const FText& NewMessage);

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_SendChatMessage(const FText& NewMessage);
	bool C2S_SendChatMessage_Validate(const FText& NewMessage);
	void C2S_SendChatMessage_Implementation(const FText& NewMessage);

	void StartGame();

};
