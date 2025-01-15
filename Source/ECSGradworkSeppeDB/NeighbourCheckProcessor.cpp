// Fill out your copyright notice in the Description page of Project Settings.


#include "NeighbourCheckProcessor.h"
#include "GameOfLifeProcessor.h"
#include "ECSFragments.h"
#include "GM_CA.h"
#include "MassCommonTypes.h"
#include "Kismet/GameplayStatics.h"

UNeighbourCheckProcessor::UNeighbourCheckProcessor()
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
	ProcessingPhase = EMassProcessingPhase::PostPhysics;
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Movement;
	ExecutionOrder.ExecuteBefore.Add(TEXT("GameOfLifeProcessor"));
	ExecutionOrder.ExecuteAfter.Add(TEXT("GridPlacementProcessor"));

}

void UNeighbourCheckProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FGridLocationFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FLifeFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddSharedRequirement<FVTheLifeGridFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.RegisterWithProcessor(*this);
}

void UNeighbourCheckProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
		if(m_FirstFrameSkipped == true)
		{
			EntityQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context)
			{
					FVTheLifeGridFragment& GottenLifeGrid = Context.GetMutableSharedFragment<FVTheLifeGridFragment>();
					const TArrayView<FLifeFragment>& LifeList = Context.GetMutableFragmentView<FLifeFragment>();
					const TArrayView<FGridLocationFragment>& GridLocationsList = Context.GetMutableFragmentView<FGridLocationFragment>();
			
					int gridLength = MyGamemode->gridLength;
					//const float WorldDeltaTime = Context.GetDeltaTimeSeconds();
					for (int32 EntityIndex = 0; EntityIndex < Context.GetNumEntities(); ++EntityIndex)
					{
						int col = GridLocationsList[EntityIndex].X;
						int row = GridLocationsList[EntityIndex].Y;
						int gridIt = GridLocationsList[EntityIndex].GridIterator;
						int neighbours = 0;
			
						for(int currentXChecking = col-1; currentXChecking <= col+1; ++currentXChecking)
						{
							//check if X is in bounds
							if(currentXChecking < 0 || currentXChecking >= gridLength) continue;
			
							//go through collumns
							for(int currentYChecking = row-1; currentYChecking <= row+1; ++currentYChecking)
							{
								//check if Y is in bounds
								if(currentYChecking < 0 || currentYChecking >= gridLength) continue;
			
								//check if it's not the checked cell, if it is then early out
								if(currentXChecking == col && currentYChecking == row) continue;
				
								//update neighbor if its alive
								if(GottenLifeGrid.LifeGrid.Contains(currentXChecking + currentYChecking * gridLength))
								{
									if(GottenLifeGrid.LifeGrid[currentXChecking + currentYChecking * gridLength] == true) ++neighbours;
								}
							}
						}
						LifeList[EntityIndex].amtOfNeighbours = neighbours; //watch out this could be a different entity
					}
			});
		}
		else
		{
			m_FirstFrameSkipped = true;
		}
}

void UNeighbourCheckProcessor::Initialize(UObject& Owner)
{
	Super::Initialize(Owner);
	MyGamemode = Cast<AGM_CA>(UGameplayStatics::GetGameMode(GetWorld()));
	if (MyGamemode == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode not found."));
	}
}
	
	

