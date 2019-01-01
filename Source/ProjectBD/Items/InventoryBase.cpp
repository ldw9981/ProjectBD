// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryBase.h"
#include "Components/ScrollBox.h"
#include "Items/ItemSlotBase.h"

void UInventoryBase::NativeConstruct()
{
	UScrollBox* InventoryScroll = Cast<UScrollBox>(GetWidgetFromName(TEXT("InventoryScroll")));
	if (InventoryScroll)
	{
		for (int i = 0; i < InventoryScroll->GetChildrenCount(); ++i)
		{
			UItemSlotBase* Slot = Cast<UItemSlotBase>(InventoryScroll->GetChildAt(i));
			if (Slot)
			{
				InventorySlots.Add(Slot);
			}
		}
	}

	UScrollBox* PickableScroll = Cast<UScrollBox>(GetWidgetFromName(TEXT("PickableScroll")));
	if (PickableScroll)
	{
		for (int i = 0; i < PickableScroll->GetChildrenCount(); ++i)
		{
			UItemSlotBase* Slot = Cast<UItemSlotBase>(PickableScroll->GetChildAt(i));
			if (Slot)
			{
				PickableSlots.Add(Slot);
			}
		}
	}

	//인벤토리 자료 초기화(UI)
	AllResetSlotInventory();
	AllResetSlotPickable();
}

UItemSlotBase* UInventoryBase::GetEmptySlotInventory()
{
	for (auto Slot : InventorySlots)
	{
		if (Slot->InventoryIndex == -1)
		{
			return Slot;
		}
	}

	return nullptr;
}

void UInventoryBase::AllResetSlotInventory()
{
	for (auto Slot : InventorySlots)
	{
		Slot->SetVisibility(ESlateVisibility::Collapsed);
		Slot->InventoryIndex = -1;
	}
}

UItemSlotBase* UInventoryBase::GetEmptySlotPickable()
{
	for (auto Slot : PickableSlots)
	{
		if (Slot->InventoryIndex == -1)
		{
			return Slot;
		}
	}

	return nullptr;
}

void UInventoryBase::AllResetSlotPickable()
{
	for (auto Slot : PickableSlots)
	{
		Slot->SetVisibility(ESlateVisibility::Collapsed);
		Slot->InventoryIndex = -1;
	}
}
