// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/ItemDataTable.h"
#include "Components/StaticMeshComponent.h"
#include "MasterItem.generated.h"


class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class PROJECTBD_API AMasterItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMasterItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = "OnRep_ItemIndex")
	int ItemIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int ItemCount = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int ItemSpawnID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemDataTable ItemData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh;
	class ARandomItemSpawner*  RandomItemSpawner = nullptr;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);

	void CompleteAsyncLoad();

	UFUNCTION()
	void OnRep_ItemIndex();

	bool SetItemData(int NewItemIndex);
	ARandomItemSpawner* GetItemSpawner();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	void SetItem(int NewSpawnID, int NewItemIndex, int NewItemCount);

	bool CheckValid();
	bool SetItemStaticMesh();


	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetVisibleHide();
	void S2A_SetVisibleHide_Implementation();
};
