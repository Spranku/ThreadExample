// Fill out your copyright notice in the Description page of Project Settings.


#include "DumpCuteCube.h"

// Sets default values
ADumpCuteCube::ADumpCuteCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADumpCuteCube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADumpCuteCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADumpCuteCube::Init(FInfoNPC InitInfo)
{
	InitBP(InitInfo);
}

void ADumpCuteCube::InitBP_Implementation(FInfoNPC InitInfo)
{
	// in BP
}

