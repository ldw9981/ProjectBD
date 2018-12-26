// Fill out your copyright notice in the Description page of Project Settings.

#include "BDGameInstance.h"
#include "Items/InventorySystem.h"
#include "ConstructorHelpers.h"

UBDGameInstance::UBDGameInstance()
{

	//CSV파일 연결
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ItemDataTable(TEXT("DataTable'/Game/Blueprints/Items/Data/DT_ItemTable.DT_ItemTable'"));
	if (DT_ItemDataTable.Succeeded())
	{
		ItemDataTable = DT_ItemDataTable.Object;
	}
}

void UBDGameInstance::Init()
{
	Inventory = NewObject<UInventorySystem>(this);

	//For Test
	if (UserID.Len() == 0)
	{
		UserID = FString::Printf(TEXT("Noname%d"), FMath::RandRange(0, 1000));
	}
}

FItemDataTable & UBDGameInstance::GetItemData(int Index) const
{
	// TODO: insert return statement here
	return *ItemDataTable->FindRow<FItemDataTable>(*FString::FromInt(Index), TEXT("ItemIndex"));

}
