// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "BDGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Basic/BasicCharacter.h"
#include "Basic/BasicPC.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, LoadedBullet);
	DOREPLIFETIME(UInventoryComponent, ExtraBullet);
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UInventoryComponent::AddItem(int ItemIndex, int ItemCount)
{
	//인벤토리 풀 체크
	UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	FItemDataTable& ItemData = GI->GetItemData(ItemIndex);
	FInventoryItemInfo InventoryItemInfo;
	InventoryItemInfo.ItemIndex = ItemIndex;
	InventoryItemInfo.ItemCount = ItemCount;

	if (ItemData.IsOverlapItemType())
	{
		int Index = GetSameItemIndex(ItemData);
		if (Index == -1)
		{
			//신규 추가			
			Index = ItemList.Add(InventoryItemInfo);
			switch (ItemData.ItemType)
			{
			case EItemType::Ammo:
				BulletIndex = Index;				
				break;
			case EItemType::Grenade:
				GrenadeIndex = Index;
				break;
			default:
				break;
			}			
		}
		else
		{
			//갯수 증가
			ItemList[Index].ItemCount += ItemCount;
		}
	}
	else
	{
		//신규 추가
		ItemList.Add(InventoryItemInfo);
	}
	return true;
}

bool UInventoryComponent::UseItem(int InventoryIndex)
{
	if (InventoryIndex == -1 || InventoryIndex >= ItemList.Num())
	{
		return false;
	}

	if (ItemList[InventoryIndex].ItemCount == 0)
	{
		return false;
	}


	UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	FItemDataTable& ItemData = GI->GetItemData(ItemList[InventoryIndex].ItemIndex);
	if (ItemData.IsOverlapItemType())
	{
		//사용하는거
		ItemList[InventoryIndex].ItemCount--;
		//효과 적용
		if (ItemList[InventoryIndex].ItemCount == 0)
		{
			//사용 다한 아이템을 삭제
			ItemList.RemoveAt(InventoryIndex);
		}
	}
	else
	{
		//장비
	}

	return true;
}


bool UInventoryComponent::DropItem(int Index)
{
	if (Index == -1 || Index >= ItemList.Num())
	{
		return false;
	}

	if (ItemList[Index].ItemCount == 0)
	{
		return false;
	}

	if (GrenadeIndex == Index)
	{
		GrenadeIndex = -1;		
	}
	else if (BulletIndex == Index)
	{
		BulletIndex = -1;
	}


	ItemList.RemoveAt(Index);
	return true;
}

bool UInventoryComponent::CheckAdd(int ItemIndex, int ItemCount)
{
	//인벤토리 풀 체크 일단 제한은 없다.
	//인벤토리 풀 체크
	UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	FItemDataTable& ItemData = GI->GetItemData(ItemIndex);
	FInventoryItemInfo InventoryItemInfo;
	InventoryItemInfo.ItemIndex = ItemIndex;
	InventoryItemInfo.ItemCount = ItemCount;

	if (ItemData.IsOverlapItemType())
	{
		int Index = GetSameItemIndex(ItemData);
		if (Index == -1)
		{
			//신규 추가

			//ItemList.Add(InventoryItemInfo);
		}
		else
		{
			//갯수 증가
			//ItemList[Index].ItemCount += ItemCount;
		}
	}
	else
	{
		// 겹치는 템이 아닌데 오른손 장비가 이미 있으면 실패
		if (ItemData.ItemEquipType == EEquipType::WeaponRightHand)
		{
			if (GetSameItemIndex(ItemData) != -1)
			{
				return false;
			}
		}
	}
	return true;
}

int UInventoryComponent::GetSameItemIndex(FItemDataTable Item)
{
	for (int i = 0; i < ItemList.Num(); ++i)
	{
		if (ItemList[i].ItemIndex == Item.ItemIndex)
		{
			return i;
		}
	}

	return -1;
}

int UInventoryComponent::GetItemIndex(int InventoryIndex)
{
	if (InventoryIndex >= ItemList.Num())
	{
		return -1;
	}
	return ItemList[InventoryIndex].ItemIndex;
}


int UInventoryComponent::GetItemCount(int InventoryIndex)
{
	if (InventoryIndex >= ItemList.Num())
	{
		return 0;
	}
	return ItemList[InventoryIndex].ItemCount;
}

void UInventoryComponent::ClearItem()
{
	ItemList.Reset();
}

FItemDataTable& UInventoryComponent::GetItemData(int InventoryIndex)
{
	UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	return GI->GetItemData(ItemList[InventoryIndex].ItemIndex);
}

bool UInventoryComponent::IsExistLoadedBullet()
{
	return LoadedBullet <= 0 ? false : true;
}

bool UInventoryComponent::IsExistExtraBullet()
{
	return ExtraBullet <= 0 ? false : true;
}

void UInventoryComponent::ReloadComplete()
{
	//총알이 없는거 
	if (!IsExistExtraBullet())
	{
		return;
	}

	//충전 되야 할 총알 수
	int AddBullet = BulletPerMagazine - LoadedBullet;

	//남은 총알이 충전 할 총알보다 작은 경우
	if (ExtraBullet <= AddBullet)
	{
		LoadedBullet += ExtraBullet;
		ExtraBullet = 0;
	}
	else
	{
		//남은 총알에서 충전 될 총알을 빼줌
		ExtraBullet -= AddBullet;
		LoadedBullet = BulletPerMagazine;
	}
}

void UInventoryComponent::S2A_UseBullet_Implementation()
{
	if (LoadedBullet > 0)
	{
		LoadedBullet--;
//		UE_LOG(LogClass, Warning, TEXT("Bullet %d %d"), LoadedBullet, ExtraBullet);
		DecreaseItem(BulletIndex);

		ABasicPC* BasicPC = Cast<ABasicPC>(GetOwner());
		if (BasicPC && GetWorld()->IsClient())
		{
			BasicPC->UI_UpdateBullet(LoadedBullet, ExtraBullet);
		}
	}
}

void UInventoryComponent::AddExtraBullet(int AddExtraBullet)
{
	ExtraBullet += AddExtraBullet;
}

void UInventoryComponent::OnRep_Bullet()
{
	if (!GetWorld()->IsClient())
	{
		return;
	}
	
	ABasicPC* BasicPC = Cast<ABasicPC>(GetOwner());
	if (!BasicPC)
	{
		return;
	}

	BasicPC->UI_UpdateBullet(LoadedBullet, ExtraBullet);
}

bool UInventoryComponent::DecreaseItem(int InventoryIndex)
{
	if (InventoryIndex == -1 || InventoryIndex >= ItemList.Num())
	{
		return false;
	}

	if (ItemList[InventoryIndex].ItemCount == 0)
	{
		return false;
	}

	ItemList[InventoryIndex].ItemCount--;
	if (ItemList[InventoryIndex].ItemCount == 0)
	{
		//사용 다한 아이템을 삭제
		ItemList.RemoveAt(InventoryIndex);
	}
	return true;
}