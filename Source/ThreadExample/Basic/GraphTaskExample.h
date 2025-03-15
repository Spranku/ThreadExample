// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThreadExample/ThreadExampleGameModeBase.h"
#include "GraphTaskExample.generated.h"

DECLARE_DELEGATE_OneParam(FTaskDelegate_OnWorkDone, int32 OutputResult)

#pragma optimize("",off)
class FTask_CounterWork
{
	FTaskDelegate_OnWorkDone TaskDelegate_OnWorkDone;
	AThreadExampleGameModeBase* GameModeRef;
	int32 *SimpleOutput;
	int32 Number;
public:
	FTask_CounterWork(FTaskDelegate_OnWorkDone InTaskDelegate_OnWorkDone, 
					AThreadExampleGameModeBase *InGameModeRef, 
					int32 * InSimpleOutput, 
					int32 InNumber) : TaskDelegate_OnWorkDone(InTaskDelegate_OnWorkDone),
					GameModeRef(InGameModeRef),
					SimpleOutput(InSimpleOutput),
					Number(InNumber)
	{
		UE_LOG(LogTemp, Error, TEXT("FTask_SimpleWork Cons"));
		// Usually the constructor doesn't do anything except save the arguments for use in DoWork or GetDesiredThread.
	}

	~FTask_CounterWork()
	{
		UE_LOG(LogTemp, Error, TEXT("FTask_SimpleWork Test"));
		// you will be destroyed immediately after you execute. Might as well do cleanup in DoWork, but you could also use a destructor.
	}
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTaskCounterWork, STATGROUP_TaskGraphTasks);
	}

	static ENamedThreads::Type GetDesiredThread()
	{
		FAutoConsoleTaskPriority myTaskPriority(TEXT("FTask_Counter"),
												TEXT("FTask_Counter in working"),
												ENamedThreads::BackgroundThreadPriority,
												ENamedThreads::NormalTaskPriority,
												ENamedThreads::NormalTaskPriority);

		return myTaskPriority.Get();
	}

	static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::FireAndForget; }

	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		int32 i = 0;
		while (i < 500)
		{
			FPlatformProcess::Sleep(0.002f);
			*SimpleOutput = (*SimpleOutput + 1);
			i++;
		}

		// The arguments are useful for setting up other tasks. 
		// Do work here, probably using SomeArgument.

		MyCompletionGraphEvent->DontCompleteUntil(TGraphTask<FSomeChildTask>::CreateTask(NULL, CurrentThread).ConstructAndDispatchWhenReady());
	}
};

UCLASS()
class THREADEXAMPLE_API AGraphTaskExample : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGraphTaskExample();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Counter = 0;

	FTaskDelegate_OnWorkDone TaskDelegate;

	UFUNCTION(BlueprintNativeEvent)
	void OnWorkDone_BP(int32 Result);

	UFUNCTION()
	void OnWorkDone(int32 Result);

};
#pragma optimize("",on)