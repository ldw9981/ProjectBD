// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicPlayerCameraManager.h"
#include "Basic/BasicCharacter.h"
#include "Basic/BasicPC.h"
#include "Curves/CurveFloat.h"
#include "ConstructorHelpers.h"
#include "Components/TimelineComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFrameWork/SpringArmComponent.h"

ABasicPlayerCameraManager::ABasicPlayerCameraManager()
{
	//Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("CurveFloat'/Game/Blueprints/CameraTest.CameraTest'"));
	if (Curve.Succeeded())
	{
		FloatCurve = Curve.Object;
	}
}

void ABasicPlayerCameraManager::BeginPlay()
{
	FOnTimelineFloat onTimelineCallback;
	FOnTimelineEventStatic onTimelineFinishedCallback;

	Super::BeginPlay();


	if (FloatCurve != NULL)
	{
		MyTimeline = NewObject<UTimelineComponent>(this, FName("TimelineAnimation"));
		MyTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		MyTimeline->SetPropertySetObject(this);

		MyTimeline->SetLooping(false);
		MyTimeline->SetTimelineLength(5.0f);
		MyTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);

		MyTimeline->SetPlaybackPosition(0.0f, false);

		onTimelineCallback.BindUFunction(this, TEXT("TimelineCallback"));
		onTimelineFinishedCallback.BindUFunction(this, FName{ TEXT("TimelineFinishedCallback") });
		MyTimeline->AddInterpFloat(FloatCurve, onTimelineCallback);
		MyTimeline->SetTimelineFinishedFunc(onTimelineFinishedCallback);

		MyTimeline->RegisterComponent();

		//UE_LOG(LogClass, Warning, TEXT("AAAAAAAa"));
	}
}

void ABasicPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	ABasicCharacter* Pawn = Cast<ABasicCharacter>(GetOwningPlayerController()->GetPawn());

	if (Pawn)
	{
		//float TargetZ = Pawn->bIsCrouched ? Pawn->GetActorLocation().Z + (Pawn->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2) : Pawn->GetActorLocation().Z + Pawn->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		//FVector TargetPosition(Pawn->SpringArm->GetComponentLocation().X, Pawn->SpringArm->GetComponentLocation().Y, FMath::FInterpTo(Pawn->SpringArm->GetComponentLocation().Z, TargetZ, DeltaTime, 15.0f));
		//Pawn->SpringArm->SetWorldLocation(TargetPosition);

		//float TargetZ = Pawn->bIsCrouched ? 38 : 72.0f;
		//CurrentZ = FMath::FInterpTo(CurrentZ, TargetZ, DeltaTime, 15.0f);
		//Pawn->SpringArm->SetRelativeLocation(FVector(0, 0, CurrentZ));

		//Vector 보간
		FVector TargetPosition = Pawn->bIsCrouched ? Pawn->CrouchSpringPosition : Pawn->NormalSpringPosition;
		CurrentSpringArm = FMath::VInterpTo(CurrentSpringArm, TargetPosition, DeltaTime, 15.0f);
		Pawn->SpringArm->SetRelativeLocation(CurrentSpringArm);


		if (bIsOnIronsight != Pawn->bIsIronsight)
		{
			TotalTime = 0;
			//MyTimeline->PlayFromStart();
			//MyTimeline->Reverse();
		}
		TotalTime += DeltaTime;
		
		//커브 자료형을 이용한 보간
		//float TargetFOV = Pawn->bIsIronsight ? FloatCurve->GetFloatValue(TotalTime) : NormalFOV;
		//DefaultFOV = TargetFOV;
		//SetFOV(DefaultFOV);
		//bIsOnIronsight = Pawn->bIsIronsight;

		//일반적인 수학 보간
		float TargetFOV = Pawn->bIsIronsight ? IronsightFOV : NormalFOV;
		DefaultFOV = FMath::FInterpTo(DefaultFOV, TargetFOV, DeltaTime, 15.0f);
		SetFOV(DefaultFOV);


	}

	Super::UpdateCamera(DeltaTime);
}

void ABasicPlayerCameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (MyTimeline != NULL)
	{
		MyTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
	}
}

void ABasicPlayerCameraManager::TimelineCallback(float val)
{
	UE_LOG(LogClass, Warning, TEXT("%f"), val);
	SetFOV(val);

}

void ABasicPlayerCameraManager::TimelineFinishedCallback()
{
	UE_LOG(LogClass, Warning, TEXT("%f"), DefaultFOV);
}

void ABasicPlayerCameraManager::PlayTimeline()
{
}
