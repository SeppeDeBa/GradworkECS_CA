// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityManager.h"
#include "GameFramework/GameMode.h"
#include "GM_CA.generated.h"

/**
 * 
 */
UCLASS()
class ECSGRADWORKSEPPEDB_API AGM_CA : public AGameMode
{
	GENERATED_BODY()

public:

	bool CanSimulate() const {return InitFinished;};

	UFUNCTION(BlueprintCallable)
	void RecalcValues()
	{
		cellSize = gridSize/(static_cast<float>(gridLength));
		scaleFactor = cellSize/100.f;
	}
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GridParams")
	int gridLength = 50;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GridParams")
	float gridSize = 1000.f;
	
	UPROPERTY(BlueprintReadOnly)
	float cellSize = gridSize/(static_cast<float>(gridLength));
	UPROPERTY(BlueprintReadOnly)
	float scaleFactor = cellSize/100.f;

	
	
	UFUNCTION(BlueprintCallable)
	void ConfirmInit(){InitFinished = true;};
private:
	
	bool InitFinished = false;

};
