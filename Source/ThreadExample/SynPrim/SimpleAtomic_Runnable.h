// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThreadExample/ThreadExampleGameModeBase.h"

/**
 * 
 */
class THREADEXAMPLE_API FSimpleAtomic_Runnable : public FRunnable
{
public:
	FSimpleAtomic_Runnable(FColor Color, AThreadExampleGameModeBase *OwnerActor, uint32 NeedIteration);

	virtual ~FSimpleAtomic_Runnable() override;

	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Exit() override;


};
