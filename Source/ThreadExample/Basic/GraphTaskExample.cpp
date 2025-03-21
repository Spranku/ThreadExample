// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphTaskExample.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGraphTaskExample::AGraphTaskExample()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGraphTaskExample::BeginPlay()
{
	Super::BeginPlay();

	/*int32 MainIdThread = FPlatformTLS::GetCurrentThreadId();
	FString NameThread = FThreadManager::GetThreadName(MainIdThread);
	UE_LOG(LogTemp, Error, TEXT("FTask_SimpleWork NameThread -%s ID - %d"),*NameThread,MainIdThread);
	UE_LOG(LogTemp, Error, TEXT("FTask_SimpleWork NumberOfCores - %d"), FPlatformMisc::NumberOfCores());
	UE_LOG(LogTemp, Error, TEXT("FTask_SimpleWork NumberOfCoresIncludingHyperthreads - %d"), FPlatformMisc::NumberOfCoresIncludingHyperthreads());*/

	TaskDelegate.BindUFunction(this, FName("OnWorkDone"));

	AThreadExampleGameModeBase* myGameModeRef = Cast<AThreadExampleGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	myCurrentTask = TGraphTask<FTask_CounterWork>::CreateTask(nullptr, ENamedThreads::AnyThread).ConstructAndHold(TaskDelegate, myGameModeRef, &Counter);
}

// Called every frame
void AGraphTaskExample::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGraphTaskExample::OnWorkDone_BP_Implementation(int32 Result)
{
	// BP
}

void AGraphTaskExample::OnWorkDone(int32 Result)
{
	myCurrentTask = nullptr;
	OnWorkDone_BP(Result);
}

void AGraphTaskExample::StartAsyncWork()
{
	if (myCurrentTask)
	{
		if (!myCurrentTask->GetCompletionEvent().IsValid())
		{
			myCurrentTask->Unlock();
		}
	}
	else
	{
		AThreadExampleGameModeBase* myGameModeRef = Cast<AThreadExampleGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		TGraphTask<FTask_CounterWork>::CreateTask(nullptr, ENamedThreads::AnyThread).ConstructAndDispatchWhenReady(TaskDelegate, myGameModeRef, &Counter);
	}
}



