// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleMutex_Runnable.h"
#include "ThreadExample/ThreadExampleGameModeBase.h"
#include "random"
#include "MessageEndpointBuilder.h"

FSimpleMutex_Runnable::FSimpleMutex_Runnable(FColor Color, AThreadExampleGameModeBase* OwnerActor, bool bIsSecondMode)
{
	GameMode_Ref = OwnerActor;
	bIsGenerateSecondName = bIsSecondMode;

	SenderEndPoint = FMessageEndpoint::Builder("Sender_FSimpleMutex_Runnable").Build();
}

FSimpleMutex_Runnable::~FSimpleMutex_Runnable()
{

}

bool FSimpleMutex_Runnable::Init()
{
	return true;
}

uint32 FSimpleMutex_Runnable::Run()
{
	TArray<FString> VowelLetter{ "a","e","i","0","u","y" };

	TArray<FString> ConstantLetter{ "b","c","d","h","j","k" };

	while (!bIsStopNameGenerator)
	{
		int8 RandomSizeName;

		FString Result;

		bool bIsStartWithVowel = GetRandom();

		if (bIsGenerateSecondName)
		{
			RandomSizeName = GetRandom(5,8);
		}
		else
		{
			RandomSizeName = GetRandom(3, 5);
		}

		for (int i  = 0; i < RandomSizeName; ++i)
		{
			bool FlipFlop = false;

			if (i % 2)
			{
				FlipFlop = true;
			}

			if (bIsStartWithVowel)
			{
				if (FlipFlop)
				{
					Result.Append(VowelLetter[GetRandom(0, VowelLetter.Num() - 1)]);
				}
				else
				{
					Result.Append(ConstantLetter[GetRandom(0, VowelLetter.Num() - 1)]);
				}
			}
			else
			{
				if (FlipFlop)
				{
					Result.Append(ConstantLetter[GetRandom(0, VowelLetter.Num() - 1)]);
				}
				else
				{
					Result.Append(VowelLetter[GetRandom(0, VowelLetter.Num() - 1)]);
				}
			}
		}


		FPlatformProcess::Sleep(1.8f);

		if (bIsGenerateSecondName)
		{
			GameMode_Ref->SecondName.Enqueue(Result); // Safe for threads
		}
		else
		{
			GameMode_Ref->FirstNameMutex.Lock();


			GameMode_Ref->FirstNames.Add(Result); // Not safe for threads

			GameMode_Ref->FirstNameMutex.Unlock();
		}

		if (SenderEndPoint.IsValid())
		{
			// Send message 
			SenderEndPoint->Publish<FBusStructMessage_NameGenerator>(new FBusStructMessage_NameGenerator(bIsGenerateSecondName, Result));
		}
	}
	return 1;
}

void FSimpleMutex_Runnable::Stop()
{
	bIsStopNameGenerator = true;
}

void FSimpleMutex_Runnable::Exit()
{
	GameMode_Ref = nullptr;
}

int8 FSimpleMutex_Runnable::GetRandom(int8 min, int8 max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<>  distrib(min, max);

	return distrib(gen);
}

bool FSimpleMutex_Runnable::GetRandom()
{
	std::random_device rd;  
	std::mt19937 gen(rd()); 
	std::bernoulli_distribution  d(0.5f);
	
	return d(gen);
}
