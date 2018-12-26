// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Items/ItemDataTable.h"
#include "BDGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UBDGameInstance : public UGameInstance
{
	GENERATED_BODY()

		UBDGameInstance();
public:
	class UInventorySystem* Inventory;

	virtual void Init() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient)
	FString UserID;

	class UDataTable* ItemDataTable;
	FItemDataTable& GetItemData(int Index) const;

};
