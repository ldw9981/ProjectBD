// Fill out your copyright notice in the Description page of Project Settings.
#include "BasicPC.h"
#include "Basic/BasicPlayerCameraManager.h"
#include "UI/Items/ItemToolTipBase.h"
#include "Components/TextBlock.h"
#include "Items/InventoryBase.h"
#include "Kismet/GameplayStatics.h"
#include "BDGameInstance.h"
#include "Items/ItemSlotBase.h"
#include "Battle/BattleWidgetBase.h"
#include "BDGameInstance.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Basic/InventoryComponent.h"
#include "Basic/BasicCharacter.h"
#include "Items/MasterItem.h"
#include "Items/RandomItemSpawner.h"
#include "Engine/GameEngine.h"
#include "Components/SkeletalMeshComponent.h"

ABasicPC::ABasicPC()
{
	PlayerCameraManagerClass = ABasicPlayerCameraManager::StaticClass();
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	bReplicates = true;
}

void ABasicPC::BeginPlay()
{
	Super::BeginPlay();
	SetItemSpawner();
	
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
	if (!IsLocalPlayerController())
	{
		return;
	}

	InventoryWidget->AllResetSlotInventory();
	InventoryWidget->AllResetSlotPickable();
//	GetOwner();
	ABasicCharacter* BasicCharacter = Cast<ABasicCharacter>(GetPawn());
	if(!BasicCharacter)
	{
		return;
	}
	
	UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	for (int i = 0; i < Inventory->ItemList.Num(); ++i)
	{
		UItemSlotBase* Slot = InventoryWidget->GetEmptySlotInventory();
		if (Slot)
		{
			int ItemIndex = Inventory->GetItemIndex(i);
			int ItemCount = Inventory->GetItemCount(i);
			Slot->SetItemData(GI->GetItemData(ItemIndex), i, ItemCount);
		}
		else
		{
			//인벤토리 풀
		}
	}


	for (int i = 0; i < InteractionItemList.Num(); ++i)
	{
		UItemSlotBase* Slot = InventoryWidget->GetEmptySlotPickable();
		if (Slot)
		{
			int ItemIndex = InteractionItemList[i]->ItemIndex;
			int ItemCount = InteractionItemList[i]->ItemCount;
			Slot->SetItemData(GI->GetItemData(ItemIndex), i, ItemCount);
		}
		else
		{
			//인벤토리 풀
		}
	}


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

void ABasicPC::S2A_DropItemAll_Implementation()
{
	ABasicCharacter* BasicCharacter = Cast<ABasicCharacter>(GetPawn());
	if (!BasicCharacter)
	{
		return;
	}

	int ItemNum = Inventory->ItemList.Num();
	for (int i = 0; i < ItemNum; i++)
	{
		int ItemIndex = Inventory->GetItemIndex(0);
		int ItemCount = Inventory->GetItemCount(0);
		if (ItemIndex == -1)
		{
			continue;
		}

		if (!Inventory->DropItem(0))
		{
			continue;
		}
		AMasterItem* Item = RandomItemSpawner->SpawnMasterItem(ItemIndex,ItemCount);
		Item->SetActorLocation(BasicCharacter->GetMesh()->GetComponentLocation() + BasicCharacter->GetActorForwardVector() * 30.0f);
	}
}

void ABasicPC::SetItemSpawner()
{
	if (RandomItemSpawner != nullptr)
	{
		return;
	}
	TSubclassOf<ARandomItemSpawner> ClassType = ARandomItemSpawner::StaticClass();
	TArray<AActor*> Results;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassType, Results);
	check(Results.Num() == 1);
	RandomItemSpawner = Cast<ARandomItemSpawner>(Results[0]);
}

void ABasicPC::AddInteraction(AMasterItem* Item)
{
	if (!IsLocalController())
	{
		return;
	}

	InteractionItemList.Add(Item);
	GetWorld()->GetTimerManager().ClearTimer(ItemCheckHandle);
	GetWorld()->GetTimerManager().SetTimer(
		ItemCheckHandle,
		this,
		&ABasicPC::CheckItem,
		0.1f,
		true,
		0);

	UpdateInventory();
}


void ABasicPC::RemoveInteraction(AMasterItem* Item)
{
	if (!IsLocalController())
	{
		return;
	}

	InteractionItemList.Remove(Item);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%d"), InteractionItemList.Num()));

	if (InteractionItemList.Num() > 0)
	{
		FVector Location = GetSightLocation();

		int Index = GetClosestItem(Location);

		if (Index != -1)
		{
			SetItemToolTipName(InteractionItemList[Index]->ItemData.ItemName);
			ShowItemToolTip(true);			
		}
	}
	else
	{
		ShowItemToolTip(false);		
		GetWorld()->GetTimerManager().ClearTimer(ItemCheckHandle);	
	}

	UpdateInventory();
}


//나랑 제일 가까운 아이템 인덱스 가져오기
int ABasicPC::GetClosestItem(FVector SightLocation)
{
	float Min = 99999999999.9f;
	int Index = -1;
	for (int i = 0; i < InteractionItemList.Num(); ++i)
	{
		if (!InteractionItemList[i]->IsValidLowLevel() || InteractionItemList[i]->IsPendingKill())
			continue;

		float Distance = FVector::Distance(SightLocation, InteractionItemList[i]->GetActorLocation());
		if (Min > Distance)
		{
			Min = Distance;
			Index = i;
		}
	}

	return Index;
}

FVector ABasicPC::GetSightLocation()
{
	FVector CameraLocation;
	FRotator CameraRotation;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPlayerViewPoint(CameraLocation, CameraRotation);

	//화면 사이즈 구하기
	int SizeX;
	int SizeY;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetViewportSize(SizeX, SizeY);

	//화면상 2D 표적점을 3D 좌표 변환
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DeprojectScreenPositionToWorld(SizeX / 2, SizeY / 2, CrosshairWorldPosition, CrosshairWorldDirection);

	//광선 시작점과 끝 구하기
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = CameraLocation + (CrosshairWorldDirection * 900000.0f);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	FHitResult OutHit;
	bool bResult = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		TraceStart,
		TraceEnd,
		ObjectTypes,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		OutHit,
		true);

	FVector Result = FVector(0, 0, 0);

	if (bResult)
	{
		Result = OutHit.ImpactPoint;
	}

	return Result;

}

void ABasicPC::DropInventoryAllItem()
{
	ABasicCharacter* BasicCharacter = Cast<ABasicCharacter>(GetPawn());
	if (!BasicCharacter)
	{
		return;
	}

	TArray<FInventoryItemInfo> ItemList;
	ItemList = Inventory->ItemList;
	S2A_ClearInventory();
	for (auto it : ItemList)
	{
		RandomItemSpawner->Multicast_SpawnMasterItem(it.ItemIndex,it.ItemIndex,
			BasicCharacter->GetMesh()->GetComponentLocation() + BasicCharacter->GetActorForwardVector() * 30.0f);
	}
}

void ABasicPC::S2A_ClearInventory_Implementation()
{
	ABasicCharacter* BasicCharacter = Cast<ABasicCharacter>(GetPawn());
	if (!BasicCharacter)
	{
		return;
	}

	int ItemNum = Inventory->ItemList.Num();
	for (int i = 0; i < ItemNum; i++)
	{
		int ItemIndex = Inventory->GetItemIndex(0);
		int ItemCount = Inventory->GetItemCount(0);
		if (ItemIndex == -1)
		{
			continue;
		}

		if (!Inventory->DropItem(0))
		{
			continue;
		}
	}

	UpdateInventory();
}

void ABasicPC::InteractionClose()
{
	FVector Location = GetSightLocation();
	InteractionIndex(GetClosestItem(Location));
}

void ABasicPC::InteractionIndex(int InteractionIndex)
{
	if (InteractionItemList.Num() <= 0 || InteractionItemList.Num() <= InteractionIndex)
	{
		return;
	}
	AMasterItem* MasterItem = InteractionItemList[InteractionIndex];
	if (!MasterItem || MasterItem->IsPendingKill())
	{
		return;
	}
	C2S_Interaction(MasterItem->ItemSpawnID);
}

bool ABasicPC::C2S_Interaction_Validate(int ItemSpwanID)
{
	return true;
}

void ABasicPC::C2S_Interaction_Implementation(int ItemSpwanID)
{
	AMasterItem* MasterItem = RandomItemSpawner->GetMasterItem(ItemSpwanID);
	if (!MasterItem || MasterItem->IsPendingKill())
	{
		return;
	}

	if (!Inventory->CheckAdd(MasterItem->ItemIndex, MasterItem->ItemCount))
	{
		return;
	}
	UE_LOG(LogClass, Warning, TEXT(__FUNCTION__));
	// 서버업데이트
	Inventory->AddItem(MasterItem->ItemIndex, MasterItem->ItemCount);

	// 리플리케이션을 쓰면 모든 클라이언트에 인벤정보가 보내지므로 사용하지않는다.
	S2C_AddToInventory(MasterItem->ItemSpawnID, MasterItem->ItemIndex, MasterItem->ItemCount);

	RandomItemSpawner->Multicast_DestroyMasterItem(MasterItem->ItemSpawnID);
}
void ABasicPC::S2C_AddToInventory_Implementation(int ItemSpwanID, int ItemIndex, int ItemCount)
{
	Inventory->AddItem(ItemIndex, ItemCount);
	UpdateInventory();
}

void ABasicPC::DropItem(int InventoryIndex)
{
	int ItemIndex = Inventory->GetItemIndex(InventoryIndex);
	int ItemCount = Inventory->GetItemCount(InventoryIndex);
	if (ItemIndex == -1)
	{
		return;
	}

	if (!Inventory->DropItem(InventoryIndex))
	{
		return;
	}

	ABasicPC* PC = Cast<ABasicPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->UpdateInventory();
	}

	C2S_DropItem(InventoryIndex);
}

bool ABasicPC::UseItem(int InventoryIndex)
{
	return Inventory->UseItem(InventoryIndex);
}


bool ABasicPC::C2S_DropItem_Validate(int InventoryIndex)
{
	return true;
}

void ABasicPC::C2S_DropItem_Implementation(int InventoryIndex)
{
	ABasicCharacter* BasicCharacter = Cast<ABasicCharacter>(GetPawn());
	if (!BasicCharacter)
	{
		return;
	}

	int ItemIndex = Inventory->GetItemIndex(InventoryIndex);
	int ItemCount = Inventory->GetItemCount(InventoryIndex);
	if (ItemIndex == -1)
	{
		return;
	}

	if (!Inventory->DropItem(InventoryIndex))
	{
		return;
	}
	RandomItemSpawner->Multicast_SpawnMasterItem(ItemIndex,
		ItemCount,
		BasicCharacter->GetMesh()->GetComponentLocation() + BasicCharacter->GetActorForwardVector() * 30.0f);
}

//동기화 안함. 각 클라이언트의 월드상의 아이템과 시야기준으로 툴팁 표시 
void ABasicPC::CheckItem()
{
	if (InteractionItemList.Num() > 0)
	{
		FVector Location = GetSightLocation();

		int Index = GetClosestItem(Location);

		if (Index != -1)
		{
			SetItemToolTipName(InteractionItemList[Index]->ItemData.ItemName);
			ShowItemToolTip(true);			
		}
	}
}
void ABasicPC::SetHPBar(float NewHP)
{	
	if (IsLocalPlayerController() && BattleWidget)
	{
		BattleWidget->HpBarData = NewHP;
	}
}
