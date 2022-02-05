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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWeaponComponent* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Replicated)
	bool bIsSprint = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float SprintSpeed = 700;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float RunSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float CrouchSpeed = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float CapsuleCrouchHalfHeight = 44.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Replicated)
	bool bIsIronsight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FVector NormalSpringPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FVector CrouchSpringPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FVector DeadSpringPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FRotator DeadSpringRotator = FRotator(-90.0f,0.0f,0.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", ReplicatedUsing = "IsFire_OnRep")
	bool bIsFire = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", ReplicatedUsing = "HP_OnRep")
	float CurrentHP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", ReplicatedUsing = "HP_OnRep")
	float MaxHP = 100.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	//class UAnimMontage* DeadAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* ReloadAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Replicated)
	bool bIsReload = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Replicated)
	bool bLeftLean = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Replicated)
	bool bRightLean = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float LeanAngle = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemBox")
	TSubclassOf<class AStaticMeshActor> ItemBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	TSubclassOf<class AActor> BulletTypeClass;

protected:
	class ARandomItemSpawner*	RandomItemSpawner = nullptr;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void Reload();
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	void Sprint();
	void UnSprint();
	void DoCrouch();
	void DoIronsight();
	void EquipPrimary();
	void EquipSecondary();
	void EquipPistol();
	void EquipMelee();
	void EquipGrenade();


	FRotator GetAimOffset() const;
	UFUNCTION()
	void IsFire_OnRep();

	void ToggleInventory();
	void InteractionClose();

	UFUNCTION()
	void StartFire();

	UFUNCTION()
	void StopFire();

	UFUNCTION()
	void Client_OnTimerFire();

	UFUNCTION()
	void HP_OnRep();

	UFUNCTION()
	void StartLeftLean();

	UFUNCTION()
	void StopLeftLean();

	UFUNCTION()
	void StartRightLean();

	UFUNCTION()
	void StopRightLean();

	bool IsDead();

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
	void C2S_Fire(FVector TraceStart, FVector TraceEnd, FVector MuzzleLocation, FRotator MuzzleRotator);
	bool C2S_Fire_Validate(FVector TraceStart, FVector TraceEnd, FVector MuzzleLocation, FRotator MuzzleRotator);
	void C2S_Fire_Implementation(FVector TraceStart, FVector TraceEnd, FVector MuzzleLocation, FRotator MuzzleRotator);

	UFUNCTION(Client, Reliable)
	void S2A_FireEffect(FName InSocketName);
	void S2A_FireEffect_Implementation(FName InSocketName);

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_Reload();
	bool C2S_Reload_Validate();
	void C2S_Reload_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_ReloadComplete();
	void S2A_ReloadComplete_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_Die();
	void S2A_Die_Implementation();

	void SetItemSpawner();
};
