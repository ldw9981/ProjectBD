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

	TArray<class UItemSlotBase*> Slots;
	

	class UItemSlotBase* GetEmptySlot();

	void AllResetSlot();
};
