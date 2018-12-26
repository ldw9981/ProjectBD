// Fill out your copyright notice in the Description page of Project Settings.

#include "MasterItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Items/ItemComponent.h"
#include "Engine/StreamableManager.h"
#include "Basic/BasicCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UnrealNetwork.h"
#include "BDGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMasterItem::AMasterItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	Sphere->SetSphereRadius(150.0f);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Sphere);
}

// Called when the game starts or when spawned
void AMasterItem::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AMasterItem::OnBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AMasterItem::OnEndOverlap);
	
}
void AMasterItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult )
{
	if (OtherActor->ActorHasTag(FName(TEXT("Player"))))
	{
		ABasicCharacter* Pawn = Cast<ABasicCharacter>(OtherActor);
		Pawn->AddInteraction(this);
	}
}

void AMasterItem::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(FName(TEXT("Player"))))
	{
		ABasicCharacter* Pawn = Cast<ABasicCharacter>(OtherActor);
		Pawn->RemoveInteraction(this);
	}
}

void AMasterItem::CompleteAsyncLoad()
{
	Mesh->SetStaticMesh(ItemData.ItemMesh.Get());
}


void AMasterItem::ItemIndex_OnRep()
{
	UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (!GI)
	{
		return;
	}
	ItemData = GI->GetItemData(ItemIndex);
	if (ItemData.ItemIndex != 0)
	{
		//메시 로딩
		FStreamableManager Loader;
		Mesh->SetStaticMesh(Loader.LoadSynchronous<UStaticMesh>(ItemData.ItemMesh));
		//Loader.RequestAsyncLoad(ItemData.ItemMesh.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &AMasterItem::CompleteAsyncLoad));
	}
}

// Called every frame
void AMasterItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMasterItem::SetItem(int NewItemIndex,int NewItemCount)
{	
	UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GI)
	{
		return;
	}
			
	ItemData = GI->GetItemData(NewItemIndex);
	if (ItemData.ItemIndex != 0)
	{
		ItemIndex = NewItemIndex;
		//메시 로딩
		FStreamableManager Loader;
		Mesh->SetStaticMesh(Loader.LoadSynchronous<UStaticMesh>(ItemData.ItemMesh));
		//Loader.RequestAsyncLoad(ItemData.ItemMesh.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &AMasterItem::CompleteAsyncLoad));
	}
	ItemCount = NewItemCount;
}