// Fill out your copyright notice in the Description page of Project Settings.

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
};
