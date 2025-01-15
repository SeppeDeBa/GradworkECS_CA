// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ECSFragments.h"
#include "MassCommonFragments.h"
#include "MassEntityTemplateRegistry.h"
#include "MassEntityTraitBase.h"
#include "PixelVerTwoTraits.generated.h"

/**
 * 
 */
UCLASS()
class ECSGRADWORKSEPPEDB_API UPixelVerTwoTraits : public UMassEntityTraitBase
{
	GENERATED_BODY()
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override
	{
		FMassEntityManager& EntityManager = UE::Mass::Utils::GetEntityManagerChecked(World);
		BuildContext.AddFragment<FTransformFragment>();
		BuildContext.AddFragment<FGridLocationFragment>();
		BuildContext.AddFragment<FLifeFragment>();
		FVTheLifeGridFragment mySharedLifeGrid;
		FSharedStruct MySharedFragment = EntityManager.GetOrCreateSharedFragment<FVTheLifeGridFragment>(mySharedLifeGrid);
		if(MySharedFragment.IsValid())
		{
			BuildContext.AddSharedFragment(MySharedFragment);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to register shared fragment!"));
		}
	}
	UPROPERTY(EditAnywhere)
	FColor InactiveColor;
	UPROPERTY(EditAnywhere)
	FColor ActiveColor;

	// Editable in the editor property list for this asset
	UPROPERTY(EditAnywhere)
	int gridLength = 50;
	UPROPERTY(EditAnywhere)
	float gridSize = 1000;
private:
	float cellSize = gridSize / gridLength;
	static int wow ;
};
