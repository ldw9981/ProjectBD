// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryBase.h"
#include "Components/ScrollBox.h"
#include "Items/ItemSlotBase.h"
#include "Basic/BasicPC.h"
#include "Basic/BasicCharacter.h"

void UInventoryBase::NativeConstruct()
{
	UScrollBox* InventoryScroll = Cast<UScrollBox>(GetWidgetFromName(TEXT("InventoryScroll")));
	if (InventoryScroll)
	{
		for (int i = 0; i < InventoryScroll->GetChildrenCount(); ++i)
		{
			UItemSlotBase* ItemSlotBase = Cast<UItemSlotBase>(InventoryScroll->GetChildAt(i));
			if (ItemSlotBase)
			{
				InventorySlots.Add(ItemSlotBase);
				ItemSlotBase->OnClickedLButton.BindUObject(this, &UInventoryBase::OnClickedLButtonInventory);
				ItemSlotBase->OnClickedRButton.BindUObject(this, &UInventoryBase::OnClickedRButtonInventory);

			}
		}
	}

	UScrollBox* PickableScroll = Cast<UScrollBox>(GetWidgetFromName(TEXT("PickableScroll")));
	if (PickableScroll)
	{
		for (int i = 0; i < PickableScroll->GetChildrenCount(); ++i)
		{
			UItemSlotBase* ItemSlotBase = Cast<UItemSlotBase>(PickableScroll->GetChildAt(i));
			if (ItemSlotBase)
			{
				PickableSlots.Add(ItemSlotBase);
				ItemSlotBase->OnClickedLButton.BindUObject(this, &UInventoryBase::OnClickedLButtonPickable);
				ItemSlotBase->OnClickedRButton.BindUObject(this, &UInventoryBase::OnClickedRButtonPickable);
			}
		}
	}

	//인벤토리 자료 초기화(UI)
	AllResetSlotInventory();
	AllResetSlotPickable();
}

UItemSlotBase* UInventoryBase::GetEmptySlotInventory()
{
	for (auto ItemSlotBase : InventorySlots)
	{
		if (ItemSlotBase->ArrayIndex == -1)
		{
			return ItemSlotBase;
		}
	}

	return nullptr;
}

void UInventoryBase::AllResetSlotInventory()
{
	for (auto ItemSlotBase : InventorySlots)
	{
		ItemSlotBase->SetVisibility(ESlateVisibility::Collapsed);
		ItemSlotBase->ArrayIndex = -1;
	}
}

UItemSlotBase* UInventoryBase::GetEmptySlotPickable()
{
	for (auto ItemSlotBase : PickableSlots)
	{
		if (ItemSlotBase->ArrayIndex == -1)
		{
			return ItemSlotBase;
		}
	}

	return nullptr;
}

void UInventoryBase::AllResetSlotPickable()
{
	for (auto ItemSlotBase : PickableSlots)
	{
		ItemSlotBase->SetVisibility(ESlateVisibility::Collapsed);
		ItemSlotBase->ArrayIndex = -1;
	}
}

void UInventoryBase::OnClickedLButtonInventory(UItemSlotBase * ItemSlotBase)
{
	UE_LOG(LogClass, Warning, TEXT(__FUNCTION__));
	ABasicPC* PC = Cast<ABasicPC>(GetOwningPlayer());
	if (PC)
	{
		ABasicCharacter* Pawn = Cast<ABasicCharacter>(PC->GetPawn());
		if (Pawn)
		{
			Pawn->UseItem(ItemSlotBase->ArrayIndex);
		}
	}
}

void UInventoryBase::OnClickedRButtonInventory(UItemSlotBase * ItemSlotBase)
{
	UE_LOG(LogClass, Warning, TEXT(__FUNCTION__));

	ABasicPC* PC = Cast<ABasicPC>(GetOwningPlayer());
	if (PC)
	{
		ABasicCharacter* Pawn = Cast<ABasicCharacter>(PC->GetPawn());
		if (Pawn)
		{
			Pawn->DropItem(ItemSlotBase->ArrayIndex);
		}
	}
}

void UInventoryBase::OnClickedLButtonPickable(UItemSlotBase * ItemSlotBase)
{
	UE_LOG(LogClass, Warning, TEXT(__FUNCTION__));
}

void UInventoryBase::OnClickedRButtonPickable(UItemSlotBase * ItemSlotBase)
{
	UE_LOG(LogClass, Warning, TEXT(__FUNCTION__));
	ABasicPC* PC = Cast<ABasicPC>(GetOwningPlayer());
	if (PC)
	{
		ABasicCharacter* Pawn = Cast<ABasicCharacter>(PC->GetPawn());
		if (Pawn)
		{
			Pawn->InteractionIndex(ItemSlotBase->ArrayIndex);
		}
	}
}
