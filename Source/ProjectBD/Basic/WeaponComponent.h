// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/NoExportTypes.h"
#include "Items/ItemDataTable.h"
#include "WeaponComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UWeaponComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
public:
	UWeaponComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
};
