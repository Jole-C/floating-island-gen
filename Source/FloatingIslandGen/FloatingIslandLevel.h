// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "FloatingIslandLevel.generated.h"

USTRUCT()
struct FIslandActorDefinition
{
	GENERATED_BODY()

public:
	FVector BoundsCenter;
	FVector BoundsExtent;
	AActor* Actor;
	int PropsToSpawn;
	TArray<AActor*> IslandProps;
};

UCLASS()
class FLOATINGISLANDGEN_API AFloatingIslandLevel : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Island Spawning | Generic Parameters")
	int MaxIslandCount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Island Spawning | Generic Parameters")
	bool OverrideIslandGenerationCall = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Island Spawning | Generic Parameters")
	bool EnableDebugRendering = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Island Spawning | Generic Parameters")
	bool DoNotDeletePreviouslyGeneratedActors = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Island Spawning | Island Distribution")
	TArray<class AFloatingIslandSpawnVolume*> SpawnVolumes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Island Spawning | Island Generating")
	bool UsePreDefinedActors = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "UsePreDefinedActors"), Category = "Island Spawning | Island Generating")
	TArray<class UPreDefinedActorDefinition*> IslandBlueprints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!UsePreDefinedActors"), Category = "Island Spawning | Island Generating")
	TArray<class UIslandParameterDefinition*> IslandParameters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!UsePreDefinedActors"), Category = "Island Spawning | Island Generating")
	TSubclassOf<class AFloatingIslandActor> IslandClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Spawning")
	bool SpawnRandomAssets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Spawning", meta = (EditCondition = "SpawnRandomAssets"))
	TArray<class UPropDefinition*> RandomAssetsList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Spawning", meta = (EditCondition = "SpawnRandomAssets"))
	int DefaultPropsToSpawn = 5;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void GenerateIslands();

	UFUNCTION(CallInEditor)
	void DeleteAllGeneratedActors();

	// Sets default values for this actor's properties
	AFloatingIslandLevel();

protected:
	TArray<FIslandActorDefinition> GeneratedIslands;
	TArray<UPropDefinition*> AbsoluteRandomProps;
	TArray<UIslandParameterDefinition*> AbsoluteIslandParameters;
	TArray<UPreDefinedActorDefinition*> AbsolutePreDefinedActors;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AFloatingIslandSpawnVolume* PickRandomVolume();
	FIslandActorDefinition SpawnNewProceduralIsland(FVector RandomPoint);
	FIslandActorDefinition SpawnIslandFromBlueprint(FVector RandomPoint);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};