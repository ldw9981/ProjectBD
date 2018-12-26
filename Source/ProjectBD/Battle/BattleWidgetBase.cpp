// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleWidgetBase.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void UBattleWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	KillingMessage = Cast<UScrollBox>(GetWidgetFromName(TEXT("KillingMessage")));
}

void UBattleWidgetBase::AddKillingMessage(const FString& Message)
{
	if (KillingMessage)
	{
		UTextBlock* NewMessageBlock = NewObject<UTextBlock>(this);
		if (NewMessageBlock)
		{
			NewMessageBlock->Font.Size = 18;
			NewMessageBlock->SetJustification(ETextJustify::Right);
			NewMessageBlock->SetText(FText::FromString(Message));
			KillingMessage->AddChild(NewMessageBlock);
		}
	}
}

void UBattleWidgetBase::DeleteTopKillingMessage()
{
	if (KillingMessage && KillingMessage->GetChildrenCount() > 0)
	{
		KillingMessage->RemoveChildAt(0);
	}
}
