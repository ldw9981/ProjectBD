// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UInventoryBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	TArray<class UItemSlotBase*> InventorySlots;
	TArray<class UItemSlotBase*> PickableSlots;
	

	class UItemSlotBase* GetEmptySlotInventory();
	void AllResetSlotInventory();

	UItemSlotBase * GetEmptySlotPickable();
	void AllResetSlotPickable();

	void OnClickedLButtonInventory(UItemSlotBase* Slot);
	void OnClickedRButtonInventory(UItemSlotBase* Slot);

	void OnClickedLButtonPickable(UItemSlotBase* Slot);
	void OnClickedRButtonPickable(UItemSlotBase* Slot);
};
