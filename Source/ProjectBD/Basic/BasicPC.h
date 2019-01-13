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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UInventoryComponent* Inventory;
	class UItemToolTipBase* ItemToolTip;
	class UInventoryBase* InventoryWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainWidget")
	TSubclassOf<class UBattleWidgetBase> BattleWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainWidget")
	class UBattleWidgetBase* BattleWidget;

	FString UserID;
	class ARandomItemSpawner*  RandomItemSpawner = nullptr;
	TArray<class AMasterItem*> InteractionItemList;		// 동기화 안함 클라이언트에서만 사용
	FTimerHandle ItemCheckHandle;
	bool bShowInventory=false;
private:
	void SetItemSpawner();
public:
	virtual void BeginPlay() override;

	void ShowItemToolTip(bool bShow);

	void SetItemToolTipName(FString ItemName);
	bool IsShowInventory();
	void UpdateInventory();



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

	UFUNCTION(NetMulticast, Reliable)
	void S2A_DropItemAll();
	void S2A_DropItemAll_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_ClearInventory();
	void S2A_ClearInventory_Implementation();


	void InteractionClose();
	void InteractionIndex(int InteractionIndex);

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_Interaction(int ItemSpwanID);
	bool C2S_Interaction_Validate(int ItemSpwanID);
	void C2S_Interaction_Implementation(int ItemSpwanID);

	UFUNCTION(Client, Reliable)
	void S2C_AddToInventory(int ItemSpwanID, int ItemIndex, int ItemCount);
	void S2C_AddToInventory_Implementation(int ItemSpwanID, int ItemIndex, int ItemCount);

	UFUNCTION()
	void CheckItem();

	void SetHPBar(float NewHP);

	void ToggleInventory();
	void DropItem(int InventoryIndex);
	bool UseItem(int InventoryIndex);

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_DropItem(int InventoryIndex);
	bool C2S_DropItem_Validate(int InventoryIndex);
	void C2S_DropItem_Implementation(int InventoryIndex);


	//로컬 플레이어만 작동됨
	void AddInteraction(AMasterItem* Item);
	//로컬 플레이어만 작동됨
	void RemoveInteraction(AMasterItem* Item);


	int GetClosestItem(FVector SightLocation);
	FVector GetSightLocation();

	void DropInventoryByDeath();

	void SetBattleWidgetAliveCountText(int AliveCount);
};
