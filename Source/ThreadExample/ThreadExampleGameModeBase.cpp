// Copyright Epic Games, Inc. All Rights Reserved.


#include "ThreadExampleGameModeBase.h"
#include "HAL/Thread.h"
#include "SynPrim/SimpleAtomic_Runnable.h"
#include "SynPrim/SimpleCounter_Runnable.h"
#include "SynPrim/SimpleMutex_Runnable.h"
#include "SynPrim/SimpleCollectable_Runnable.h"
#include "Misc/ScopeLock.h"
#include "DumpCuteCube.h"
#include "MessageEndpointBuilder.h"

void AThreadExampleGameModeBase::BusMessageHandler_NameGenerator(const FBusStructMessage_NameGenerator& Message,
																 const TSharedRef<IMessageContext, 
																 ESPMode::ThreadSafe>& Context)
{
	// Message receiver
	// Here we can give message from other thread
	EventMessage_NameGenerator(Message.bIsSecondName, Message.TextName);
	
}

void AThreadExampleGameModeBase::BusMessageHandler_NPCInfo(const FInfoNPC& Message,
														   const TSharedRef<IMessageContext, 
														   ESPMode::ThreadSafe>& Context)
{
	EventMessage_NPCInfo(Message);
}

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

	// Check current Thread and stop it
	//StopSimpleCounterThread();

	ReceiveEndPoint_NameGenerator = FMessageEndpoint::Builder("Receiver_AThreadExampleGameModeBase").Handling<FBusStructMessage_NameGenerator>(this,&AThreadExampleGameModeBase::BusMessageHandler_NameGenerator);

	// Subscribe for messages
	if (ReceiveEndPoint_NameGenerator.IsValid())
	{
		ReceiveEndPoint_NameGenerator->Subscribe<FBusStructMessage_NameGenerator>();
	}

	ReceiveEndPoint_NPCInfo = FMessageEndpoint::Builder("Receiver_AThreadExampleGameModeBase").Handling<FInfoNPC>(this, &AThreadExampleGameModeBase::BusMessageHandler_NPCInfo);

	if (ReceiveEndPoint_NPCInfo.IsValid())
	{
		ReceiveEndPoint_NPCInfo->Subscribe<FInfoNPC>();
	}

}

void AThreadExampleGameModeBase::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	StopSimpleCounterThread();
	StopSimpleMutexThreads();

	if (ReceiveEndPoint_NameGenerator.IsValid())
	{
		ReceiveEndPoint_NameGenerator.Reset();
	}

	if (ReceiveEndPoint_NPCInfo.IsValid())
	{
		ReceiveEndPoint_NPCInfo.Reset();
	}
}

void AThreadExampleGameModeBase::SendRef_ScopedEvent(FScopedEvent& ScopedEvent_Ref)
{
	SimpleCounterScopedEvent_Ref = &ScopedEvent_Ref;
}
//////////////////////////////////////////////////// Counter ///////////////////////////////////////////////////////////////////////
void AThreadExampleGameModeBase::StopSimpleCounterThread()
{
	if (CurrentRunningGameModeThread_SimpleCounter)
	{
		if (MyRunnableClass_SimpleCounter)
		{
			CurrentRunningGameModeThread_SimpleCounter->Suspend(false); // Is thread was paused - continue
			// NOT SAFE
			MyRunnableClass_SimpleCounter->bIsStopThread = true;
			// SAFE
			MyRunnableClass_SimpleCounter->bIsStopThreadSafe = true;

			if (SimpleCounterEvent)
			{
				SimpleCounterEvent->Trigger();
				// Epic recomended to use this func for clear work cycle. It is important for Simple Counter Event, caus it should be success destroyed
				FPlatformProcess::ReturnSynchEventToPool(SimpleCounterEvent); 
				SimpleCounterEvent = nullptr;
			}

			if (SimpleCounterScopedEvent_Ref)
			{
				SimpleCounterScopedEvent_Ref->Trigger();
				SimpleCounterScopedEvent_Ref = nullptr;
			}

			CurrentRunningGameModeThread_SimpleCounter->WaitForCompletion(); // Like detach() ; Wait during thread finish work
			CurrentRunningGameModeThread_SimpleCounter = nullptr;
			MyRunnableClass_SimpleCounter = nullptr;
		}
	}
}

void AThreadExampleGameModeBase::KillSimpleCounterThread(bool bIsShouldWait)
{
	if (CurrentRunningGameModeThread_SimpleCounter)
	{
		CurrentRunningGameModeThread_SimpleCounter->Suspend(false); // Is thread was paused - continue
		CurrentRunningGameModeThread_SimpleCounter->Kill(bIsShouldWait);
		CurrentRunningGameModeThread_SimpleCounter = nullptr;
		MyRunnableClass_SimpleCounter = nullptr;
	}
}

void AThreadExampleGameModeBase::CreateSimpleCounterThread()
{
	if (bUseFEvent)
	{
		SimpleCounterEvent = FPlatformProcess::GetSynchEventFromPool(); // Create synch event
	}

	if (!CurrentRunningGameModeThread_SimpleCounter)
	{
		if (!MyRunnableClass_SimpleCounter)
		{
			MyRunnableClass_SimpleCounter = new FSimpleCounter_Runnable(ColorSimpleCounter, this, bUseSafeVariable);
		}
		CurrentRunningGameModeThread_SimpleCounter = FRunnableThread::Create(MyRunnableClass_SimpleCounter, TEXT("SimpleCounter Thread"), 0, EThreadPriority::TPri_Normal);
	}
}

int64 AThreadExampleGameModeBase::GetSimpleCounterThread()
{
	int64 result = 0;
	if (MyRunnableClass_SimpleCounter)
	{
		if (MyRunnableClass_SimpleCounter->bIsUseSafeVariable)
		{
			result = MyRunnableClass_SimpleCounter->CounterSafe.GetValue(); // Safe
		}
		else
		{
			result = MyRunnableClass_SimpleCounter->Counter; // Not safe
		}
	}
	return result;
}

bool AThreadExampleGameModeBase::SwitchRunStateSimpleCounterThread(bool bIsPause)
{
	if (CurrentRunningGameModeThread_SimpleCounter)
	{
		CurrentRunningGameModeThread_SimpleCounter->Suspend(bIsPause); // Set pause for thread
	}
	return !bIsPause;
}

void AThreadExampleGameModeBase::StartSimpleCounterThreadWithEvent()
{
	if (SimpleCounterEvent)
	{
		SimpleCounterEvent->Trigger(); // This method unpouse thread and the thread continue work
		// SimpleCounterEvent = nullptr; // Every call Triger() nedd to clear with nullptr
	}
}

void AThreadExampleGameModeBase::StartSimpleCounterThreadWithScopedEvent()
{
	if (SimpleCounterScopedEvent_Ref)
	{
		SimpleCounterScopedEvent_Ref->Trigger();
		SimpleCounterScopedEvent_Ref = nullptr; // Every call Triger() nedd to clear with nullptr
	}
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

//////////////////////////////////////////////////// Mutex ////////////////////////////////////////////////////\\*************************************

void AThreadExampleGameModeBase::CreateSimpleMutexThread()
{
	for (int i =0; i < 4; ++i)
	{
		bool bFlag = false;
		if (i % 2)
		{
			bFlag = true;
		}
		class FSimpleMutex_Runnable *MyRunnableClass_SimpleMutex = new FSimpleMutex_Runnable(ColorForThreads,this, bFlag);
		CurrentRunningGameModeThread_SimpleMutex.Add(FRunnableThread::Create(MyRunnableClass_SimpleMutex, TEXT("SimpleMutex Thread"), 0, EThreadPriority::TPri_Lowest));
	}
}

void AThreadExampleGameModeBase::CreateSimpleCollectableThread()
{
	class FSimpleCollectable_Runnable* MyRunnableClass_SimpleMutex = new FSimpleCollectable_Runnable(ColorForThreads, this);
	CurrentRunningGameModeThread_SimpleMutex.Add(FRunnableThread::Create(MyRunnableClass_SimpleMutex, TEXT("SimpleCollectable Thread"), 0, EThreadPriority::TPri_Lowest));
}


void AThreadExampleGameModeBase::StopSimpleMutexThreads()
{
	if (CurrentRunningGameModeThread_SimpleMutex.Num() > 0)
	{
		for (auto RunnableThread : CurrentRunningGameModeThread_SimpleMutex)
		{
			if (RunnableThread)
			{
				RunnableThread->Kill(true);
			}
		}
		CurrentRunningGameModeThread_SimpleMutex.Empty();
	}

	if (CurrentRunningGameModeThread_SimpleCOllectable)
	{
		CurrentRunningGameModeThread_SimpleCOllectable->Kill(true);
		CurrentRunningGameModeThread_SimpleCOllectable = nullptr;
	}
}

TArray<FString> AThreadExampleGameModeBase::GetFirstNames()
{
	{
		FScopeLock myScopedLock(&FirstNameMutex);
		return FirstNames;
	}
}

// All the names that the threads have written to the array
TArray<FString> AThreadExampleGameModeBase::GetSecondNames()
{
	TArray<FString> result;
	FString OneRead;

	while (SecondName.Dequeue(OneRead))
	{
		result.Add(OneRead);
	}

	// This array need because TQueue remove item. So need to save current item in simple array
	CurrentSecondName.Append(result);

	return CurrentSecondName;
}

TArray<FInfoNPC> AThreadExampleGameModeBase::GetNPCInfo()
{
	TArray<FInfoNPC> result;
	return result;
}

void AThreadExampleGameModeBase::EventMessage_NameGenerator(bool bIsSecondName, FString StringData)
{
	OnUpdateByNameGeneratorThreads.Broadcast(bIsSecondName, StringData);
}

void AThreadExampleGameModeBase::EventMessage_NPCInfo(FInfoNPC NPCData)
{
	OnUpdateByThreadNPC.Broadcast(NPCData);
	
	// Spawn cube
	UWorld* myWorld = GetWorld();
	if (myWorld && SpawnObjectThread)
	{
		FVector SpawnLoc = FVector(800.0f, 100.0f * cubeCout, 100.0f);
		FRotator SpawnRot;
		ADumpCuteCube *myCuteCube;
		myCuteCube = Cast<ADumpCuteCube>(myWorld->SpawnActor(SpawnObjectThread, &SpawnLoc, &SpawnRot, FActorSpawnParameters()));
		if (myCuteCube)
		{
			NPCData.Color = FColor(FMath::RandRange(0.0f,255.0f), FMath::RandRange(0.0f, 255.0f), FMath::RandRange(0.0f, 255.0f), 255);
			myCuteCube->Init(NPCData);
			cubeCout++;
		}
	}

}

// Parallel For
void AThreadExampleGameModeBase::StartParallell()
{
	FCriticalSection ParallelMutex;

	ParallelFor(10, [&](int32 index)
		{
			FPlatformProcess::Sleep(0.001f);
			int32 Cout = 0;
			for (int i =0; i < 50; ++i)
			{
				Cout++;
			}

			ParallelMutex.Lock();
			ParallelCout1 += Cout;
			ParallelMutex.Unlock();

		}), EParallelForFlags::BackgroundPriority;
}

void AThreadExampleGameModeBase::StartParallell2()
{
	const auto Function = [&](int32 index)
		{
			FPlatformProcess::Sleep(0.1f);
			int32 Cout = 0;
			for (int i = 0; i < 50; ++i)
			{
				Cout++;
			}
			ParallelCout2 += Cout;
		};

	ParallelForTemplate(10,Function, EParallelForFlags::BackgroundPriority);
}

void AThreadExampleGameModeBase::StartParallell3()
{
	ParallelForWithPreWork(10, [&](int32 index)
		{
			for (int i = 0; i < 50; ++i)
			{
				ParallelCout3++;
				UE_LOG(LogTemp, Error, TEXT(" I Start ParallelFor WithPreWork"));
			}
		}, []()
			{
				UE_LOG(LogTemp, Error, TEXT(" I Start Help Work"));
			//	FPlatformProcess::Sleep(5.0f);
				UE_LOG(LogTemp, Error, TEXT(" I Finish Help Work"));
			},EParallelForFlags::BackgroundPriority);
}
