// Fill out your copyright notice in the Description page of Project Settings.

#define LOG(Message, ...) UE_LOG(LogCategory, LogLevel, TEXT(Message), __VA_ARGS__)
#include "Grid.h"

#include "PhysicsAssetGenerationSettings.h"
#include "Kismet/GameplayStatics.h"

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


void AGrid::FlipIsGridRunning()
{
	m_IsGridRunning = !m_IsGridRunning;
}

void AGrid::FlipUpdateEveryFrame()
{
	UpdateEveryFrame = !UpdateEveryFrame;
}

void AGrid::DebugLogInfo()
{
	if(GEngine)
	{
		//pause state
		if(m_IsGridRunning) GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, TEXT("Active"));
		else GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, TEXT("Paused"));
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, TEXT("Space to toggle pause"));

		//speed state
		if(UpdateEveryFrame) GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, TEXT("Every Frame"));
		else GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, TEXT("Slow"));
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, TEXT("F to toggle speed"));

		//clear out
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Orange, TEXT("G to clear"));

		//reload
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Orange, TEXT("R to reload"));

		//generate single
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Orange, TEXT("N to single-frame(Only in pause)"));
		//generate single


		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Purple, TEXT("Population: ") + FString::Printf(TEXT("%d"), m_CurrAlivePixels));
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Purple, TEXT("Generation: ") + FString::Printf(TEXT("%d"), m_CurrGeneration));
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

	//enable mouse
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if(playerController)
	{
		player1Controller = playerController;
		playerController->bShowMouseCursor = true;
		playerController->bEnableClickEvents = true;
		playerController->bEnableMouseOverEvents = true;
	}
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//1. check mouse clicks
	if(player1Controller && player1Controller->WasInputKeyJustPressed(EKeys::LeftMouseButton))
	{
		FHitResult hitResult;
		player1Controller->GetHitResultUnderCursor(ECC_Visibility, false, hitResult);

		if(hitResult.bBlockingHit && hitResult.GetActor() == this)
		{
			OnMouseClicked((hitResult.Location));
		}
	}

	//2. check spacebar input for pausing
	if (player1Controller && player1Controller->WasInputKeyJustPressed(EKeys::SpaceBar))
	{
		if(m_CurrGeneration == 0) startTime = FPlatformTime::Seconds();
		FlipIsGridRunning();
		UE_LOG(LogTemp, Log, TEXT("Toggled Grid Running: %s"), m_IsGridRunning ? TEXT("ON") : TEXT("OFF"));
	}

	//3. check F for speedup button
	if (player1Controller && player1Controller->WasInputKeyJustPressed(EKeys::F))
	{
		FlipUpdateEveryFrame();
		UE_LOG(LogTemp, Log, TEXT("Toggled Update Every Frame: %s"), UpdateEveryFrame ? TEXT("ON") : TEXT("OFF"));
	}
	
	//4. check next iteration if applicable
	if(m_IsGridRunning)
	{
		if(UpdateEveryFrame)
		{
			GenerateNext();
		}
		else
		{
			if(m_IsGridCreated)
			{
				m_CurrTimer += DeltaTime;
				if(m_CurrTimer >= UpdateTimerMax)
				{
					m_CurrTimer = 0.f;
					GenerateNext();
				}
			}
		}
	}
	//5. generate new iteration if paused and N pressed
	else if (player1Controller && player1Controller->WasInputKeyJustPressed(EKeys::N))
	{
		GenerateNext();
	}


	//6. debug to screen
	if(doDebugVisualisation) DebugLogInfo();

	//7. reload level if R pressed
	if (player1Controller && player1Controller->WasInputKeyJustPressed(EKeys::R) && !m_IsGridRunning)
	{
		for(APixel* pixelPtr : m_GridPtrArray)
		{
			pixelPtr->ForceDead();
			m_CurrGeneration = 0;
		}
		// UWorld* World = GetWorld();
		// if (World)
		// {
		// 	FString CurrentLevelName = World->GetMapName();
		//
		// 	//remove the prefix from the map name
		// 	const FString Prefix = World->StreamingLevelsPrefix;
		// 	if (CurrentLevelName.StartsWith(Prefix))
		// 	{
		// 		CurrentLevelName.RightChopInline(Prefix.Len());
		// 	}
		//
		// 	//reload the current level
		// 	UGameplayStatics::OpenLevel(World, FName(*CurrentLevelName));
		//}
	}

	//8. on G press clear screen 
	if (player1Controller && player1Controller->WasInputKeyJustPressed(EKeys::G))
	{
		for(APixel* pixelPtr : m_GridPtrArray)
		{
			pixelPtr->ForceDead();
		}
	}
}

void AGrid::OnMouseClicked(const FVector& ClickLocation)
{
	float CellSize = GridSize / Cols;
	FVector LocalClick = ClickLocation - GetActorLocation();  // Convert to local grid space
	int32 Column = FMath::FloorToInt((LocalClick.X + GridSize / 2) / CellSize);
	int32 Row = FMath::FloorToInt((LocalClick.Y + GridSize / 2) / CellSize);

	// Ensure the indices are within bounds
	if (Column >= 0 && Column < Cols && Row >= 0 && Row < Cols)
	{
		APixel* ClickedPixel = GetPixelFromGrid(Column, Row);
		if (ClickedPixel)
		{
			ClickedPixel->FlipState();
			UE_LOG(LogTemp, Log, TEXT("Clicked on cell: Row = %d, Column = %d"), Row, Column);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Click was out of bounds!"));
	}
}

void AGrid::GeneratePixels()
{
	m_CurrGeneration = 0;
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

				if((y%4 == 1) && (x%4 != 0)) m_pCurrPixel->ForceAlive();
				else m_pCurrPixel->ForceDead();

				//if((y%3 != 0) && (x>0 && x < Cols-1)) m_pCurrPixel->ForceAlive();
				m_GridPtrArray.Add(newPixel);
			}
		}
	}
	m_IsGridCreated = true;
}

void AGrid::GenerateNext()
{
	++m_CurrGeneration;
	if (measure1000Generations && m_CurrGeneration == 1000)
	{
		thousandGenerationTime = FPlatformTime::Seconds();
		double deltaTime = thousandGenerationTime - startTime;
		UE_LOG(LogTemp, Log, TEXT("Elapsed Time: %f seconds"), deltaTime);
	}
	else if (measure10kGeneration && m_CurrGeneration == 10000)
	{
		double tenThousandGenTime = FPlatformTime::Seconds();
		double deltaTime = (tenThousandGenTime - startTime) / 10000;
		UE_LOG(LogTemp, Log, TEXT("average time over 10k frames is: %f"), deltaTime);
	}
	// else if ((m_CurrGeneration % 1000) == 0)
	// {
	// 	startTime = thousandGenerationTime;
	// 	thousandGenerationTime = FPlatformTime::Seconds();
	// 	double deltaTime = thousandGenerationTime - startTime;
	// 	UE_LOG(LogTemp, Log, TEXT("Elapsed Time: %f seconds"), deltaTime); 
	// }
	m_CurrAlivePixels = 0;

	
	double GoLStartTime = FPlatformTime::Seconds();
	for (int32 gridY = 0; gridY < Cols; gridY++)
	{
		for (int32 gridX = 0; gridX < Cols; gridX++)
		{
			m_pCurrPixel = GetPixelFromGrid(gridX, gridY);
			m_pCurrPixel->DoGameOfLifeLoop(GetAmtPixelNeighbours(gridX, gridY));
		}
	}
	double GoLEndTime = FPlatformTime::Seconds();
	double GOLDeltaTime = GoLEndTime - GoLStartTime;
	totalGameOfLifeGridCheckTimes +=GOLDeltaTime ;
	if(measureGameOfLifeTicks && m_CurrGeneration == 1000)
	{
		totalGameOfLifeGridCheckTimes /= 1000.f;
		UE_LOG(LogTemp, Log, TEXT("average time per game of life tick: %f"), totalGameOfLifeGridCheckTimes);

	}
	for (int32 gridY = 0; gridY < Cols; gridY++)
	{
		for (int32 gridX = 0; gridX < Cols; gridX++)
		{
			m_pCurrPixel = GetPixelFromGrid(gridX, gridY);
			m_pCurrPixel->UpdatePixelAliveStatus();
			if(m_pCurrPixel->GetNextAlive()) ++m_CurrAlivePixels;
		}
	}
}

