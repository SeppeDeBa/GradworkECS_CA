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
	SetColor(FVector(1,0,0));
	pStaticMesh->SetMaterial(0, m_pDynamicMaterial);
}

// Called every frame
void APixel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APixel::SetColor(FVector newColor)
{
	m_pDynamicMaterial->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(FVector(newColor)));
	m_Color = newColor;
	//pStaticMesh->SetMaterial(0, m_pDynamicMaterial);
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
		switch(m_CurrentPixelType)
		{
		case EMPTY:
			SetColor(FVector(1.f, 0.f, 0.f));
			break;
		case SAND:
			SetColor(FVector(1.f, 0.55f, 0.2f));
			break;
		case WATER:
			SetColor(FVector(0, 0.5f, 1.f));

			break;
		default: ;
		}
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

