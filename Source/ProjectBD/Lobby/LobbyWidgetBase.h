// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ULobbyWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//class UTextBlock* PublicMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTextBlock* AliveCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UScrollBox* ChatScroll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UButton* StartGameButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UEditableTextBox* ChatInput;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void StartGame();

	void AddMessage(FText NewMessage);

	UFUNCTION()
	void OnTextCommittedProcess(const FText& Text, ETextCommit::Type CommitMethod);

	void ShowStartGameButton(bool bShow);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void UpdateAliveCount();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString PublicMessageData;
};
