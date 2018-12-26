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

	int BulletPerMagazine = 30; // 한탄창에 들어가는 총알 수
	int BulletCountinMagazine = 30;  //현재 탄창에 있는 총알 수
	int TotalBulletCount = 120; // 전체 총알수

	//현재 총알이 있는지 확인
	bool IsHaveBullet();

	//재장전 완료
	void ReloadComplete();

	//총알 사용
	bool UseBullet();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	float Damage = 1.0f;

	float GetDamage();

};
