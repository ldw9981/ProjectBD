// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomItemSpawner.generated.h"

UCLASS()
class PROJECTBD_API ARandomItemSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARandomItemSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int UniqueSpawnID = 0;

	TMap<int,class AMasterItem*> SpawnItems;		

	UPROPERTY(ReplicatedUsing = "Spwan_OnRep")
	TArray<int> RandomItems;

	USceneComponent* Scene;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Spwan_OnRep();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	class AMasterItem* SpawnMasterItem(int ItemIndex, int ItemCount);
	bool DestroyMasterItem(int TargetSpawnID);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SpawnMasterItem(int ItemIndex, int ItemCount, FVector Location, bool bVisible);
	void Multicast_SpawnMasterItem_Implementation(int ItemIndex, int ItemCount, FVector Location,bool bVisible);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DestroyMasterItem(int TargetSpawnID);
	void Multicast_DestroyMasterItem_Implementation(int TargetSpawnID);

	AMasterItem* GetMasterItem(int SpawnID);
};