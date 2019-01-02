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
	DECLARE_DELEGATE_OneParam(FOnClickedCustom,UItemSlotBase*)

	FItemDataTable ItemData;

	int ArrayIndex = -1;

	class UImage* ItemThumnail;
	class UTextBlock* ItemName;
	class UTextBlock* ItemCount;
	class UButton* ItemButton;

	virtual void NativeConstruct() override;
	
	FOnClickedCustom OnClickedLButton;
	FOnClickedCustom OnClickedRButton;

	// NativeOnMouseButtonDown 는 마우스 왼쪽 버튼에 반응하지 않는다.그래서 아래함수를 재정의한다.
	// https://issues.unrealengine.com/issue/UE-49100
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void SetItemData(FItemDataTable NewItemData, int NewInventoryIndex, int NewCount);
};
