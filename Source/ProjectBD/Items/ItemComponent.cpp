// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemComponent.h"
#include "ConstructorHelpers.h"

// Sets default values for this component's properties
UItemComponent::UItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	//CSV파일 연결
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ItemDataTable(TEXT("DataTable'/Game/Blueprints/Items/Data/DT_ItemTable.DT_ItemTable'"));
	if (DT_ItemDataTable.Succeeded())
	{
		ItemDataTable = DT_ItemDataTable.Object;
	}
}


// Called when the game starts
void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FItemDataTable & UItemComponent::GetItemData(int Index) const
{
	// TODO: insert return statement here
	return *ItemDataTable->FindRow<FItemDataTable>(*FString::FromInt(Index), TEXT("ItemIndex"));

}