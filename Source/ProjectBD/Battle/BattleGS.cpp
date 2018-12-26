// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleGS.h"
#include "UnrealNetwork.h"
#include "Battle/BattleWidgetBase.h"
#include "Basic/BasicPC.h"
#include "Kismet/GameplayStatics.h"

void ABattleGS::AliveCount_OnRep()
{
	ABasicPC* PC = Cast<ABasicPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && PC->IsLocalPlayerController())
	{
		PC->BattleWidget->AliveCountData = FString::Printf(TEXT("%d명 생존"), AliveCount);
	}
}

void ABattleGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABattleGS, AliveCount);
}
