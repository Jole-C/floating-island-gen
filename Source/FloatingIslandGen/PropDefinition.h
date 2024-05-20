// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDefinition.h"
#include "PropDefinition.generated.h"

/**
 * 
 */
UCLASS()
class FLOATINGISLANDGEN_API UPropDefinition : public UBaseDefinition
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> PropToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector SpawnOffset = FVector(0, 0, -5);
};