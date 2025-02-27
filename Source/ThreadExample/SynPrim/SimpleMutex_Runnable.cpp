// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleMutex_Runnable.h"
#include "ThreadExample/ThreadExampleGameModeBase.h"

FSimpleMutex_Runnable::FSimpleMutex_Runnable(FColor Color, AThreadExampleGameModeBase* OwnerActor, bool bIsSecondMode)
{

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
	return 1;
}

////void FSimpleMutex_Runnable::Stop()
//{
//}

void FSimpleMutex_Runnable::Exit()
{
}
