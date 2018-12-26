// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UTitleWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UEditableTextBox* UserID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UEditableTextBox* Password;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UEditableTextBox* ServerIP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UButton* MakeRoomButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UButton* ConnectGameButton;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void MakeRoom();

	UFUNCTION()
	void ConnectGame();

	void SetUserID(FString NewUserID);
};
