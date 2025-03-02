// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleCollectable_Runnable.h"
#include "ThreadExample/ThreadExampleGameModeBase.h"


FSimpleCollectable_Runnable::FSimpleCollectable_Runnable(FColor Color, AThreadExampleGameModeBase* OwnerActor)
{
	GameMode_Ref = OwnerActor;

	SenderEndPoint = FMessageEndpoint::Builder("Sender_FSimpleCollectable_Runnable").Build();
}

FSimpleCollectable_Runnable::~FSimpleCollectable_Runnable()
{
}

uint32 FSimpleCollectable_Runnable::Run()
{
	uint32 i = 0;
	while (!bIsStopCollectable)
	{
		FPlatformProcess::Sleep(2.0f); // Imitation work load

		FInfoNPC newNPC;
		newNPC.Id = i;
		i++;
		newNPC.Age = FMath::Rand();

		int32 sizeNames = GameMode_Ref->FirstNames.Num();

		if (sizeNames > 0)
		{
			uint32 randNameIndex = FMath::RandHelper(sizeNames - 1);

			GameMode_Ref->FirstNameMutex.Lock();
			newNPC.Name = GameMode_Ref->FirstNames[randNameIndex];
			GameMode_Ref->FirstNames.RemoveAt(randNameIndex);
			GameMode_Ref->FirstNameMutex.Unlock();
		}

		TArray<FString> AviableSecondNames = GameMode_Ref->GetSecondNames();
		int32 SizeSecondNames = AviableSecondNames.Num();

		if (SizeSecondNames > 0)
		{
			int32 randNameIndex = FMath::RandHelper(SizeSecondNames - 1);
			newNPC.SecondName = AviableSecondNames[randNameIndex];
			GameMode_Ref->CurrentSecondName.Remove(newNPC.SecondName);
		}

		{ // Lock
			FScopeLock NpcScopedLock(&GameMode_Ref->NPCInfoMutex); // Create critical section
			GameMode_Ref->NPCInfo.Add(newNPC);
		} // End lock

		if (SenderEndPoint.IsValid())
		{
			SenderEndPoint->Publish<FInfoNPC>(new FInfoNPC(newNPC));
		}
	}

	return 1;
}

void FSimpleCollectable_Runnable::Stop()
{
	bIsStopCollectable = true;
}

void FSimpleCollectable_Runnable::Exit()
{
	if(SenderEndPoint.IsValid())
	{
		SenderEndPoint.Reset();
	}
}
