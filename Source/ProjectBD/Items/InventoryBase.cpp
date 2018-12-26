// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryBase.h"
#include "Components/ScrollBox.h"
#include "Items/ItemSlotBase.h"

void UInventoryBase::NativeConstruct()
{
	UScrollBox* Scroll = Cast<UScrollBox>(GetWidgetFromName(TEXT("InventoryScroll")));
	if (Scroll)
	{
		for (int i = 0; i < Scroll->GetChildrenCount(); ++i)
		{
			UItemSlotBase* Slot = Cast<UItemSlotBase>(Scroll->GetChildAt(i));
			if (Slot)
			{
				Slots.Add(Slot);
			}
		}
	}

	//인벤토리 자료 초기화(UI)
	AllResetSlot();
}

UItemSlotBase* UInventoryBase::GetEmptySlot()
{
	for (auto Slot : Slots)
	{
		if (Slot->InventoryIndex == -1)
		{
			return Slot;
		}
	}

	return nullptr;
}

void UInventoryBase::AllResetSlot()
{
	for (auto Slot : Slots)
	{
		Slot->SetVisibility(ESlateVisibility::Collapsed);
		Slot->InventoryIndex = -1;
	}
}
