// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pixel.generated.h"

UCLASS()
class ECSGRADWORKSEPPEDB_API APixel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APixel();
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* pStaticMesh;

	

private:
	UMaterialInterface* m_pMeshMaterial;
	UMaterialInstanceDynamic* m_pDynamicMaterial;
	TArray<USceneComponent*> m_pMeshChildren;
	
	int m_amtOfNeighbours = 0;

	FVector m_Scale;

	bool m_IsAlive = false;
	bool m_NextUpdateAliveStatus = false;

	void SetAlive(bool aliveState);
	void SetNeighbourCount(int neighbours);
	void DoGameOfLifeCheck();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetColor(FVector newColor);
	
	bool GetCurrentAlive() const {return m_IsAlive;};
	bool GetNextAlive() const {return m_NextUpdateAliveStatus;};


	void ForceAlive();
	void DoGameOfLifeLoop(int neighbours);
	void UpdatePixelAliveStatus();
};
