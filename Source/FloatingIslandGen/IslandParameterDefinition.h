// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDefinition.h"
#include "IslandParameterDefinition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class FLOATINGISLANDGEN_API UIslandParameterDefinition : public UBaseDefinition
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NumberOfCones;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseConeHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseConeWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinimumConeHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinimumConeWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RandomHeightDeviation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RandomWidthDeviation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ConeHeightDividend;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ConeWidthDividend;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IslandTipWidth;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WarpMagnitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WarpFrequency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SmoothingIterations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RemeshTriangleCount;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector IslandScale;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxProps;
};
