// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HAL/ThreadingBase.h"
#include "ThreadExampleGameModeBase.generated.h"

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
	
};
