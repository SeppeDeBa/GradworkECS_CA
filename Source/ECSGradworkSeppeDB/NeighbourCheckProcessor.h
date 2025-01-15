// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "MassProcessor.h"
#include "NeighbourCheckProcessor.generated.h"

/**
 * 
 */
UCLASS()
class ECSGRADWORKSEPPEDB_API UNeighbourCheckProcessor : public UMassProcessor
{
	GENERATED_BODY()
public:
	UNeighbourCheckProcessor();
protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;
	FMassEntityQuery EntityQuery;
	virtual void Initialize(UObject& Owner) override;

private:
	bool m_FirstFrameSkipped = false;

	class AGM_CA* MyGamemode;

};
