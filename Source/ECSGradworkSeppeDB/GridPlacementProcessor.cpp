// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPlacementProcessor.h"
// Fill out your copyright notice in the Description page of Project Settings.

#include "ECSFragments.h"
#include "GM_CA.h"
#include "MassCommonFragments.h"
#include "MassCommonTypes.h"
#include "NeighbourCheckProcessor.h"
#include "GM_CA.h"
#include "Kismet/GameplayStatics.h"
#include "MassDebugVisualizationTrait.h"
#include "MassRepresentationActorManagement.h"
#include "MassStationaryVisualizationTrait.h"
#include "MassActorSubsystem.h"
UGridPlacementProcessor::UGridPlacementProcessor()
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
	ProcessingPhase = EMassProcessingPhase::PostPhysics;
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Movement;
	ExecutionOrder.ExecuteBefore.Add(TEXT("NeighbourCheckProcessor"));
}

void UGridPlacementProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FGridLocationFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FLifeFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddSharedRequirement<FVTheLifeGridFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.RegisterWithProcessor(*this);
}

void UGridPlacementProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	if(!ranOnce)
	{
		ranOnce = true;
		EntityQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context)
		{
			const TArrayView<FTransformFragment>& TransformsList = Context.GetMutableFragmentView<FTransformFragment>();
			FVTheLifeGridFragment& GottenLifeGrid = Context.GetMutableSharedFragment<FVTheLifeGridFragment>();
			const TArrayView<FLifeFragment>& LifeList = Context.GetMutableFragmentView<FLifeFragment>();
			const TArrayView<FGridLocationFragment>& GridLocList = Context.GetMutableFragmentView<FGridLocationFragment>();
			UE_LOG(LogTemp, Error, TEXT("Context.GetNumEntities() is: %d"), Context.GetNumEntities());
			for (int32 EntityIndex = 0; EntityIndex < Context.GetNumEntities(); ++EntityIndex)
			{
				FTransform& currTransform = TransformsList[EntityIndex].GetMutableTransform();
				// FVisualsFragment& currVisFrag = VisualsList[EntityIndex];
				 FGridLocationFragment& currGridLocFrag = GridLocList[EntityIndex];

				currGridLocFrag.GridIterator = currentIt;
				currGridLocFrag.X = currentIt % MyGamemode->gridLength;
				currGridLocFrag.Y = (int)(currentIt / MyGamemode->gridLength);

				if((currGridLocFrag.Y % 4 == 1) && (currGridLocFrag.X % 4 != 0))
				{
					LifeList[EntityIndex].IsAlive = false;
					LifeList[EntityIndex].IsNextUpdateAlive = true;
					GottenLifeGrid.LifeGrid.Add(currentIt, true);
				}
				else
				{
					LifeList[EntityIndex].IsAlive = true;
					LifeList[EntityIndex].IsNextUpdateAlive = false;
					GottenLifeGrid.LifeGrid.Add(currentIt, false);
				}
				currTransform.SetLocation(FVector(-MyGamemode->gridSize/2.f +  currGridLocFrag.X * MyGamemode->cellSize, -MyGamemode->gridSize/2.f + currGridLocFrag.Y * MyGamemode->cellSize,10.0f));
				currTransform.SetScale3D(FVector(MyGamemode->scaleFactor));
				++currentIt;
				}
		});
	}
}

void UGridPlacementProcessor::Initialize(UObject& Owner)
{
	Super::Initialize(Owner);
	MyGamemode = Cast<AGM_CA>(UGameplayStatics::GetGameMode(GetWorld()));
	if (MyGamemode == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode not found."));
	}
}
