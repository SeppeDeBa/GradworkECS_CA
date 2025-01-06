// Fill out your copyright notice in the Description page of Project Settings.


#include "NeighbourCheckProcessor.h"
#include "GameOfLifeProcessor.h"
#include "ECSFragments.h"

UNeighbourCheckProcessor::UNeighbourCheckProcessor()
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
	ProcessingPhase = EMassProcessingPhase::PrePhysics;
}

void UNeighbourCheckProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FGridLocationFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FLifeFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddSharedRequirement<FVTheLifeGridFragment>(EMassFragmentAccess::ReadOnly);

	EntityQuery.RegisterWithProcessor(*this);
}

void UNeighbourCheckProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	EntityQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context)
	{
		const FVTheLifeGridFragment& GottenLifeGrid = Context.GetSharedFragment<FVTheLifeGridFragment>();
		const TArrayView<FLifeFragment> LifeList = Context.GetMutableFragmentView<FLifeFragment>();
		const TArrayView<FGridLocationFragment> GridLocationsList = Context.GetMutableFragmentView<FGridLocationFragment>();

		TMap<int,bool> lifeGrid = GottenLifeGrid.LifeGrid;
		int gridLength = GottenLifeGrid.GridLength;
		//const float WorldDeltaTime = Context.GetDeltaTimeSeconds();

		for (int32 EntityIndex = 0; EntityIndex < Context.GetNumEntities(); ++EntityIndex)
		{
			int col = GridLocationsList[EntityIndex].X;
			int row = GridLocationsList[EntityIndex].Y;

			int neighbours = 0;

			for(int currentXChecking = row-1; currentXChecking <= row+1; ++currentXChecking)
			{
				//check if Y is in bounds
				if(currentXChecking < 0 || currentXChecking >= gridLength) continue;
		
				//go through collumns
				for(int currentYChecking = col-1; currentYChecking <= col+1; ++currentYChecking)
				{
					//check if Y is in bounds
					if(currentYChecking < 0 || currentYChecking >= gridLength) continue;

					//check if it's not the checked cell, if it is then early out
					if(currentXChecking == row && currentYChecking == row) continue;
			
					//update neighbor if its alive
					if(lifeGrid[currentXChecking + currentYChecking * gridLength] == true) ++neighbours; 
				}
			}
			LifeList[EntityIndex].amtOfNeighbours = neighbours; //watch out this could be a different entity
		}
	});
}
