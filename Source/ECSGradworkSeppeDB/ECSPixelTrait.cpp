// Fill out your copyright notice in the Description page of Project Settings.


#include "ECSPixelTrait.h"

#include "MassEntityTemplateRegistry.h"
#include "MassCommonFragments.h"
#include "ECSFragments.h"
#include "MassEntitySubsystem.h"

void UECSPixelTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	FMassEntityManager& EntityManager = UE::Mass::Utils::GetEntityManagerChecked(World);



	//2. create gridLocation
	FGridLocationFragment& gridLockFrag = BuildContext.AddFragment_GetRef<FGridLocationFragment>();
	
	
	//3. Adding lifeFrag
	BuildContext.AddFragment<FLifeFragment>();

	//4. Adding TransformFrag
	BuildContext.AddFragment<FTransformFragment>();


	FVTheLifeGridFragment mySharedLifeGrid;
	// uint32 MySharedFragmentHash =
	// 	UE::StructUtils::GetStructCrc32(FConstStructView::Make(mySharedLifeGrid));
	FSharedStruct MySharedFragment =
		EntityManager.GetOrCreateSharedFragment<FVTheLifeGridFragment>(mySharedLifeGrid);

	//1. add shared
	BuildContext.AddSharedFragment(MySharedFragment);
}
