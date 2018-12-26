// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicPC.h"
#include "Basic/BasicPlayerCameraManager.h"
#include "UI/Items/ItemToolTipBase.h"
#include "Components/TextBlock.h"
#include "Items/InventoryBase.h"
#include "Kismet/GameplayStatics.h"
#include "BDGameInstance.h"
#include "Items/InventorySystem.h"
#include "Items/ItemSlotBase.h"
#include "Battle/BattleWidgetBase.h"
#include "BDGameInstance.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Basic/InventoryComponent.h"

ABasicPC::ABasicPC()
{
	PlayerCameraManagerClass = ABasicPlayerCameraManager::StaticClass();
}

void ABasicPC::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		//UI 생성(UMG 블루프린트 클래스를 로딩, 생성)
		FStringClassReference ItemToolTipClass(TEXT("WidgetBlueprint'/Game/Blueprints/UI/ItemToolTip.ItemToolTip_C'"));

		//클래스 로딩 시도
		UClass* WidgetClass = ItemToolTipClass.TryLoadClass<UItemToolTipBase>();
		if (WidgetClass) //클래스 로딩 성공
		{
			//클래스를 인스턴스 만들기
			ItemToolTip = Cast<UItemToolTipBase>(CreateWidget<UUserWidget>(this, WidgetClass));
			//화면에 붙이기
			if (ItemToolTip)
			{
				ItemToolTip->AddToViewport();
				ItemToolTip->SetVisibility(ESlateVisibility::Collapsed);
			}
		}

		FStringClassReference InventoryClass(TEXT("WidgetBlueprint'/Game/Blueprints/UI/Inventory.Inventory_C'"));
		WidgetClass = InventoryClass.TryLoadClass<UInventoryBase>();
		if (WidgetClass)
		{
			InventoryWidget = Cast<UInventoryBase>(CreateWidget<UUserWidget>(this, WidgetClass));
			if (InventoryWidget)
			{
				InventoryWidget->AddToViewport();
				InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}

		if (BattleWidgetClass)
		{
			BattleWidget = CreateWidget<UBattleWidgetBase>(this, BattleWidgetClass);
			if (BattleWidget)
			{
				BattleWidget->AddToViewport();
			}
		}

		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;

		UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (GI)
		{
			UserID = GI->UserID;
			C2S_SetUserID(UserID);
		}
	}
}

void ABasicPC::ShowItemToolTip(bool bShow)
{
	if (!IsLocalPlayerController())
	{
		return;
	}

	if (ItemToolTip)
	{
		if (bShow)
		{
			ItemToolTip->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ItemToolTip->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

}

void ABasicPC::SetItemToolTipName(FString ItemName)
{
	if (IsLocalPlayerController())
	{
		ItemToolTip->ItemName->SetText(FText::FromString(ItemName));
	}
}

bool ABasicPC::IsShowInventory()
{
	if (!IsLocalPlayerController())
	{
		return false;
	}

	if (InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		return true;
	}
	return false;
}

void ABasicPC::ToggleInventory()
{
	if (InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		UpdateInventory();
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());
	}
}

//Data(GI->Inventory) - UI(Widget, InventoryBase, ItemSlotBase) 연결
void ABasicPC::UpdateInventory()
{
	InventoryWidget->AllResetSlot();

//	GetOwner();
	APawn* Pawn = GetPawn();
	if(!Pawn)
	{
		return;
	}
	UInventoryComponent* InventoryComponent = Cast<UInventoryComponent>( Pawn->GetComponentByClass(UInventoryComponent::StaticClass()));
	if (!InventoryComponent)
	{
		return;
	}


	UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	for (int i = 0; i < InventoryComponent->ItemList.Num(); ++i)
	{
		UItemSlotBase* Slot = InventoryWidget->GetEmptySlot();
		if (Slot)
		{
			int ItemIndex = InventoryComponent->GetItemIndex(i);
			int ItemCount = InventoryComponent->GetItemCount(i);
			Slot->SetItemData(GI->GetItemData(ItemIndex), i, ItemCount);
		}
		else
		{
			//인벤토리 풀
		}
	}
	/*
	UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		for (int i = 0; i < GI->Inventory->ItemList.Num(); ++i)
		{
			UItemSlotBase* Slot = InventoryWidget->GetEmptySlot();
			if (Slot)
			{
				Slot->SetItemData(GI->Inventory->ItemList[i], i);
			}
			else
			{
				//인벤토리 풀
			}
		}
	}
	*/

}

bool ABasicPC::C2S_SetUserID_Validate(const FString& NewUserID)
{
	return true;
}

void ABasicPC::C2S_SetUserID_Implementation(const FString& NewUserID)
{
	UserID = NewUserID;
}


void ABasicPC::AllSendKillingMessage(const FString& Message)
{
	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		ABasicPC* PC = Cast<ABasicPC>(*Iter);
		if (PC)
		{
			PC->S2C_SendKillingMessage(Message);
		}
	}
}

void ABasicPC::S2C_SendKillingMessage_Implementation(const FString& Message)
{
	if (BattleWidget)
	{
		BattleWidget->AddKillingMessage(Message);
		FTimerHandle DeleteTimer;
		GetWorld()->GetTimerManager().SetTimer(DeleteTimer,
			this,
			&ABasicPC::DeleteTopKillingMessage,
			3.0f);
	}
}

void ABasicPC::DeleteTopKillingMessage()
{
	if (BattleWidget)
	{
		BattleWidget->DeleteTopKillingMessage();
	}
}
