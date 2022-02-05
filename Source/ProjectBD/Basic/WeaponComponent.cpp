// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponComponent.h"
#include "Net/UnrealNetwork.h"
#include "Basic/BasicCharacter.h"
#include "Basic/BasicPC.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
