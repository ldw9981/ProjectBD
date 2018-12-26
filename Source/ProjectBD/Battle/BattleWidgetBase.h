// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UBattleWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString PublicMessageData;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HpBarData;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString AliveCountData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UScrollBox* KillingMessage;

	virtual void NativeConstruct() override;

	void AddKillingMessage(const FString & Message);

	void DeleteTopKillingMessage();

};