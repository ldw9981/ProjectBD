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
	float SafeZoneBarData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HpBarData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString TextBullet;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString TextExtraBullet;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString AliveCountData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString TextPhazeTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UScrollBox* KillingMessage;

	virtual void NativeConstruct() override;

	void AddKillingMessage(const FString & Message);

	void DeleteTopKillingMessage();
	void SetAliveCountText(int AliveCount);
	void SetTextBullet(int Bullet);
	void SetTextExtraBullet(int ExtraBullet);
	void SetTextPhazeTime(float PhazeTime);
};