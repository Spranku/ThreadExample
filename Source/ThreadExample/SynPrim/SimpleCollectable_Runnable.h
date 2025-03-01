// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MessageEndpointBuilder.h"

class AThreadExampleGameModeBase;

/**
 * 
 */
class THREADEXAMPLE_API FSimpleCollectable_Runnable : public FRunnable
{
public:

	FSimpleCollectable_Runnable(FColor Color, AThreadExampleGameModeBase* OwnerActor);
	~FSimpleCollectable_Runnable();

	//virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Exit() override;

	AThreadExampleGameModeBase* GameMode_Ref = nullptr;

	FThreadSafeBool bIsStopCollectable = false;

	// Work with widget.This thread will send info for spawnable actors
	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> SenderEndPoint;
};
