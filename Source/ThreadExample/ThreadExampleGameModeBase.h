// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HAL/ThreadingBase.h"
#include "SynPrim/SimpleAtomic_Runnable.h"
#include "SynPrim/SimpleCounter_Runnable.h"
#include "ThreadExampleGameModeBase.generated.h"

USTRUCT(BlueprintType, Atomic)
struct FInfoNPC
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Id = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name = "none";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SecondName = "none";
};

/**
 * 
 */
UCLASS()
class THREADEXAMPLE_API AThreadExampleGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	// SimpleCounter setting
	UPROPERTY(BlueprintReadWrite)
	bool bUseSafeVariable = true;

	UPROPERTY(BlueprintReadWrite)
	FColor ColorSimpleCounter;

	class FSimpleCounter_Runnable* MyRunnableClass_SimpleCounter = nullptr;
	FRunnableThread* CurrentRunningGameModeThread_SimpleCounter = nullptr;

	FEvent* SimpleCounterEvent;

	FScopedEvent* SimpleCounterScopedEvent_Ref; 

	void SendRef_ScopedEvent(FScopedEvent& ScopedEvent_Ref);
	
	UPROPERTY(BlueprintReadWrite)
	bool bUseFEvent = true;

	UPROPERTY(BlueprintReadWrite)
	bool bUseScopedEvent = true;

	// SimpleCounter control
	UFUNCTION(BlueprintCallable)
	void StopSimpleCounterThread();

	UFUNCTION(BlueprintCallable)
	void KillSimpleCounterThread(bool bIsShouldWait);

	UFUNCTION(BlueprintCallable)
	void CreateSimpleCounterThread();

	UFUNCTION(BlueprintCallable)
	int64 GetSimpleCounterThread();

	UFUNCTION(BlueprintCallable)
	bool SwitchRunStateSimpleCounterThread(bool bIsPause);

	UFUNCTION(BlueprintCallable)
	void StartSimpleCounterThreadWithEvent();

	UFUNCTION(BlueprintCallable)
	void StartSimpleCounterThreadWithScopedEvent();

	// SimpleAtomic setting
	TArray<FRunnableThread*> CurrentRunnableGameModeThread_SimpleAtomic;

	UPROPERTY(BlueprintReadWrite, Category = "SimpleAtomic Setting")
	int32 IterationRunnableCircle = 100000;

	UPROPERTY(BlueprintReadWrite, Category = "SimpleAtomic Setting")
	int32 NumberOfThreadToCreate = 12;

	UPROPERTY(BlueprintReadWrite, Category = "SimpleAtomic Setting")
	bool bIsUseAtomic = true;

	//	SimpleAtomic control
	UFUNCTION(BlueprintCallable)
	void CreateSimpleAtomicThread();

	UFUNCTION(BlueprintCallable)
	void GetCounterSimpleAtomic(int32 &Atomic1, int32 &Atomic2, int32 &NonAtomic1, int32 &NonAtomic2);

	UFUNCTION(BlueprintCallable)
	void ResetCounterSimpleAtomic();

	std::atomic_int16_t AtomicCounter1; // C++ class

	int16 NonAtomicCounter1;

	std::atomic_int16_t AtomicCounter2; // C++ class

	int16 NonAtomicCounter2;
	
	// SimpleMutex setting
 
	// Array of running threads. they generate data
	TArray<FRunnableThread*> CurrentRunningGameModeThread_SimpleMutex; 

	// Thread for save data
	FRunnableThread* CurrentRunningGameModeThread_SimpleCOllectable;

	UFUNCTION(BlueprintCallable)
	void CreateSimpleMutexThread();

	UFUNCTION(BlueprintCallable)
	void CreateSimpleCollectableThread();

	UFUNCTION(BlueprintCallable)
	void StopSimpleMutexThreads();

	UFUNCTION(BlueprintCallable)
	TArray<FString> GetFirstNames();

	UFUNCTION(BlueprintCallable)
	TArray<FString> GetSecondNames();

	UFUNCTION(BlueprintCallable)
	TArray<FInfoNPC> GetNPCInfo();

	// SimpleMutex storage
	TArray<FString> FirstName;

	FCriticalSection FirstNameMutex; // Since FirstName will be in threads running, need to use mutex for control

	// TQueue - linked list for safe work with threads. 
	// Work mode: Mpsc - Multiply producer, single consumer
	// Work mode: Spsc - Single producer, single consumer
	// Since SecondName will be work with any threads, need to use Mpsc work mode
	TQueue<FString, EQueueMode::Mpsc> SecondName;

	FColor ColorForThreads;
};
