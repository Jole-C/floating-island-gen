// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDefinition.h"
#include "PreDefinedActorDefinition.generated.h"

/**
 * 
 */
UCLASS()
class FLOATINGISLANDGEN_API UPreDefinedActorDefinition : public UBaseDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> BlueprintToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int PropsToSpawn = 5;
};
