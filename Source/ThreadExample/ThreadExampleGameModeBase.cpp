// Copyright Epic Games, Inc. All Rights Reserved.


#include "ThreadExampleGameModeBase.h"
#include "HAL/Thread.h"
#include "SynPrim/SimpleAtomic_Runnable.h"

void AThreadExampleGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	uint32 ThreadID = FPlatformTLS::GetCurrentThreadId();

	//UE_LOG(LogTemp, Error, TEXT("AThreadExampleGameModeBase::Tick ID - %d, Name - %s"), ThreadID, FThreadManager::GetThreadName(ThreadID));

	/*FThreadManager::Get().ForEachThread([&](uint32 ThreadId, FRunnableThread* Runnable)
		{
			FString myThreadType = "none";
			switch (Runnable->GetThreadPriority())
			{
			case TPri_Normal:
				myThreadType = "TPri_Normal";
				break;
			case TPri_AboveNormal:
				myThreadType = "TPri_AboveNormal";
				break;
			case TPri_BelowNormal:
				myThreadType = "TPri_BelowNormal";
				break;
			case TPri_Highest:
				myThreadType = "TPri_Highest";
				break;
			case TPri_Lowest:
				myThreadType = "TPri_Lowest";
				break;
			case TPri_SlightlyBelowNormal:
				myThreadType = "TPri_SlightlyBelowNormal";
				break;
			case TPri_TimeCritical:
				myThreadType = "TPri_TimeCritical";
				break;
			case TPri_Num:
				myThreadType = "TPri_Num";
				break;
			default:
				break;
			}

			UE_LOG(LogTemp, Error, TEXT("AThreadExampleGameModeBase::Tick ID - %d, Priority - %s"), ThreadID, *Runnable->GetThreadName(),*myThreadType);
		});*/
	
}

void AThreadExampleGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AThreadExampleGameModeBase::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
//////////////////////////////////////////////////// Counter ///////////////////////////////////////////////////////////////////////
void AThreadExampleGameModeBase::StopSimpleCounterThread()
{
	if (CurrentRunningGameModeThread_SimpleCounter)
	{
		CurrentRunningGameModeThread_SimpleCounter->WaitForCompletion(); // Like detach() ; Wait during thread finish work
		CurrentRunningGameModeThread_SimpleCounter = nullptr;
	}
}

void AThreadExampleGameModeBase::KillSimpleCounterThread(bool bIsShouldWait)
{
	if (CurrentRunningGameModeThread_SimpleCounter)
	{
		CurrentRunningGameModeThread_SimpleCounter->Kill(bIsShouldWait);
	}
}

void AThreadExampleGameModeBase::CreateSimpleCounterThread()
{
	if (!CurrentRunningGameModeThread_SimpleCounter)
	{
		if (!MyRunnableClass_SimpleCounter)
		{
			
			MyRunnableClass_SimpleCounter = new FSimpleCounter_Runnable(ColorSimpleCounter, this);
		}
		CurrentRunningGameModeThread_SimpleCounter = FRunnableThread::Create(MyRunnableClass_SimpleCounter, TEXT("SimpleCounter Thread"), 0, EThreadPriority::TPri_Normal);
	}
}

int64 AThreadExampleGameModeBase::GetSimpleCounterThread()
{
	int64 result = 0;
	if (MyRunnableClass_SimpleCounter)
	{
		result = MyRunnableClass_SimpleCounter->Counter;
	}
	return result;
}

//////////////////////////////////////////////////// Atomic ///////////////////////////////////////////////////////////////////////

void AThreadExampleGameModeBase::CreateSimpleAtomicThread()
{
	for (int i =0; i < NumberOfThreadToCreate; ++i)
	{
		bool bIsFlag = false;
		if (i % 2)
			bIsFlag = true;

		FColor color;
		// my first thread
		class FSimpleAtomic_Runnable *MyRunnableClass_SimpleAtomic = new FSimpleAtomic_Runnable(color, this, IterationRunnableCircle, bIsFlag, bIsUseAtomic); // Thread with arguments
		CurrentRunnableGameModeThread_SimpleAtomic.Add(FRunnableThread::Create(MyRunnableClass_SimpleAtomic, TEXT("SimpleAtomic thread"), 0, EThreadPriority::TPri_Lowest));
	}
}

void AThreadExampleGameModeBase::GetCounterSimpleAtomic(int32& Atomic1, int32& Atomic2, int32& NonAtomic1, int32& NonAtomic2)
{
	if (bIsUseAtomic)
	{
		
		NonAtomic1 = FPlatformAtomics::AtomicRead(&NonAtomicCounter1); // Safe read data
		NonAtomic2 = FPlatformAtomics::AtomicRead(&NonAtomicCounter2); // Safe read data

		if (AtomicCounter1.is_lock_free())
		{
			Atomic1 = AtomicCounter1;
		}
		if (AtomicCounter2.is_lock_free())
		{
			Atomic2 = AtomicCounter2;
		}
	}
	else
	{
		NonAtomic1 = NonAtomicCounter1;
		NonAtomic2 = NonAtomicCounter2;
	}
}

void AThreadExampleGameModeBase::ResetCounterSimpleAtomic()
{
	NonAtomicCounter1 = 0;
	NonAtomicCounter2 = 0;
	AtomicCounter1 = 0;
	AtomicCounter2 = 0;
}
