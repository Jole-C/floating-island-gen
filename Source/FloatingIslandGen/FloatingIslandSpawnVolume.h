// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingIslandSpawnVolume.generated.h"

UCLASS()
class FLOATINGISLANDGEN_API AFloatingIslandSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBoxComponent* Volume;

	// Sets default values for this actor's properties
	AFloatingIslandSpawnVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
