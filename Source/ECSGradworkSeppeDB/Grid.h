// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pixel.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

UCLASS()
class ECSGRADWORKSEPPEDB_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();
	UFUNCTION(BlueprintCallable)
	void GeneratePixels();
	void GenerateNext();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GridVariables")
	float GridSize = 1000.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GridVariables")
	int Cols = 16; //cols are equal to rows

	const float m_UpdateTimerMax = 5.f;
	float m_CurrTimer = 0.f;
private:


	TSubclassOf<class APixel> m_PixelClass;
	APixel* m_pCurrPixel;
	TArray<APixel*> m_GridPtrArray;
	
	APixel* GetPixelFromGrid(int x, int y) const;
	bool m_IsGridCreated = false;

	int GetAmtPixelNeighbours(int x, int y) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
