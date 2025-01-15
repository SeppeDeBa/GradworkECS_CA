// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pixel.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

UCLASS()
class ECSGRADWORKSEPPEDB_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();
	//generations
	void GeneratePixels();
	void GenerateNext();

	//timer
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GridVariables")
	bool UpdateEveryFrame = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GridVariables")
	float UpdateTimerMax = 1.f;
	float m_CurrTimer = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Measuring")
	bool measure1000Generations = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Measuring")
	bool measure10kGeneration = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Measuring")
	bool measureGameOfLifeTicks = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Measuring")
	bool doDebugVisualisation = false;
	
	double startTime;
	double thousandGenerationTime;

	double totalGameOfLifeGridCheckTimes;
	//GridParams
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GridVariables")
	float GridSize = 1000.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GridVariables")
	int Cols = 16; //cols are equal to rows
	
private:
	bool m_IsGridRunning = false;

	//helper functions
	void FlipIsGridRunning();
	void FlipUpdateEveryFrame();
	void DebugLogInfo();
	TSubclassOf<class APixel> m_PixelClass;
	APixel* m_pCurrPixel;
	TArray<APixel*> m_GridPtrArray;
	int m_CurrAlivePixels = 0;
	int m_CurrGeneration = 0;
	APixel* GetPixelFromGrid(int x, int y) const;
	bool m_IsGridCreated = false;

	int GetAmtPixelNeighbours(int x, int y) const;
	APlayerController* player1Controller;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void OnMouseClicked(const FVector& ClickLocation);
};
