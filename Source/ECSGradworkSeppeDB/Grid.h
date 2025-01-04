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
private:
	float m_GridSize = 2000.f;
	int m_Cols = 100; //cols are equal to rows
	TSubclassOf<class APixel> m_PixelClass;
	APixel* m_pCurrPixel;
	TArray<APixel*> m_GridPtrArray;

	APixel* GetPixelFromGrid(int x, int y) const;
	bool m_IsGridCreated = false;


	bool UpdateSandPixel(APixel* processedPixel, int xGridPos, int yGridPos);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
