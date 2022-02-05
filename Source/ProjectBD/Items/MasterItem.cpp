// Fill out your copyright notice in the Description page of Project Settings.

#include "MasterItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/StreamableManager.h"
#include "Basic/BasicCharacter.h"
#include "Basic/BasicPC.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "BDGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMesh.h"
#include "Items/RandomItemSpawner.h"

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

	bReplicates = true;
}

// Called when the game starts or when spawned
void AMasterItem::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AMasterItem::OnBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AMasterItem::OnEndOverlap);
		
}

void AMasterItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//COND_InitialOnly - This property will only attempt to send on the initial bunch
	DOREPLIFETIME_CONDITION(AMasterItem, ItemIndex, COND_InitialOnly);	
	DOREPLIFETIME_CONDITION(AMasterItem, ItemCount, COND_InitialOnly);	
	DOREPLIFETIME_CONDITION(AMasterItem, ItemSpawnID, COND_InitialOnly);
}
void AMasterItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult )
{
	if (!CheckValid())
	{
		return;
	}

	if (!OtherActor->ActorHasTag(FName(TEXT("Player"))))
	{
		return;
	}
	
	ABasicCharacter* Pawn = Cast<ABasicCharacter>(OtherActor);
	if (!Pawn)
	{
		return;
	}
	ABasicPC* PC = Cast<ABasicPC>(Pawn->GetController());
	if (!PC)
	{
		return;
	}
	PC->AddInteraction(this);
}

void AMasterItem::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (!CheckValid())
	{
		return;
	}

	if (!OtherActor->ActorHasTag(FName(TEXT("Player"))))
	{
		return;
	}

	ABasicCharacter* Pawn = Cast<ABasicCharacter>(OtherActor);
	if (!Pawn)
	{
		return;
	}
	ABasicPC* PC = Cast<ABasicPC>(Pawn->GetController());
	if (!PC)
	{
		return;
	}
	PC->RemoveInteraction(this);
}

void AMasterItem::CompleteAsyncLoad()
{
	//Mesh->SetStaticMesh(ItemData.ItemMesh);
}

void AMasterItem::OnRep_ItemIndex()
{
	if (SetItemData(ItemIndex))
	{
		//메시 로딩
		bool bResult = SetItemStaticMesh();
	}
}

bool AMasterItem::SetItemData(int NewItemIndex)
{
	UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GI)
	{
		return false;
	}
	auto& RetItemData = GI->GetItemData(NewItemIndex);
	if (RetItemData.ItemIndex == 0)
	{
		return false;
	}
	ItemData = RetItemData;
	
	return true;
}

// Called every frame
void AMasterItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMasterItem::SetItem(int NewSpawnID,int NewItemIndex,int NewItemCount)
{	
	if (!SetItemData(NewItemIndex))
	{
		return;
	}
		
	ItemSpawnID = NewSpawnID;	
	ItemIndex = NewItemIndex;
	ItemCount = NewItemCount;
	//메시 로딩
	bool bResult = SetItemStaticMesh();
}

bool AMasterItem::CheckValid()
{
	if (ItemIndex == -1 || ItemCount == -1 || ItemSpawnID == -1)
	{
		return false;
	}
	return true;
}

bool AMasterItem::SetItemStaticMesh()
{
	FStreamableManager Loader;
	return Mesh->SetStaticMesh(Loader.LoadSynchronous<UStaticMesh>(ItemData.ItemMesh));
}

void AMasterItem::S2A_SetVisibleHide_Implementation()
{
	Mesh->SetVisibility(false);
}
