// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem.h"
#include "Items/MasterItem.h"
#include "Engine/World.h"
#include "Basic/BasicPC.h"
#include "Kismet/GameplayStatics.h"
#include "Basic/BasicCharacter.h"


bool UInventorySystem::AddItem(FItemDataTable Item)
{
	//인벤토리 풀 체크

	if (Item.IsOverlapItemType())
	{
		int Index = GetSameItemIndex(Item);
		if (Index == -1)
		{
			//신규 추가
			ItemList.Add(Item);
		}
		else
		{
			//갯수 증가
			ItemList[Index].ItemCount += Item.ItemCount;
		}
	}
	else
	{
		//신규 추가
		ItemList.Add(Item);
	}

	return true;
}

bool UInventorySystem::UseItem(int Index)
{
	if (Index == -1)
	{
		return false;
	}

	if (ItemList[Index].ItemCount == 0)
	{
		return false;
	}


	if (ItemList[Index].IsOverlapItemType())
	{
		//사용하는거
		ItemList[Index].ItemCount--;
		//효과 적용
		if (ItemList[Index].ItemCount == 0)
		{
			//사용 다한 아이템을 삭제
			ItemList.RemoveAt(Index);
		}
	}
	else
	{
		//장비
	}

	return true;
}


bool UInventorySystem::DropItem(int Index)
{
	if (Index == -1)
	{
		return false;
	}

	if (ItemList[Index].ItemCount == 0)
	{
		return false;
	}

	ItemList.RemoveAt(Index);
	return true;	
}

int UInventorySystem::GetSameItemIndex(FItemDataTable Item)
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
