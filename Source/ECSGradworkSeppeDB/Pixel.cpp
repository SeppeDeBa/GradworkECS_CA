// Fill out your copyright notice in the Description page of Project Settings.


#include "Pixel.h"

// Sets default values
APixel::APixel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	pStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cell Mesh"));

	//enable collision
	pStaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	pStaticMesh->SetCollisionObjectType(ECC_WorldDynamic);
	pStaticMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	pStaticMesh->SetGenerateOverlapEvents(true);
	pStaticMesh->SetNotifyRigidBodyCollision(true);
	pStaticMesh->bReturnMaterialOnMove = true;

	//enable input
	pStaticMesh->SetEnableGravity(false);
	pStaticMesh->bSelectable = true;
	pStaticMesh->bUseDefaultCollision = true;
	
	RootComponent = pStaticMesh;
	
}

void APixel::BeginPlay()
{
	Super::BeginPlay();
	m_pMeshMaterial = pStaticMesh->GetMaterial(0);
	m_pDynamicMaterial = UMaterialInstanceDynamic::Create(m_pMeshMaterial, NULL);
	SetColor(FVector(0.f,0.f,0.f));
	pStaticMesh->SetMaterial(0, m_pDynamicMaterial);

	//add onclick events
	pStaticMesh->OnClicked.AddDynamic(this, &APixel::OnMeshClicked);
}

void APixel::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(pStaticMesh)
	{
		pStaticMesh->OnClicked.RemoveDynamic(this, &APixel::APixel::OnMeshClicked);
	}
	Super::EndPlay(EndPlayReason);
}


// Called every frame
void APixel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APixel::SetColor(FVector newColor)
{
	m_pDynamicMaterial->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(FVector(newColor)));
}

void APixel::ForceAlive()
{
	if(m_IsAlive != true)
	{
		m_IsAlive = true;
		m_NextUpdateAliveStatus = true;
		SetAlive(true);
	}
}

void APixel::ForceDead()
{
	if(m_IsAlive)
	{
		m_IsAlive = false;
		m_NextUpdateAliveStatus = false;
		SetAlive(false);
	}
}

void APixel::DoGameOfLifeLoop(int neighbours)
{
	SetNeighbourCount(neighbours);
	DoGameOfLifeCheck();

}

void APixel::SetAlive(bool aliveState)
{
	m_NextUpdateAliveStatus = aliveState;
	SetColor(FVector(aliveState, aliveState, 0.f));//will be yellow if alive, black if not
}

void APixel::SetNeighbourCount(int neighbours)
{
	m_amtOfNeighbours = neighbours;
}

void APixel::DoGameOfLifeCheck()
{
	if(m_IsAlive == true)
	{
		if(m_amtOfNeighbours <= 1)
		{
			SetAlive(false);
		}
		if(m_amtOfNeighbours >= 4)
		{
			SetAlive(false);
		}
		//stay alive if 2 or 3
	}
	else
	{
		if(m_amtOfNeighbours == 3)
		{
			SetAlive(true);
		}
	}
}

void APixel::FlipState()
{
	SetAlive(!m_IsAlive);
	m_IsAlive = m_NextUpdateAliveStatus;
}

void APixel::UpdatePixelAliveStatus()
{
	if(m_NextUpdateAliveStatus != m_IsAlive)
	{
		m_IsAlive = m_NextUpdateAliveStatus;
		SetColor(FVector(m_IsAlive, m_IsAlive, 0.f));//will be yellow if alive, black if not
	}
}

void APixel::OnMeshClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	FlipState();
}
