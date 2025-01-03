// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

// Sets default values
AGrid::AGrid()
{
	m_pCurrPixel = nullptr;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UClass> pixelBP (TEXT("Blueprint'/Game/Blueprints/BP_Pixel.BP_Pixel_C'")); 
	if (pixelBP.Object)
	{
		m_PixelClass = pixelBP.Object;
	}
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

	for (int32 i = 0; i < m_Cols; i++) {       
		for (int32 j = 0; j < m_Cols; j++) { 		
			spawnLocation = FVector( i*m_GridSize/m_Cols,j*m_GridSize/m_Cols,10.0f);			
			m_GridPtrArray.Add(GetWorld()->SpawnActor<APixel>(m_PixelClass, spawnLocation, spawnRotation, spawnInfo)); 
			m_pCurrPixel = Cast<APixel>(m_GridPtrArray[i*m_Cols + j]);
			m_pCurrPixel->SetNewType(EMPTY);
		}
	}
	m_IsGridCreated = true;
}
