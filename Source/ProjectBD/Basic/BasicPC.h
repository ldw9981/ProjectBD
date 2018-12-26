// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasicPC.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ABasicPC : public APlayerController
{
	GENERATED_BODY()
public:
	ABasicPC();

	virtual void BeginPlay() override;

	void ShowItemToolTip(bool bShow);

	void SetItemToolTipName(FString ItemName);
	
	class UItemToolTipBase* ItemToolTip;
	
	class UInventoryBase* InventoryWidget;
	bool IsShowInventory();
	void ToggleInventory();
	void UpdateInventory();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MainWidget")
	TSubclassOf<class UBattleWidgetBase> BattleWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainWidget")
	class UBattleWidgetBase* BattleWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAlive = true;

	FString UserID;

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_SetUserID(const FString& NewUserID);
	bool C2S_SetUserID_Validate(const FString& NewUserID);
	void C2S_SetUserID_Implementation(const FString& NewUserID);


	void AllSendKillingMessage(const FString & Message);

	UFUNCTION(Client, Reliable)
	void S2C_SendKillingMessage(const FString & Message);
	void S2C_SendKillingMessage_Implementation(const FString & Message);

	UFUNCTION()
	void DeleteTopKillingMessage();
};
