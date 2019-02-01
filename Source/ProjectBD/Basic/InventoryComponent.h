// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/ItemDataTable.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct PROJECTBD_API FInventoryItemInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
	int ItemIndex;
	UPROPERTY()
	int ItemCount;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTBD_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	

	const int BulletPerMagazine = 30; //한탄창에 들어가는 총알 수

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = "OnRep_Bullet")
	int LoadedBullet = 0;	//  총을쏠때 줄어드는 현재 탄창에 있는 총알 수

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = "OnRep_Bullet")
	int ExtraBullet = 0;	// 여분 총알수

	int BulletIndex = -1;
	int GrenadeIndex = -1;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray<FInventoryItemInfo> ItemList;
	bool AddItem(int ItemIndex,int ItemCount);
	bool UseItem(int InventoryIndex);
	bool DropItem(int Index);
	bool CheckAdd(int ItemIndex, int ItemCount);

	int GetSameItemIndex(FItemDataTable Item);

	int GetItemIndex(int InventoryIndex);
	int GetItemCount(int InventoryIndex);

	void ClearItem();
	FItemDataTable & GetItemData(int InventoryIndex);


	///
	//현재 총알이 있는지 확인
	bool IsExistLoadedBullet();
	bool IsExistExtraBullet();

	//재장전 완료
	void ReloadComplete();

	//총알 사용
	UFUNCTION(NetMulticast, Reliable)
	void S2A_UseBullet();
	void S2A_UseBullet_Implementation();

	void AddExtraBullet(int AddExtraBullet);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_Bullet();
		
	bool DecreaseItem(int InventoryIndex);
};
