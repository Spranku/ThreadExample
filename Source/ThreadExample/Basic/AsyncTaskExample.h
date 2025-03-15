// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AsyncTaskExample.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAsyncTask_OnWorkDone, int32, OutputResult);

UCLASS()
class THREADEXAMPLE_API AAsyncTaskExample : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAsyncTaskExample();

	UPROPERTY(BlueprintAssignable)
	FAsyncTask_OnWorkDone AsyncTask_OnWorkDone;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Counter = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Counter2 = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	void StartExample(bool bIsBackGroundTask = true);

};
