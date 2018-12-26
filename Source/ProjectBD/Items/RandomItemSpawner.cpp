// Fill out your copyright notice in the Description page of Project Settings.

#include "RandomItemSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Items/RandomItemPoint.h"
#include "Engine/World.h"
#include "MasterItem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BDGameInstance.h"
#include "UnrealNetwork.h"

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

		for (auto it : Results)
		{
			int ItemIndex = FMath::RandRange(1, 6) * 10;
			RandomItems.Add(ItemIndex);
		}
		Spwan_OnRep();
	}
}

// Called every frame
void ARandomItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARandomItemSpawner::Spwan_OnRep()
{
	TSubclassOf<ARandomItemPoint> RandomItemPointClassType = ARandomItemPoint::StaticClass();
	TArray<AActor*> Results;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), RandomItemPointClassType, Results);

	FVector SourceLocation = GetActorLocation();
	Results.Sort([SourceLocation](const AActor& A, const AActor& B)
	{
		float DistanceA = FVector::DistSquared(SourceLocation, A.GetActorLocation());
		float DistanceB = FVector::DistSquared(SourceLocation, B.GetActorLocation());

		return DistanceA > DistanceB;
	});

	UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	for (int Index = 0; Index< Results.Num();Index++)
	{
		FItemDataTable& ItemData = GI->GetItemData(RandomItems[Index]);
		
		TSubclassOf<AMasterItem> MasterItemClassType = AMasterItem::StaticClass();
		FActorSpawnParameters SpawnParms;
		AMasterItem* Item = GetWorld()->SpawnActor<AMasterItem>(MasterItemClassType, SpawnParms);
		Item->SetActorTransform(Results[Index]->GetActorTransform());
		Item->SetItem(RandomItems[Index], ItemData.ItemCount);
	}
}

void ARandomItemSpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ARandomItemSpawner, RandomItems, COND_InitialOnly);	//COND_InitialOnly - This property will only attempt to send on the initial bunch
}

