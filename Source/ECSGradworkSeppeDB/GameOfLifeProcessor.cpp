// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOfLifeProcessor.h"

#include "ECSFragments.h"

UGameOfLifeProcessor::UGameOfLifeProcessor()
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
	ProcessingPhase = EMassProcessingPhase::DuringPhysics;
}

void UGameOfLifeProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FLifeFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FVisualsFragment>(EMassFragmentAccess::ReadWrite);
	

	EntityQuery.RegisterWithProcessor(*this);
}

void UGameOfLifeProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	EntityQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context)
	{
		const TArrayView<FLifeFragment> LifeList = Context.GetMutableFragmentView<FLifeFragment>();
		const TArrayView<FVisualsFragment> VisualsList = Context.GetMutableFragmentView<FVisualsFragment>();

		//const float WorldDeltaTime = Context.GetDeltaTimeSeconds();

		for (int32 EntityIndex = 0; EntityIndex < Context.GetNumEntities(); ++EntityIndex)
		{
			bool pixelIsAlive = LifeList[EntityIndex].IsAlive;
			int pixelNeighbourCount = LifeList[EntityIndex].amtOfNeighbours;
			if(pixelIsAlive == true)
			{
				if(pixelNeighbourCount <= 1)
				{
					LifeList[EntityIndex].IsNextUpdateAlive = false;
					VisualsList[EntityIndex].DynamicMaterial->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(FVector(0, 0, 0)));
				}
				if(pixelNeighbourCount >= 4)
				{
					LifeList[EntityIndex].IsNextUpdateAlive = false;
					VisualsList[EntityIndex].DynamicMaterial->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(FVector(0, 0, 0)));
				}
				//stay alive if 2 or 3
			}
			else
			{
				if(pixelNeighbourCount == 3)
				{
					LifeList[EntityIndex].IsNextUpdateAlive = true;
					VisualsList[EntityIndex].DynamicMaterial->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(FVector(1, 1, 0)));
				}
			}
		}
	});
}


