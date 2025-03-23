// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeGenerator.generated.h"

USTRUCT()
struct FCell
{
	GENERATED_BODY()

public:
	int32 SetID;        // ������������� ���������
	bool RightWall;     // ���� �� ����� ������
	bool BottomWall;    // ���� �� ����� �����
};

UCLASS()
class THREADEXAMPLE_API AMazeGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMazeGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ��������� ���������
	void GenerateMaze();

	// ������������ ���������
	void VisualizeMaze();

    UPROPERTY(EditAnywhere, Category = "Maze")
    TSubclassOf<AActor> WallBlueprint;

    UPROPERTY(EditAnywhere, Category = "Maze")
    TSubclassOf<AActor> FloorBlueprint;

    // ������� ���������
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
    int32 MazeWidth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
    int32 MazeHeight;

    // ������ ������
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
    float CellSize;

    // �������� (��������� ������ �����)
    TArray<TArray<FCell>> Maze;

    // ������������� ������ ������
    void InitializeFirstRow(TArray<FCell>& Row);

    // �������� ������������ ����
    void RemoveVerticalWalls(TArray<FCell>& Row);

    // �������� �������������� ��������
    void CreateHorizontalPassages(TArray<FCell>& Row);

    // ������� � ��������� ������
    void MoveToNextRow(TArray<FCell>& CurrentRow, TArray<FCell>& NextRow);

    // ���������� ���������
    void FinalizeMaze();

    // ����������� ��������
    void MergeSets(TArray<FCell>& Row, int32 SetID1, int32 SetID2);
};
