// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HAL/ThreadingBase.h"
#include "SynPrim/SimpleAtomic_Runnable.h"
#include "SynPrim/SimpleCounter_Runnable.h"
#include "IMessageBus.h"
#include "MessageEndpoint.h"
#include "ThreadExampleGameModeBase.generated.h"

class ADumpCuteCube;

USTRUCT(BlueprintType)
struct FInfoNPC
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Id = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name = "none";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SecondName = "none";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Age = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor Color;
};

USTRUCT()
struct FBusStructMessage_NameGenerator
{
	GENERATED_BODY()

	bool bIsSecondName = false;

	FString TextName = "none";

	FBusStructMessage_NameGenerator(bool InBool = false, FString InText = "None") : bIsSecondName(InBool), TextName(InText) {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateByNameGeneratorThreads,
											 bool,
											 bIsSecondFString,
											 FString,
											 StringData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateByThreadNPC, FInfoNPC, DataNPC);

/**
 * 
 */
UCLASS()
class THREADEXAMPLE_API AThreadExampleGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnUpdateByNameGeneratorThreads OnUpdateByNameGeneratorThreads;

	UPROPERTY(BlueprintAssignable)
	FOnUpdateByThreadNPC OnUpdateByThreadNPC;

	void BusMessageHandler_NameGenerator(const struct FBusStructMessage_NameGenerator& Message, 
										 const TSharedRef<IMessageContext, 
									     ESPMode::ThreadSafe>& Context);

	void BusMessageHandler_NPCInfo(const struct FInfoNPC& Message,
								   const TSharedRef<IMessageContext,
								   ESPMode::ThreadSafe>& Context);

	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> ReceiveEndPoint_NameGenerator;

	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> ReceiveEndPoint_NPCInfo;

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


	void EventMessage_NameGenerator(bool bIsSecondName, FString StringData);

	void EventMessage_NPCInfo(FInfoNPC NPCData);

	// SimpleMutex storage
	TArray<FString> FirstNames;

	FCriticalSection FirstNameMutex; // Since FirstName will be in threads running, need to use mutex for control

	// TQueue - linked list for safe work with threads. 
	// Work mode: Mpsc - Multiply producer, single consumer
	// Work mode: Spsc - Single producer, single consumer
	// Since SecondName will be work with any threads, need to use Mpsc work mode
	TQueue<FString, EQueueMode::Mpsc> SecondName;
	
	TArray<FString> CurrentSecondName;

	TArray<FInfoNPC> NPCInfo;

	FCriticalSection NPCInfoMutex;

	FColor ColorForThreads;

	int32 cubeCout = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ADumpCuteCube> SpawnObjectThread;
};
