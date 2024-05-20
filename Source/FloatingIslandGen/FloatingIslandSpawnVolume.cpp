// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingIslandSpawnVolume.h"
#include "Components/BoxComponent.h"

// Sets default values
AFloatingIslandSpawnVolume::AFloatingIslandSpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Volume = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	SetRootComponent(Volume);
}

// Called when the game starts or when spawned
void AFloatingIslandSpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFloatingIslandSpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

