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

void AThreadExampleGameModeBase::CreateSimpleAtomicThread()
{
	FColor color;
	// my first thread
	class FSimpleAtomic_Runnable *MyRunnableClass_SimpleAtomic = new FSimpleAtomic_Runnable(color, this, 2000); // Thread with arguments
	CurrentRunnableGameModeThread_SimpleAtomic.Add(FRunnableThread::Create(MyRunnableClass_SimpleAtomic, TEXT("SimpleAtomic thread"), 0, EThreadPriority::TPri_Normal));
}
