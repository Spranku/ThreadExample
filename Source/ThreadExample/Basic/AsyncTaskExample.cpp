// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncTaskExample.h"
#include "HAL/ThreadManager.h"

class FAsyncTask_Counter : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<FAsyncTask_Counter>;

	int32 ExampleData;
	FAsyncTask_OnWorkDone AsyncTask_OnWorkDone;
	int32* SimpleOutput;

	FAsyncTask_Counter(int32 InExampleData, FAsyncTask_OnWorkDone InAsyncTaskDelegate_OnWorkDone, int32* InSimpleOutput) 
		: ExampleData(InExampleData), AsyncTask_OnWorkDone(InAsyncTaskDelegate_OnWorkDone), SimpleOutput(InSimpleOutput)
	{
	}

	void DoWork()
	{
		uint64 i = 0;
		while (i < 500)
		{
			FPlatformProcess::Sleep(0.002f);
			*SimpleOutput = (*SimpleOutput + 1);
			i++;
		}
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FAsyncTask_Counter, STATGROUP_ThreadPoolAsyncTasks);
	}
};

// Sets default values
AAsyncTaskExample::AAsyncTaskExample()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAsyncTaskExample::BeginPlay()
{
	Super::BeginPlay();

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [&]()
		{
			int32 i = 0;
			while (i < 50)
			{
				FPlatformProcess::Sleep(0.02f);
				i++;
				Counter2 = i;
			}
		});
}

// Called every frame
void AAsyncTaskExample::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAsyncTaskExample::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AAsyncTaskExample::StartExample(bool bIsBackGroundTask)
{
	if (bIsBackGroundTask)
	{
		(new FAutoDeleteAsyncTask<FAsyncTask_Counter>(5, AsyncTask_OnWorkDone, &Counter))->StartBackgroundTask();
	}
	else
	{
		(new FAutoDeleteAsyncTask<FAsyncTask_Counter>(5, AsyncTask_OnWorkDone, &Counter))->StartSynchronousTask();
	}
}

