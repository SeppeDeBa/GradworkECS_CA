// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOfLifeProcessor.h"

#include "ECSFragments.h"
#include "GM_CA.h"
#include "MassCommonTypes.h"
#include "NeighbourCheckProcessor.h"
#include "Kismet/GameplayStatics.h"

UGameOfLifeProcessor::UGameOfLifeProcessor()
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
	ProcessingPhase = EMassProcessingPhase::PostPhysics;
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Movement;
	ExecutionOrder.ExecuteAfter.Add(TEXT("NeighbourCheckProcessor"));
	ExecutionOrder.ExecuteBefore.Add(TEXT("AliveStateProcessor"));
}

void UGameOfLifeProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FLifeFragment>(EMassFragmentAccess::ReadWrite);

	EntityQuery.RegisterWithProcessor(*this);
}

void UGameOfLifeProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
		if(m_FirstFrameSkipped == true)
		{
			++itCounter;
			if(!startedCounting)
			{
				startedCounting = true;
				startTime = FPlatformTime::Seconds();
			}
			if(doMeasurementsFor1k && itCounter == 1000)
			{
				double oneThousandGenTime = FPlatformTime::Seconds();
				double deltaTime = (oneThousandGenTime - startTime);
				UE_LOG(LogTemp, Log, TEXT("Elapsed Time: %f seconds"), deltaTime);
				if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, TEXT("1k is done"));

			}
			if(doMeasurementsFor10k && itCounter == 10000)
			{
				if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, TEXT("10k is done"));

				double tenThousandGenTime = FPlatformTime::Seconds();
				double deltaTime = (tenThousandGenTime - startTime) / 10000;
				UE_LOG(LogTemp, Log, TEXT("average time over 10k frames is: %f"), deltaTime);
			}

			double GoLStartTime = FPlatformTime::Seconds();
			EntityQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context)
			{

					const TArrayView<FLifeFragment>& LifeList = Context.GetMutableFragmentView<FLifeFragment>();
			
			
					for (int32 EntityIndex = 0; EntityIndex < Context.GetNumEntities(); ++EntityIndex)
					{
						bool pixelIsAlive = LifeList[EntityIndex].IsAlive;
						int pixelNeighbourCount = LifeList[EntityIndex].amtOfNeighbours;
						if(pixelIsAlive == true)
						{
							if(pixelNeighbourCount <= 1)
							{
								LifeList[EntityIndex].IsNextUpdateAlive = false;
							}
							if(pixelNeighbourCount >= 4)
							{
								LifeList[EntityIndex].IsNextUpdateAlive = false;
							}
							//stay alive if 2 or 3
						}
						else
						{
							if(pixelNeighbourCount == 3)
							{
								LifeList[EntityIndex].IsNextUpdateAlive = true;
							}
						}
					}
			});
			double GoLEndTime = FPlatformTime::Seconds();
			double GoLDeltaTime = GoLEndTime - GoLStartTime;
			totalGameOfLifeGridCheckTimes += GoLDeltaTime;
			if(itCounter % 1000 == 0)
			{
				UE_LOG(LogTemp, Log, TEXT("1000 hit"));
			
				if(doMeasurementsForGoL && itCounter == 1000)
				{
					totalGameOfLifeGridCheckTimes /= 1000.f;
					UE_LOG(LogTemp, Log, TEXT("average time per game of life tick: %f"), totalGameOfLifeGridCheckTimes);
				}
			}
		}
		else
		{
			m_FirstFrameSkipped = true;
			
		}
}

void UGameOfLifeProcessor::Initialize(UObject& Owner)
{
	Super::Initialize(Owner);
	MyGamemode = Cast<AGM_CA>(UGameplayStatics::GetGameMode(GetWorld()));
	if (MyGamemode == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode not found."));
	}
}



