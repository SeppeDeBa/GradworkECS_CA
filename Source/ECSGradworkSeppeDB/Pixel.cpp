// Fill out your copyright notice in the Description page of Project Settings.


#include "Pixel.h"

// Sets default values
APixel::APixel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	pStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cell Mesh"));
	RootComponent = pStaticMesh;
	
}

// Called when the game starts or when spawned
void APixel::BeginPlay()
{
	Super::BeginPlay();
	m_pMeshMaterial = pStaticMesh->GetMaterial(0);
	m_pDynamicMaterial = UMaterialInstanceDynamic::Create(m_pMeshMaterial, NULL);
	
}

// Called every frame
void APixel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APixel::SetColor(FVector newColor)
{
	m_pDynamicMaterial->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(FVector(m_Color)));
	pStaticMesh->SetMaterial(0, m_pDynamicMaterial);
}

void APixel::SetPreviousType()
{
	m_PreviousPixelType = m_CurrentPixelType;
}

void APixel::SetNewType(PixelType newType)
{
	if(newType != m_CurrentPixelType)
	{
		SetPreviousType();
		m_CurrentPixelType = newType;
	}
}

PixelType APixel::GetPreviousType() const
{
	return m_PreviousPixelType;
}

PixelType APixel::GetType() const
{
	return m_CurrentPixelType;
}

void APixel::ResetPixel(PixelType typeToSet)
{
	SetNewType(typeToSet);
}

