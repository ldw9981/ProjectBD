// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCameraShake.h"

UMyCameraShake::UMyCameraShake()
{
	//OscillationDuration = 5.0f; //셰이크 타임
	//OscillationBlendInTime = 0.5f; //카메라 흔들기 시작 할때 블렌딩 시간
	//OscillationBlendOutTime = 0.5f; //카메라 흔들기 끝 할때 블렌딩 시간

	//RotOscillation.Pitch.Amplitude = 50.0f;  // 크기 
	//RotOscillation.Pitch.Frequency = 20.0f; //횟수
	//RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom; // 시작 값

	//RotOscillation.Yaw.Amplitude = 30.0f;  // 크기 
	//RotOscillation.Yaw.Frequency = 20.0f; //횟수
	//RotOscillation.Yaw.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom; // 시

	//RotOscillation.Roll.Amplitude = 100.0f;  // 크기 
	//RotOscillation.Roll.Frequency = 10.0f; //횟수
	//RotOscillation.Roll.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom; // 시


	OscillationDuration = 0.12f; //셰이크 타임
	OscillationBlendInTime = 0.05f; //카메라 흔들기 시작 할때 블렌딩 시간
	OscillationBlendOutTime = 0.05f; //카메라 흔들기 끝 할때 블렌딩 시간

	RotOscillation.Pitch.Amplitude = 10.0f;  // 크기 
	RotOscillation.Pitch.Frequency = 1.0f; //횟수
	RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero; // 시작 값
}
