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

bool AGrid::UpdateSandPixel(APixel* processedPixel, int xGridPos, int yGridPos)
{
	int lowerGridPos = yGridPos + 1;
	if(lowerGridPos >= Cols) return false; //go out if its at bottom as sand can only go down

	//do sand logics
	//check under
	APixel* checkingPixel = GetPixelFromGrid(xGridPos, lowerGridPos);
	if(checkingPixel->GetType() == EMPTY)
	{
		checkingPixel->SetNewType(SAND);
		processedPixel->SetNewType(EMPTY);
		return true;
	}
	//check under left
	if(xGridPos-1 >= 0)//check if valid in grid
	{
		checkingPixel = GetPixelFromGrid(xGridPos-1, lowerGridPos);
		if(checkingPixel->GetType() == EMPTY)
		{
			checkingPixel->SetNewType(SAND);
			processedPixel->SetNewType(EMPTY);
			return true;
		}
	}
	//check under right
	if(xGridPos+1 < Cols)//check if valid in grid
	{
		checkingPixel = GetPixelFromGrid(xGridPos+1, lowerGridPos);
		if(checkingPixel->GetType() == EMPTY)
		{
			checkingPixel->SetNewType(SAND);
			processedPixel->SetNewType(EMPTY);
			return true;
		}
	}
	return false;
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
	if(m_IsGridCreated)
	{
		m_CurrTimer += DeltaTime;
		if(m_CurrTimer >= m_UpdateTimerMax)
		{
			m_CurrTimer = 0.f;
			GenerateNext();
		}
	}
}
void AGrid::GeneratePixels()
{
	m_IsGridCreated = false;
	m_GridPtrArray.Empty();
	FActorSpawnParameters spawnInfo;
	FVector spawnLocation = FVector(0.0f, 0.0f, 0.0f);
	FRotator spawnRotation = FRotator(0.0f, 0.0f, 0.0f);
	int currIdx = 0;

	//fix cellSize, mesh is base size 100
	float cellSize = GridSize / Cols;
	float scaleFactor = cellSize / 100.f;//100.f is the default for static meshes 
	auto parentX = this->GetTransform().GetLocation().X;
	auto parentY = this->GetTransform().GetLocation().Y;
	for (int32 y = 0; y < Cols; y++) {       
		for (int32 x = 0; x < Cols; x++) { 		
			++currIdx;
			spawnLocation = FVector( -GridSize/2.f +  x * cellSize, -GridSize/2.f + y * cellSize,10.0f);			
			APixel* newPixel = GetWorld()->SpawnActor<APixel>(m_PixelClass, spawnLocation, spawnRotation, spawnInfo);
			if(newPixel)
			{
				m_pCurrPixel = newPixel;
				if(y < 2 && x > 4 && x < 16) m_pCurrPixel->SetNewType(SAND);
				else m_pCurrPixel->SetNewType(EMPTY);
				//m_pCurrPixel->SetNewType(EMPTY);
				m_pCurrPixel->SetActorScale3D(FVector(scaleFactor));
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
			switch(m_pCurrPixel->GetType())
			{
			case EMPTY:
				break;
			case SAND:
				UpdateSandPixel(m_pCurrPixel, gridX, gridY);
				break;
			case WATER:
				break;
			default: ;
			}
		}
	}
}

