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
struct ECSGRADWORKSEPPEDB_API FGridLocationFragment : public FMassFragment
{
	GENERATED_BODY()
	int X = 0;
	int Y = 0;
	int GridIterator = 0;
};


USTRUCT(BlueprintType)
struct ECSGRADWORKSEPPEDB_API FVisualsFragment : public FMassFragment
{
	GENERATED_BODY()
	//static mesh
	UPROPERTY()
	UStaticMeshComponent* StaticMesh;

	//materials
	UPROPERTY()
	UMaterialInterface* MeshMaterial;
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;
	UPROPERTY()
	FVector Scale;
};

// ==== SHARED FRAGMENTS ====
// USTRUCT()
// struct ECSGRADWORKSEPPEDB_API FVGridParametersFragment : public FMassSharedFragment
// {
// 	GENERATED_BODY()	
// 	int GridLength = 50;
// 	float GridSize = 1000;
// 	float cellSize = GridSize / GridLength;
// };

USTRUCT()
struct ECSGRADWORKSEPPEDB_API FVTheLifeGridFragment : public FMassSharedFragment
{
	GENERATED_BODY()	
	int GridLength = 50;
	float GridSize = 1000;
	float cellSize = GridSize / GridLength;

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
