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

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	// SimpleAtomic setting
	TArray<FRunnableThread*> CurrentRunnableGameModeThread_SimpleAtomic;

	//	SimpleAtomic control
	UFUNCTION(BlueprintCallable)
	void CreateSimpleAtomicThread();
	
};
