// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHUD.h"
#include "Engine/StreamableManager.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"

AMyHUD::AMyHUD()
{
}

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	FStreamableManager Loader;
	FStringAssetReference HudTextureString(TEXT("Texture2D'/Game/Weapons/crosshair.crosshair'"));
	Crosshair = Loader.LoadSynchronous<UTexture2D>(HudTextureString);
}

void AMyHUD::DrawHUD()
{
	Super::DrawHUD();

	if (Crosshair)
	{
		DrawTextureSimple(Crosshair,
			(Canvas->SizeX / 2) - (Crosshair->GetSizeX() / 2),
			(Canvas->SizeY / 2) - (Crosshair->GetSizeY() / 2)
		);
	}

}
