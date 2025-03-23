// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeGenerator.h"

// Sets default values
AMazeGenerator::AMazeGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMazeGenerator::BeginPlay()
{
	Super::BeginPlay();
    GenerateMaze();
    VisualizeMaze();
	
}

// Called every frame
void AMazeGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMazeGenerator::GenerateMaze()
{
    Maze.Empty();
    TArray<FCell> CurrentRow;
    InitializeFirstRow(CurrentRow);

    for (int32 y = 0; y < MazeHeight; y++)
    {
        RemoveVerticalWalls(CurrentRow);
        CreateHorizontalPassages(CurrentRow);
        Maze.Add(CurrentRow);

        if (y < MazeHeight - 1)
        {
            TArray<FCell> NextRow;
            MoveToNextRow(CurrentRow, NextRow);
            CurrentRow = NextRow;
        }
    }

    FinalizeMaze();
}

void AMazeGenerator::VisualizeMaze()
{
    if (!WallBlueprint || !FloorBlueprint) return;

    for (int32 y = 0; y < MazeHeight; y++)
    {
        for (int32 x = 0; x < MazeWidth; x++)
        {
            // Пол
            FVector FloorLocation = FVector(x * CellSize, y * CellSize, 0);
            GetWorld()->SpawnActor<AActor>(FloorBlueprint, FloorLocation, FRotator::ZeroRotator);

            // Правая стена
            if (Maze[y][x].RightWall)
            {
                FVector WallLocation = FVector((x + 0.5) * CellSize, y * CellSize, 0);
                GetWorld()->SpawnActor<AActor>(WallBlueprint, WallLocation, FRotator::ZeroRotator);
            }

            // Нижняя стена
            if (Maze[y][x].BottomWall)
            {
                FVector WallLocation = FVector(x * CellSize, (y + 0.5) * CellSize, 0);
                GetWorld()->SpawnActor<AActor>(WallBlueprint, WallLocation, FRotator::ZeroRotator);
            }
        }
    }
}

void AMazeGenerator::InitializeFirstRow(TArray<FCell>& Row)
{
    Row.Empty();
    for (int32 i = 0; i < MazeWidth; i++)
    {
        FCell Cell;
        Cell.SetID = i; // Уникальный SetID
        Cell.RightWall = true;
        Cell.BottomWall = true;
        Row.Add(Cell);
    }
}

void AMazeGenerator::RemoveVerticalWalls(TArray<FCell>& Row)
{
    for (int32 i = 0; i < MazeWidth - 1; i++)
    {
        if (Row[i].SetID != Row[i + 1].SetID && FMath::RandBool())
        {
            Row[i].RightWall = false;
            MergeSets(Row, Row[i].SetID, Row[i + 1].SetID);
        }
    }
}

void AMazeGenerator::CreateHorizontalPassages(TArray<FCell>& Row)
{
    TMap<int32, bool> HasPassage; // Храним, есть ли проход в множестве

    // Сначала случайно удаляем нижние стены
    for (int32 i = 0; i < MazeWidth; i++)
    {
        if (FMath::RandBool())
        {
            Row[i].BottomWall = false;
            HasPassage.Add(Row[i].SetID, true); // Отмечаем, что в множестве есть проход
        }
    }

    // Гарантируем, что в каждом множестве есть хотя бы один проход
    for (int32 i = 0; i < MazeWidth; i++)
    {
        if (!HasPassage.Contains(Row[i].SetID))
        {
            // Находим первую ячейку в множестве и удаляем нижнюю стену
            for (int32 j = 0; j < MazeWidth; j++)
            {
                if (Row[j].SetID == Row[i].SetID)
                {
                    Row[j].BottomWall = false;
                    HasPassage.Add(Row[j].SetID, true); // Отмечаем, что в множестве есть проход
                    break;
                }
            }
        }
    }
}

void AMazeGenerator::MoveToNextRow(TArray<FCell>& CurrentRow, TArray<FCell>& NextRow)
{
    NextRow.Empty();
    for (int32 i = 0; i < MazeWidth; i++)
    {
        FCell Cell;
        if (CurrentRow[i].BottomWall)
        {
            Cell.SetID = FMath::Rand(); // Новый уникальный SetID
        }
        else
        {
            Cell.SetID = CurrentRow[i].SetID; // Сохраняем SetID
        }
        Cell.RightWall = true;
        Cell.BottomWall = true;
        NextRow.Add(Cell);
    }
}

void AMazeGenerator::FinalizeMaze()
{
    TArray<FCell>& LastRow = Maze[MazeHeight - 1];
    for (int32 i = 0; i < MazeWidth - 1; i++)
    {
        if (LastRow[i].SetID != LastRow[i + 1].SetID)
        {
            LastRow[i].RightWall = false;
            MergeSets(LastRow, LastRow[i].SetID, LastRow[i + 1].SetID);
        }
    }
}

void AMazeGenerator::MergeSets(TArray<FCell>& Row, int32 SetID1, int32 SetID2)
{
    for (FCell& Cell : Row)
    {
        if (Cell.SetID == SetID2)
        {
            Cell.SetID = SetID1; // Объединяем множества
        }
    }
}

