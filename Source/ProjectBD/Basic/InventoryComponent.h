// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/ItemDataTable.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct PROJECTBD_API FInventoryItemInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ItemIndex;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ItemCount;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTBD_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



	TArray<FInventoryItemInfo> ItemList;
	bool AddItem(int ItemIndex,int Count);
	bool UseItem(int InventoryIndex);
	bool DropItem(int Index);

	int GetSameItemIndex(FItemDataTable Item);

	int GetItemIndex(int InventoryIndex);
	int GetItemCount(int InventoryIndex);
};
