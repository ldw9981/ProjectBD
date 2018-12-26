// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponComponent.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}

float UWeaponComponent::GetDamage()
{
	return Damage;
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UWeaponComponent::IsHaveBullet()
{
	return TotalBulletCount <= 0 ? false : true;
}

void UWeaponComponent::ReloadComplete()
{
	//총알이 없는거 
	if (!IsHaveBullet())
	{
		return;
	}

	//충전 되야 할 총알 수
	int AddBullet = BulletPerMagazine - BulletCountinMagazine;

	//남은 총알이 충전 할 총알보다 작은 경우
	if (TotalBulletCount <= AddBullet)
	{
		BulletCountinMagazine += TotalBulletCount;
		TotalBulletCount = 0;
	}
	else
	{
		//남은 총알에서 충전 될 총알을 빼줌
		TotalBulletCount -= AddBullet;
		BulletCountinMagazine = BulletPerMagazine;
	}
}

bool UWeaponComponent::UseBullet()
{
	if (BulletCountinMagazine >  0)
	{
		BulletCountinMagazine--;
		return true;
	}
	else
	{
		return false;
	}
}

