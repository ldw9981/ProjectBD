// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Items/ItemDataTable.h"
#include "BasicCharacter.generated.h"

UCLASS()
class PROJECTBD_API ABasicCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetHPBar();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void CheckItem();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Reload();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWeaponComponent* Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UInventoryComponent* Inventory;


	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);

	void Sprint();
	void UnSprint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State", Replicated)
	bool bIsSprint = false;

	//float WalkSpeed = 150.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float SprintSpeed = 700;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float RunSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float CrouchSpeed = 150.0f;

	void DoCrouch();
	void DoIronsight();

	FRotator GetAimOffset() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float CapsuleCrouchHalfHeight = 44.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Replicated)
	bool bIsIronsight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FVector NormalSpringPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FVector CrouchSpringPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", ReplicatedUsing = "IsFire_OnRep")
	bool bIsFire = false;
	UFUNCTION()
	void IsFire_OnRep();

	UFUNCTION()
	void StartFire();

	UFUNCTION()
	void StopFire();

	UFUNCTION()
	void Client_OnTimerFire();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class USoundBase* FireSound;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (AllowPrivateAcess = true))
	class UMaterialInstance* BulletDecal;

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State", ReplicatedUsing = "HP_OnRep")
	float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", ReplicatedUsing = "HP_OnRep")
	float MaxHP = 100.0f;

	UFUNCTION()
	void HP_OnRep();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* DeadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Replicated)
	bool bIsReload = false;

	UFUNCTION()
	void StartLeftLean();

	UFUNCTION()
	void StopLeftLean();

	UFUNCTION()
	void StartRightLean();

	UFUNCTION()
	void StopRightLean();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Replicated)
	bool bLeftLean = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Replicated)
	bool bRightLean = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float LeanAngle = 30.0f;

	bool IsDead();

	TArray<class AMasterItem*> InteractionItemList;

	void AddInteraction(class AMasterItem* NewItem);
	void RemoveInteraction(class AMasterItem* NewItem);
	int GetClosestItem(FVector SightLocation);
	FVector GetSightLocation();

	FTimerHandle ItemCheckHandle;

	void Interaction();

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_Interaction(FVector Location);
	bool C2S_Interaction_Validate(FVector Location);
	void C2S_Interaction_Implementation(FVector Location);


	void ToggleInventory();

	bool PickupItem(AMasterItem* MasterItem);

	void DropItem(int InventoryIndex);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_DoIronsight();
	bool C2S_DoIronsight_Validate();
	void C2S_DoIronsight_Implementation();


	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_SetSprint(bool Sprint);
	bool C2S_SetSprint_Validate(bool Sprint);
	void C2S_SetSprint_Implementation(bool Sprint);


	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_SetLeftLean(bool NewLean);
	bool C2S_SetLeftLean_Validate(bool NewLean);
	void C2S_SetLeftLean_Implementation(bool NewLean);


	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_SetRightLean(bool NewLean);
	bool C2S_SetRightLean_Validate(bool NewLean);
	void C2S_SetRightLean_Implementation(bool NewLean);


	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_SetFire(bool NewFire);
	bool C2S_SetFire_Validate(bool NewFire);
	void C2S_SetFire_Implementation(bool NewFire);



	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetMaxWalkSpeed(float NewSpeed);
	void S2A_SetMaxWalkSpeed_Implementation(float NewSpeed);

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_Fire(FVector TraceStart, FVector TraceEnd);
	bool C2S_Fire_Validate(FVector TraceStart, FVector TraceEnd);
	void C2S_Fire_Implementation(FVector TraceStart, FVector TraceEnd);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_HitEffectBlood(FVector Point,FRotator Rotation);
	void S2A_HitEffectBlood_Implementation(FVector Point, FRotator Rotation);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_HitEffectBlock(FVector Point, FRotator Rotation);
	void S2A_HitEffectBlock_Implementation(FVector Point, FRotator Rotation);


	UFUNCTION(NetMulticast, Reliable)
	void S2A_FireEffect(FName InSocketName);
	void S2A_FireEffect_Implementation(FName InSocketName);

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_Reload();
	bool C2S_Reload_Validate();
	void C2S_Reload_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_ReloadComplete();
	void S2A_ReloadComplete_Implementation();
};
