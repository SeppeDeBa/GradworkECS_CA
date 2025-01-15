// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTraitBase.h"
#include "ECSPixelTrait.generated.h"

/**
 * 
 */


UCLASS(meta = (DisplayName = "Pixel Actor Trait"))
class ECSGRADWORKSEPPEDB_API UECSPixelTrait : public UMassEntityTraitBase
{
	GENERATED_BODY()
public:

	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;


	UPROPERTY(EditAnywhere, Category = "Config")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, Category = "Config")
	UStaticMesh* staticMeshActualMesh;//todo:better naming

	
	UPROPERTY(EditAnywhere, Category = "Config")
	TSoftObjectPtr<UMaterialInterface> MaterialOverride;
	// Editable in the editor property list for this asset
	UPROPERTY(EditAnywhere)
	int gridLength = 40;
	UPROPERTY(EditAnywhere)
	float gridSize = 1000;
private:
	float cellSize = gridSize / gridLength;
	static int wow ;
	
};
