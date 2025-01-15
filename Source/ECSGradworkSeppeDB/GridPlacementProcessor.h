// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntitySubsystem.h"
#include "MassExecutionContext.h"
#include "MassProcessor.h"
#include "GridPlacementProcessor.generated.h"

/**
 * 
 */
UCLASS()
class ECSGRADWORKSEPPEDB_API UGridPlacementProcessor : public UMassProcessor
{
	GENERATED_BODY()
public:
	UGridPlacementProcessor();
protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;
	FMassEntityQuery EntityQuery;

	virtual void Initialize(UObject& Owner) override;
private:
	bool ranOnce = false;

	int currentIt = 0;
	
	class AGM_CA* MyGamemode;

};