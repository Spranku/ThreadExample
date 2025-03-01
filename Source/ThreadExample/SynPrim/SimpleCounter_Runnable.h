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
	FSimpleCounter_Runnable(FColor Color, AThreadExampleGameModeBase* OwnerActor, bool VariableMode);

	~FSimpleCounter_Runnable();

	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Exit() override;

	// Setting
	AThreadExampleGameModeBase* GameModeRef;

	bool bIsUseSafeVariable;

	// Not safe variable
	bool bIsStopThread = false;

	int64 Counter = 0;

	// Safe variable has atomic type. Actually this is amotic variables. They safe for using with thread
	FThreadSafeBool bIsStopThreadSafe = FThreadSafeBool(false); // Recomended to use Constuctor(), than  = false;
	FThreadSafeCounter CounterSafe = FThreadSafeCounter(0);		// Recomended to use Constuctor(), than  = 0;

};
