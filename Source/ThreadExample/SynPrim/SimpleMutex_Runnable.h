// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class AThreadExampleGameModeBase;

/**
 * 
 */
class THREADEXAMPLE_API FSimpleMutex_Runnable : public FRunnable
{
public:
	FSimpleMutex_Runnable(FColor Color, AThreadExampleGameModeBase* OwnerActor, bool bIsSecondMode);
	~FSimpleMutex_Runnable();

	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Exit() override;

	AThreadExampleGameModeBase* GameMode_Ref = nullptr;

	bool bIsGenerateSecondName;

	FThreadSafeBool bIsStopNameGenerator = false;

	int8 GetRandom(int8 min, int8 max);

	bool GetRandom();
};
