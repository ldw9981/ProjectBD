// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemSlotBase.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "BDGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Items/InventorySystem.h"
#include "Engine/StreamableManager.h"
#include "Basic/BasicPC.h"
#include "Basic/BasicCharacter.h"

void UItemSlotBase::NativeConstruct()
{
	ItemThumnail = Cast<UImage>(GetWidgetFromName(TEXT("ItemThumnail")));
	ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
	ItemCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemCount")));
	ItemButton = Cast<UButton>(GetWidgetFromName(TEXT("ItemButton")));

	if (ItemButton)
	{
		ItemButton->OnClicked.AddDynamic(this, &UItemSlotBase::OnClicked);
	}
}

//left Click
void UItemSlotBase::OnClicked()
{
	UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		if (GI->Inventory->UseItem(InventoryIndex))
		{
			//ABasicPC* PC = Cast<ABasicPC>(GetOwningPlayer());
			ABasicPC* PC = Cast<ABasicPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			if (PC)
			{
				PC->UpdateInventory();
			}
		}
	}
}

FReply UItemSlotBase::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		ABasicPC* PC = Cast<ABasicPC>(GetOwningPlayer());
		if (PC)
		{
			ABasicCharacter* Pawn = Cast<ABasicCharacter>(PC->GetPawn());
			if (Pawn)
			{
				Pawn->DropItem(InventoryIndex);
			}
		}
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

//Array -> UI ¿¬°á
void UItemSlotBase::SetItemData(FItemDataTable NewItemData, int NewInventoryIndex,int NewCount)
{
	ItemData = NewItemData;
	InventoryIndex = NewInventoryIndex;
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


