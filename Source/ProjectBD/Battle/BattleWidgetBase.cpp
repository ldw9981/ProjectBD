// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleWidgetBase.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void UBattleWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	KillingMessage = Cast<UScrollBox>(GetWidgetFromName(TEXT("KillingMessage")));

	SetTextBullet(0);
	SetTextExtraBullet(0);
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

void UBattleWidgetBase::SetAliveCountText(int AliveCount)
{
	AliveCountData = FString::Printf(TEXT("%d명 생존"), AliveCount);	
}

void UBattleWidgetBase::SetTextBullet(int Bullet)
{
	TextBullet = FString::Printf(TEXT("%d"), Bullet);
}

void UBattleWidgetBase::SetTextExtraBullet(int ExtraBullet)
{
	TextExtraBullet = FString::Printf(TEXT("%d"), ExtraBullet);
}
