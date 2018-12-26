// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemToolTipBase.h"
#include "Components/TextBlock.h"

void UItemToolTipBase::NativeConstruct()
{
	ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
}
