﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntitySubsystem.h"
#include "MassExecutionContext.h"
#include "MassProcessor.h"
#include "GameOfLifeProcessor.generated.h"

/**
 * 
 */
UCLASS()
class ECSGRADWORKSEPPEDB_API UGameOfLifeProcessor : public UMassProcessor
{
	GENERATED_BODY()
public:
	UGameOfLifeProcessor();
protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;
	FMassEntityQuery EntityQuery;
	virtual void Initialize(UObject& Owner) override;

private:
	bool doMeasurementsForGoL = true;
	bool doMeasurementsFor10k = true;
	bool doMeasurementsFor1k = true;
	int itCounter;
	double startTime;
	double totalGameOfLifeGridCheckTimes;
	bool startedCounting = false;
	class AGM_CA* MyGamemode;
	bool m_FirstFrameSkipped = false;
};
