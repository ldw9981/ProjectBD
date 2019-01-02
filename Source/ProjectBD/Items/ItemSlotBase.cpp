// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemSlotBase.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "BDGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StreamableManager.h"
#include "Basic/BasicPC.h"
#include "Basic/BasicCharacter.h"
#include "Engine/Texture2D.h"

void UItemSlotBase::NativeConstruct()
{
	ItemThumnail = Cast<UImage>(GetWidgetFromName(TEXT("ItemThumnail")));
	ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
	ItemCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemCount")));
	ItemButton = Cast<UButton>(GetWidgetFromName(TEXT("ItemButton")));
}
FReply UItemSlotBase::NativeOnPreviewMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		OnClickedRButton.ExecuteIfBound(this);
		return FReply::Handled();
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		OnClickedLButton.ExecuteIfBound(this);
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

//Array -> UI 연결
void UItemSlotBase::SetItemData(FItemDataTable NewItemData, int NewInventoryIndex,int NewCount)
{
	ItemData = NewItemData;
	ArrayIndex = NewInventoryIndex;
	FStreamableManager Loader;
	if (ItemThumnail)
	{
		UTexture2D* Thumnail = Loader.LoadSynchronous<UTexture2D>(ItemData.ItemThumnail);
		if (Thumnail)
		{
			ItemThumnail->SetBrushFromTexture(Thumnail);
		}
	}

	if (ItemName)
	{
		ItemName->SetText(FText::FromString(ItemData.ItemName));
	}

	if (ItemCount)
	{
		ItemCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), NewCount)));
	}
	SetVisibility(ESlateVisibility::Visible);
}


