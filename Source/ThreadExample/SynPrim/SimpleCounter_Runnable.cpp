// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleCounter_Runnable.h"

FSimpleCounter_Runnable::FSimpleCounter_Runnable(FColor Color, AThreadExampleGameModeBase* OwnerActor)
{
}

FSimpleCounter_Runnable::~FSimpleCounter_Runnable()
{
}

bool FSimpleCounter_Runnable::Init()
{
	return true;
}

uint32 FSimpleCounter_Runnable::Run()
{
	while (!bIsStopThread)
	{
		Counter++;
	}
	return 1;
}

void FSimpleCounter_Runnable::Stop()
{
}

void FSimpleCounter_Runnable::Exit()
{
}
