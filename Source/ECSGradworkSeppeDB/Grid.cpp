// Fill out your copyright notice in the Description page of Project Settings.

#define LOG(Message, ...) UE_LOG(LogCategory, LogLevel, TEXT(Message), __VA_ARGS__)
#include "Grid.h"

// Sets default values
AGrid::AGrid()
{
	m_pCurrPixel = nullptr;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UClass> pixelBP(TEXT("Blueprint'/Game/BPs/BP_Pixel.BP_Pixel_C'"));
	if (pixelBP.Object)
	{
		m_PixelClass = pixelBP.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PixelClass is not valid!"));
	}
}


APixel* AGrid::GetPixelFromGrid(int x, int y) const
{
	ensure(x >= 0 && x < Cols);
	ensure(y >= 0 && y < Cols);
	return m_GridPtrArray[x + y * Cols];
}



int AGrid::GetAmtPixelNeighbours(int x, int y) const
{
	int neighbours = 0;

	for(int currentXChecking = x-1; currentXChecking <= x+1; ++currentXChecking)
	{
		//check if Y is in bounds
		if(currentXChecking < 0 || currentXChecking >= Cols) continue;
		
		//go through collumns
		for(int currentYChecking = y-1; currentYChecking <= y+1; ++currentYChecking)
		{
			//check if Y is in bounds
			if(currentYChecking < 0 || currentYChecking >= Cols) continue;

			//check if it's not the checked cell, if it is then early out
			if(currentXChecking == x && currentYChecking == y) continue;
			
			//update neighbor if its alive
			if(GetPixelFromGrid(currentXChecking, currentYChecking)->GetCurrentAlive()) ++neighbours;
			
		}
	}
	return neighbours;
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	GeneratePixels();
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GenerateNext();

	
	// if(m_IsGridCreated)
	// {
	// 	m_CurrTimer += DeltaTime;
	// 	if(m_CurrTimer >= m_UpdateTimerMax)
	// 	{
	// 		m_CurrTimer = 0.f;
	// 		GenerateNext();
	// 	}
	// }
}
void AGrid::GeneratePixels()
{
	m_IsGridCreated = false;
	m_GridPtrArray.Empty();
	FActorSpawnParameters spawnInfo;
	FVector spawnLocation = FVector(0.0f, 0.0f, 0.0f);
	FRotator spawnRotation = FRotator(0.0f, 0.0f, 0.0f);

	//fix cellSize, mesh is base size 100
	float cellSize = GridSize / Cols;
	float scaleFactor = cellSize / 100.f;//100.f is the default for static meshes 

	for (int32 y = 0; y < Cols; y++) {       
		for (int32 x = 0; x < Cols; x++) { 		
			spawnLocation = FVector( -GridSize/2.f +  x * cellSize, -GridSize/2.f + y * cellSize,10.0f);			
			APixel* newPixel = GetWorld()->SpawnActor<APixel>(m_PixelClass, spawnLocation, spawnRotation, spawnInfo);
			if(newPixel)
			{
				m_pCurrPixel = newPixel;
				//m_pCurrPixel->SetNewType(EMPTY);
				m_pCurrPixel->SetActorScale3D(FVector(scaleFactor));
				if((y%3 != 0) && (x>0 && x < Cols-1)) m_pCurrPixel->ForceAlive();
				m_GridPtrArray.Add(newPixel);
			}
		}
	}
	m_IsGridCreated = true;
}

void AGrid::GenerateNext()
{
	for (int32 gridY = 0; gridY < Cols; gridY++)
	{
		for (int32 gridX = 0; gridX < Cols; gridX++)
		{
			m_pCurrPixel = GetPixelFromGrid(gridX, gridY);
			m_pCurrPixel->DoGameOfLifeLoop(GetAmtPixelNeighbours(gridX, gridY));
		}
	}

	for (int32 gridY = 0; gridY < Cols; gridY++)
	{
		for (int32 gridX = 0; gridX < Cols; gridX++)
		{
			m_pCurrPixel = GetPixelFromGrid(gridX, gridY);
			m_pCurrPixel->UpdatePixelAliveStatus();
		}
	}
	
}

