// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "MassProcessor.h"
#include "AliveStateProcessor.generated.h"

/**
 * 
 */
UCLASS()
class ECSGRADWORKSEPPEDB_API UAliveStateProcessor : public UMassProcessor
{
	GENERATED_BODY()
public:
	UAliveStateProcessor();
protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;
	FMassEntityQuery EntityQuery;
};
