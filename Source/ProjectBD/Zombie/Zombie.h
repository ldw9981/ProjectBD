// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Zombie.generated.h"

UENUM(BlueprintType)
enum class EZombieState : uint8
{
	Normal		= 0		UMETA(Display = "Normal"),
	Battle		= 1		UMETA(Display = "Battle"),
	Chase		= 2		UMETA(Display = "Chase"),
	Dead		= 3		UMETA(Display = "Dead")
};

UCLASS()
class PROJECTBD_API AZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float MaxHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float WalkSpeed = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float RunSpeed = 350.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float Attack = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float AttackRange = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EZombieState CurrentState = EZombieState::Normal;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPawnSensingComponent* PawnSensing;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	void SetState(EZombieState NewState);

	UFUNCTION()
	void OnHearNoise(APawn* Pawn, const FVector& Location, float Volume);


	bool SetSpeed(EZombieState NewState);
};
