// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleCollectable_Runnable.h"
#include "ThreadExample/ThreadExampleGameModeBase.h"

FSimpleCollectable_Runnable::FSimpleCollectable_Runnable(FColor Color, AThreadExampleGameModeBase* OwnerActor)
{
}

FSimpleCollectable_Runnable::~FSimpleCollectable_Runnable()
{
}

uint32 FSimpleCollectable_Runnable::Run()
{
	return 1;
}

void FSimpleCollectable_Runnable::Exit()
{
}
