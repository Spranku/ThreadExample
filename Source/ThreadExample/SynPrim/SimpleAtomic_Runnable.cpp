// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleAtomic_Runnable.h"

FSimpleAtomic_Runnable::FSimpleAtomic_Runnable(FColor Color, 
											   AThreadExampleGameModeBase* OwnerActor, 
											   uint32 NeedIteration, 
											   bool SeprateLogic,
											   bool bIsUseAtomic)
{
	GameMode_Ref = OwnerActor;
	NumberIteration = NeedIteration;
	bUseAtomicFlag = bIsUseAtomic;
	SeprateLogicFlag = SeprateLogic;
	UE_LOG(LogTemp, Error, TEXT("FSimpleAtomic_Runnable::FSimpleAtomic_Runnable"));
}

FSimpleAtomic_Runnable::~FSimpleAtomic_Runnable()
{
	UE_LOG(LogTemp, Error, TEXT("FSimpleAtomic_Runnable::~FSimpleAtomic_Runnable"));
}

bool FSimpleAtomic_Runnable::Init()
{
	UE_LOG(LogTemp, Error, TEXT("FSimpleAtomic_Runnable::Init"));
	return true;
}

uint32 FSimpleAtomic_Runnable::Run()
{
	//FPlatformProcess::Sleep(1.0f); // sleep_for from C++ 

	for (int i = 0; i < NumberIteration; ++i)
	{
		if (SeprateLogicFlag)
		{
			if (bUseAtomicFlag)
			{
				// Safe elem for thread
				GameMode_Ref->AtomicCounter1.fetch_add(1); // fetch_add from C++ STL
				FPlatformAtomics::InterlockedIncrement(&GameMode_Ref->NonAtomicCounter1);
			}
			else
			{
				// It's not safe elem for thread
				GameMode_Ref->NonAtomicCounter1++;
			}
		}
		else
		{
			if (bUseAtomicFlag)
			{
				// Safe elem for thread
				GameMode_Ref->AtomicCounter2.fetch_add(1);
				FPlatformAtomics::InterlockedIncrement(&GameMode_Ref->NonAtomicCounter2);
			}
			else
			{
				// It's not safe elem for thread
				GameMode_Ref->NonAtomicCounter2++;
			}
		}
	}

	return 0;
}

void FSimpleAtomic_Runnable::Stop()
{
	UE_LOG(LogTemp, Error, TEXT("FSimpleAtomic_Runnable::Stop"));
}

void FSimpleAtomic_Runnable::Exit()
{
	UE_LOG(LogTemp, Error, TEXT("FSimpleAtomic_Runnable::Exit"));
}
