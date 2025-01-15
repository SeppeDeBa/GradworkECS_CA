// Fill out your copyright notice in the Description page of Project Settings.


#include "AliveStateProcessor.h"
#include "GameOfLifeProcessor.h"
#include "ECSFragments.h"
#include "MassCommonFragments.h"
#include "MassCommonTypes.h"


#include "MassStationaryVisualizationTrait.h"

UAliveStateProcessor::UAliveStateProcessor()
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
	ProcessingPhase = EMassProcessingPhase::PostPhysics;
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Movement;
	ExecutionOrder.ExecuteAfter.Add(TEXT("GameOfLifeProcessor"));

}

void UAliveStateProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FLifeFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FGridLocationFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddSharedRequirement<FVTheLifeGridFragment>(EMassFragmentAccess::ReadWrite);

	EntityQuery.RegisterWithProcessor(*this);
}

void UAliveStateProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	EntityQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context)
	{
		const TArrayView<FLifeFragment>& LifeList = Context.GetMutableFragmentView<FLifeFragment>();
		const TArrayView<FTransformFragment>& TransformsList = Context.GetMutableFragmentView<FTransformFragment>();
		FVTheLifeGridFragment& GottenLifeGrid = Context.GetMutableSharedFragment<FVTheLifeGridFragment>();
		const TArrayView<FGridLocationFragment>& GridLocList = Context.GetMutableFragmentView<FGridLocationFragment>();
		
		
		for (int32 EntityIndex = 0; EntityIndex < Context.GetNumEntities(); ++EntityIndex)
		{
			FTransform& currTransform = TransformsList[EntityIndex].GetMutableTransform();
			FGridLocationFragment& currGridLocFrag = GridLocList[EntityIndex];
			FLifeFragment& currFragment = LifeList[EntityIndex];
			if(currFragment.IsAlive != currFragment.IsNextUpdateAlive)
			{
				GottenLifeGrid.LifeGrid[currGridLocFrag.GridIterator] = currFragment.IsNextUpdateAlive;
				currFragment.IsAlive = currFragment.IsNextUpdateAlive;
				UE::Math::TVector<double> newTransform = currTransform.GetLocation();
				if(currFragment.IsAlive) newTransform.Z = 10.f;
				else newTransform.Z = -60.f;
				currTransform.SetLocation(newTransform); 
			}
			LifeList[EntityIndex].IsAlive = LifeList[EntityIndex].IsNextUpdateAlive;
			GottenLifeGrid.LifeGrid [GridLocList[EntityIndex].GridIterator] = LifeList[EntityIndex].IsAlive;
		}
	});
}
