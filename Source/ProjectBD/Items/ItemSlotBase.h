// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/ItemDataTable.h"
#include "ItemSlotBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UItemSlotBase : public UUserWidget
{
	GENERATED_BODY()
public:
	FItemDataTable ItemData;

	int InventoryIndex = -1;

	class UImage* ItemThumnail;
	class UTextBlock* ItemName;
	class UTextBlock* ItemCount;
	class UButton* ItemButton;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClicked();

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void SetItemData(FItemDataTable NewItemData, int NewInventoryIndex, int NewCount);
};
