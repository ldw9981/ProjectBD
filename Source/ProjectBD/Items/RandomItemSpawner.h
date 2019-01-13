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
	USceneComponent* Scene;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	class AMasterItem* SpawnMasterItem(int ItemIndex, int ItemCount);
	bool DestroyMasterItem(int TargetSpawnID);
	AMasterItem* GetMasterItem(int SpawnID);
};