// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ThreadExample/ThreadExampleGameModeBase.h"
#include "CoreMinimal.h"

class AThreadExampleGameModeBase;
/**
 * 
 */
class THREADEXAMPLE_API FSimpleCounter_Runnable : public FRunnable
{
public:
	FSimpleCounter_Runnable(FColor Color, AThreadExampleGameModeBase* OwnerActor);

	~FSimpleCounter_Runnable();

	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Exit() override;

	// Not safe variable
	bool bIsStopThread = false;

	int64 Counter = 0;
};
