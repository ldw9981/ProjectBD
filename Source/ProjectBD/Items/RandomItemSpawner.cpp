// Fill out your copyright notice in the Description page of Project Settings.

#include "RandomItemSpawner.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Items/RandomItemPoint.h"
#include "Engine/World.h"
#include "MasterItem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BDGameInstance.h"
#include "UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ARandomItemSpawner::ARandomItemSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ARandomItemSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (UKismetSystemLibrary::IsServer(GetWorld()))
	{
		TSubclassOf<ARandomItemPoint> RandomItemPointClassType = ARandomItemPoint::StaticClass();
		TArray<AActor*> Results;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), RandomItemPointClassType, Results);
		UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		for (auto it : Results)
		{
			int ItemIndex = FMath::RandRange(1, 7) * 10;
			FItemDataTable& ItemData = GI->GetItemData(ItemIndex);
			AMasterItem* Item = SpawnMasterItem(ItemData.ItemIndex, ItemData.ItemCount);
			Item->SetActorTransform(it->GetActorTransform());
		}
	}
}

// Called every frame
void ARandomItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AMasterItem * ARandomItemSpawner::GetMasterItem(int SpawnID)
{
	return SpawnItems.FindRef(SpawnID);
}

void ARandomItemSpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);	
}

AMasterItem* ARandomItemSpawner::SpawnMasterItem(int ItemIndex, int ItemCount)
{
	FActorSpawnParameters SpawnParms;
	AMasterItem* Item = GetWorld()->SpawnActor<AMasterItem>(ItemTypeClass, SpawnParms);
	UniqueSpawnID++;
	SpawnItems.Add(UniqueSpawnID, Item);
	Item->SetItem(UniqueSpawnID,ItemIndex, ItemCount);
	return Item;
}

bool ARandomItemSpawner::DestroyMasterItem(int TargetSpawnID)
{
	AMasterItem* Item = Cast<AMasterItem>(SpawnItems.FindAndRemoveChecked(TargetSpawnID));
	if (Item)
	{
		Item->Destroy();
		return true;
	}
	return false;
}


