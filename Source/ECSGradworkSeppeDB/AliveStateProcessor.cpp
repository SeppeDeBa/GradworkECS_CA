// Fill out your copyright notice in the Description page of Project Settings.


#include "AliveStateProcessor.h"
#include "GameOfLifeProcessor.h"
#include "ECSFragments.h"

UAliveStateProcessor::UAliveStateProcessor()
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
	ProcessingPhase = EMassProcessingPhase::PostPhysics;
}

void UAliveStateProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FLifeFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddSharedRequirement<FVTheLifeGridFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FGridLocationFragment>(EMassFragmentAccess::ReadWrite);

	EntityQuery.RegisterWithProcessor(*this);
}

void UAliveStateProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	EntityQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context)
	{
		//FVTheLifeGridFragment GottenLifeGrid = Context.GetSharedFragment<FVTheLifeGridFragment>();
		FVTheLifeGridFragment GottenLifeGrid = Context.GetMutableSharedFragment<FVTheLifeGridFragment>();
		const TArrayView<FLifeFragment> LifeList = Context.GetMutableFragmentView<FLifeFragment>();
		const TArrayView<FGridLocationFragment> GridLocationsList = Context.GetMutableFragmentView<FGridLocationFragment>();

		
		int gridLength = GottenLifeGrid.GridLength;
		//const float WorldDeltaTime = Context.GetDeltaTimeSeconds();

		for (int32 EntityIndex = 0; EntityIndex < Context.GetNumEntities(); ++EntityIndex)
		{
			LifeList[EntityIndex].IsAlive = LifeList[EntityIndex].IsNextUpdateAlive;
			GottenLifeGrid.LifeGrid [GridLocationsList[EntityIndex].GridIterator] = LifeList[EntityIndex].IsAlive;
		}
	});
}
