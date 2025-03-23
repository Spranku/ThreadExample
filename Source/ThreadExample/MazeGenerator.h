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
	int32 SetID;        // Идентификатор множества
	bool RightWall;     // Есть ли стена справа
	bool BottomWall;    // Есть ли стена снизу
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

	// Генерация лабиринта
	void GenerateMaze();

	// Визуализация лабиринта
	void VisualizeMaze();

    UPROPERTY(EditAnywhere, Category = "Maze")
    TSubclassOf<AActor> WallBlueprint;

    UPROPERTY(EditAnywhere, Category = "Maze")
    TSubclassOf<AActor> FloorBlueprint;

    // Размеры лабиринта
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
    int32 MazeWidth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
    int32 MazeHeight;

    // Размер ячейки
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
    float CellSize;

    // Лабиринт (двумерный массив ячеек)
    TArray<TArray<FCell>> Maze;

    // Инициализация первой строки
    void InitializeFirstRow(TArray<FCell>& Row);

    // Удаление вертикальных стен
    void RemoveVerticalWalls(TArray<FCell>& Row);

    // Создание горизонтальных проходов
    void CreateHorizontalPassages(TArray<FCell>& Row);

    // Переход к следующей строке
    void MoveToNextRow(TArray<FCell>& CurrentRow, TArray<FCell>& NextRow);

    // Завершение лабиринта
    void FinalizeMaze();

    // Объединение множеств
    void MergeSets(TArray<FCell>& Row, int32 SetID1, int32 SetID2);
};
