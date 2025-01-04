// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

// Sets default values
AGrid::AGrid()
{
	m_pCurrPixel = nullptr;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//static ConstructorHelpers::FObjectFinder<UClass> pixelBP (TEXT("Blueprint'/Game/Blueprints/BP_Pixel.BP_Pixel_C'")); 
	//if (pixelBP.Object)
	{
		//m_PixelClass = pixelBP.Object;
	}
}


APixel* AGrid::GetPixelFromGrid(int x, int y) const
{
	ensure(x >= 0 && x < m_Cols);
	ensure(y >= 0 && y < m_Cols);
	return m_GridPtrArray[x + y * m_Cols];
}

bool AGrid::UpdateSandPixel(APixel* processedPixel, int xGridPos, int yGridPos)
{
	if(yGridPos >= m_Cols) return false; //go out if its at bottom as sand can only go down

	//do sand logics
	//check under
	APixel* checkingPixel = GetPixelFromGrid(xGridPos, yGridPos+1);
	if(checkingPixel->GetType() == EMPTY)
	{
		checkingPixel->SetNewType(SAND);
		processedPixel->SetNewType(EMPTY);
		return true;
	}
	//check under left
	if(xGridPos-1 >= 0)//check if valid in grid
	{
		checkingPixel = GetPixelFromGrid(xGridPos-1, yGridPos+1);
		if(checkingPixel->GetType() == EMPTY)
		{
			checkingPixel->SetNewType(SAND);
			processedPixel->SetNewType(EMPTY);
			return true;
		}
	}
	//check under right
	if(xGridPos+1 < m_Cols)//check if valid in grid
	{
		checkingPixel = GetPixelFromGrid(xGridPos+1, yGridPos+1);
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
	if(m_IsGridCreated) GenerateNext();
}
void AGrid::GeneratePixels()
{
	m_IsGridCreated = false;
	m_GridPtrArray.Empty();
	FActorSpawnParameters spawnInfo;
	FVector spawnLocation = FVector(0.0f, 0.0f, 0.0f);
	FRotator spawnRotation = FRotator(0.0f, 0.0f, 0.0f);

	for (int32 x = 0; x < m_Cols; x++) {       
		for (int32 y = 0; y < m_Cols; y++) { 		
			spawnLocation = FVector( x*m_GridSize/m_Cols,y*m_GridSize/m_Cols,10.0f);			
			m_GridPtrArray.Add(GetWorld()->SpawnActor<APixel>(m_PixelClass, spawnLocation, spawnRotation, spawnInfo)); 
			m_pCurrPixel = Cast<APixel>(m_GridPtrArray[y*m_Cols + y]);
			m_pCurrPixel->SetNewType(EMPTY);
		}
	}
	m_IsGridCreated = true;
}

void AGrid::GenerateNext()
{
	for (int32 gridX = 0; gridX < m_Cols; gridX++)
	{
		for (int32 gridY = 0; gridY < m_Cols; gridY++)
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
