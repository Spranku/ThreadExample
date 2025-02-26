// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleCounter_Runnable.h"

FSimpleCounter_Runnable::FSimpleCounter_Runnable(FColor Color, AThreadExampleGameModeBase* OwnerActor, bool VariableMode)
{
	GameModeRef = OwnerActor;
	bIsUseSafeVariable = VariableMode;
}

FSimpleCounter_Runnable::~FSimpleCounter_Runnable()
{
}

bool FSimpleCounter_Runnable::Init()
{
	return true;
}

//	#pragma optimize("", off) // Off optimisation for thread loop. Now we can stop thread without lags
uint32 FSimpleCounter_Runnable::Run()
{
	// FEvent
	if (GameModeRef->SimpleCounterEvent)
	{
		GameModeRef->SimpleCounterEvent->Wait(10000); 
		if (GameModeRef->SimpleCounterEvent)
		{
			GameModeRef->SimpleCounterEvent->Wait(10000);
		}
	}

	if (bIsUseSafeVariable)
	{
		// Use safe (atomic) variables 
		while (!bIsStopThreadSafe)
		{
			CounterSafe.Increment();
		}
	}
	else
	{
		while (!bIsStopThread)
		{
			// Lopping counter on thread is so fast, so need to delay() or something for stop it.
			//	FPlatformProcess::Sleep(0.02f); Looping so fast, so sleep can help stop it but only for debug. 
			// Or can use #pragma optimize("",off/on) for this func.
			// Finnaly, we will to use safe variable for this problem, example FThreadSafeBool
			Counter++;
		
		}
	}
	return 1;
}
//	#pragma optimize("", on) // Off optimisation for other code

void FSimpleCounter_Runnable::Stop()
{
	// FRunnableThread::Kill() calls this func
	bIsStopThread = true;
	bIsStopThreadSafe = true;

}

void FSimpleCounter_Runnable::Exit()
{
	GameModeRef = nullptr;
}
