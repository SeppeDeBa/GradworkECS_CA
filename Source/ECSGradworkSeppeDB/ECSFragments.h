// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "ECSFragments.generated.h"


// ==== BASIC FRAGMENTS ====
USTRUCT(BlueprintType)
struct ECSGRADWORKSEPPEDB_API FLifeFragment : public FMassFragment
{
	GENERATED_BODY()
	bool IsAlive = false;
	bool IsNextUpdateAlive = false;
	int amtOfNeighbours = 0;
};

USTRUCT(BlueprintType)
struct ECSGRADWORKSEPPEDB_API FAliveColorFragment : public FMassFragment
{
	GENERATED_BODY()

	FAliveColorFragment() = default;
	
	FAliveColorFragment(FColor Color) : AliveColor(Color){};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FColor AliveColor = FColor::Yellow;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FColor DeadColor = FColor::Black; 
};
USTRUCT(BlueprintType)
struct ECSGRADWORKSEPPEDB_API FGridLocationFragment : public FMassFragment
{
	GENERATED_BODY()
	int X = 0;
	int Y = 0;
	int GridIterator = 0;
};


USTRUCT()
struct ECSGRADWORKSEPPEDB_API FVTheLifeGridFragment : public FMassSharedFragment
{
	GENERATED_BODY()	
	TMap<int, bool> LifeGrid;
};


// ==== TAGS ====
USTRUCT(BlueprintType)
struct ECSGRADWORKSEPPEDB_API FLonelinessTag : public FMassTag
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct ECSGRADWORKSEPPEDB_API FOverpopulationTag : public FMassTag
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct ECSGRADWORKSEPPEDB_API FNeighbourSurvival : public FMassTag
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct ECSGRADWORKSEPPEDB_API FNeighbourBirth : public FMassTag
{
	GENERATED_BODY()
};
