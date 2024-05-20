// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BaseDefinition.generated.h"

/**
 * 
 */
UCLASS()
class FLOATINGISLANDGEN_API UBaseDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int SpawnChance = 50;
	
};
