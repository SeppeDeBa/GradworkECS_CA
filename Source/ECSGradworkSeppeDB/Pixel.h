// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pixel.generated.h"


UENUM(BlueprintType)
enum PixelType
{
	EMPTY UMETA(DisplayName = "EMPTY"),
	SAND UMETA(DisplayName = "SAND"),
	WATER UMETA(DisplayName = "WATER")
};

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
	PixelType m_CurrentPixelType = EMPTY;
	PixelType m_PreviousPixelType = EMPTY;
	FVector m_Color;
	FVector m_Scale;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetColor(FVector newColor);
	void SetPreviousType();
	void SetNewType(PixelType newType);
	PixelType GetPreviousType() const;
	PixelType GetType() const;
	void  ResetPixel(PixelType typeToSet = EMPTY);

};
