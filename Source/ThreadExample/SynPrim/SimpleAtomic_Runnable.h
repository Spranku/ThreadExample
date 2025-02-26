// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThreadExample/ThreadExampleGameModeBase.h"

class AThreadExampleGameModeBase;
/**
 * 
 */
class THREADEXAMPLE_API FSimpleAtomic_Runnable : public FRunnable
{
public:
	FSimpleAtomic_Runnable(FColor Color, 
						   AThreadExampleGameModeBase *OwnerActor, 
		                   uint32 NeedIteration, 
		                   bool SeprateLogic, 
		                   bool bIsUseAtomic);

	virtual ~FSimpleAtomic_Runnable() override;

	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Exit() override;

	int NumberIteration = 0;

	AThreadExampleGameModeBase* GameMode_Ref = nullptr;

	bool bIsStopThread = false;

	bool bUseAtomicFlag = true;

	bool SeprateLogicFlag = false;

};
