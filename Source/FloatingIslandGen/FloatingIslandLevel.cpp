// Fill out your copyright notice in the Description page of Project Settings.

#include "FloatingIslandLevel.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"
#include <Kismet/GameplayStatics.h>
#include "BoxTypes.h"

#include "FloatingIslandSpawnVolume.h"
#include "FloatingIslandActor.h"
#include "PreDefinedActorDefinition.h"
#include "IslandParameterDefinition.h"
#include "PropDefinition.h"

//Generates islands based on blueprints or PCG, then spawns props
void AFloatingIslandLevel::GenerateIslands()
{
	//Set up random actor arrays
	for (UPropDefinition* Prop : RandomAssetsList)
	{
		if (!IsValid(Prop))
		{
			continue;
		}

		for (int i = 0; i < Prop->SpawnChance; i++)
		{
			AbsoluteRandomProps.Add(Prop);
		}
	}

	if (UsePreDefinedActors == true)
	{
		for (UPreDefinedActorDefinition* Actor : IslandBlueprints)
		{
			if (!IsValid(Actor))
			{
				continue;
			}

			for (int i = 0; i < Actor->SpawnChance; i++)
			{
				AbsolutePreDefinedActors.Add(Actor);
			}
		}
	}
	else
	{
		for (UIslandParameterDefinition* Parameters : IslandParameters)
		{
			if (!IsValid(Parameters))
			{
				continue;
			}

			for (int i = 0; i < Parameters->SpawnChance; i++)
			{
				AbsoluteIslandParameters.Add(Parameters);
			}
		}
	}

	//Delete all previously generated actors - mainly useful for 
	if (DoNotDeletePreviouslyGeneratedActors == false)
	{
		DeleteAllGeneratedActors();
	}

	//Exit early if no spawn volumes are present
	if (SpawnVolumes.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No spawn volumes present!"));
		return;
	}

	//Generate the islands
	for (int i = 0; i < MaxIslandCount; i++)
	{
		//Pick a random spawn volume
		AFloatingIslandSpawnVolume* ChosenVolume = PickRandomVolume();

		//Find a point inside of it, and spawn a new island, then add it to the array
		if (ChosenVolume != nullptr)
		{
			FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(ChosenVolume->GetActorLocation(), ChosenVolume->Volume->GetScaledBoxExtent() / 2);
			FIslandActorDefinition ActorDefinition;
			
			if (UsePreDefinedActors == true)
			{
				ActorDefinition = SpawnIslandFromBlueprint(RandomPoint);
			}
			else
			{
				ActorDefinition = SpawnNewProceduralIsland(RandomPoint);
			}

			GeneratedIslands.Add(ActorDefinition);
		}
	}

	//Place assets if required
	if (SpawnRandomAssets == true)
	{
		//Do not continue if there are no random assets to spawn
		if (RandomAssetsList.IsEmpty())
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No props in the array to spawn!"));
			return;
		}

		//For each generated island...
		for (FIslandActorDefinition& ActorDefinition : GeneratedIslands)
		{
			AActor* Actor = ActorDefinition.Actor;

			if (Actor == nullptr)
			{
				continue;
			}

			//Grab the values from the actor definition
			FVector Center = ActorDefinition.BoundsCenter;
			FVector Extents = ActorDefinition.BoundsExtent;
			int NumberOfAssets = ActorDefinition.PropsToSpawn;

			//If debug rendering is enabled, draw the bounds
			if (EnableDebugRendering == true)
			{
				DrawDebugBox(GetWorld(), Actor->GetActorLocation() + Center, Extents, FColor::Purple, true);
			}

			//For the total number of assets to place on this island...
			for (int i = 0; i < NumberOfAssets; i++)
			{
				//Get a random point in the bounding box found earlier
				FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(Center, Extents);

				//Set up a hit result, using the x and y from the random point
				FHitResult Hit;
				TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
				TArray<AActor*> IgnoreActors;
				FVector TracePositionEnd = Actor->GetActorLocation() + FVector(RandomPoint.X, RandomPoint.Y, -150);
				FVector TracePositionStart = Actor->GetActorLocation() + FVector(RandomPoint.X, RandomPoint.Y, 150);

				//Enable/disable debug draw trace
				EDrawDebugTrace::Type DrawTrace = EDrawDebugTrace::None;

				if (EnableDebugRendering == true)
				{
					DrawTrace = EDrawDebugTrace::Persistent;
				}

				//Add the island channel channel
				ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2));

				//Perform the trace
				UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TracePositionStart, TracePositionEnd, ObjectTypes, true, IgnoreActors, DrawTrace, Hit, false);

				if (Hit.bBlockingHit == true)
				{
					//Pick a random prop and get its actor class
					int RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, AbsoluteRandomProps.Num() - 1);
					UPropDefinition* ChosenProp = AbsoluteRandomProps[RandomIndex];
					TSubclassOf<AActor> PropClass = ChosenProp->PropToSpawn;

					//Set up spawn parameters for collision rules
					FActorSpawnParameters SpawnParameters;
					SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
					
					//Spawn the prop and add it to the spawned props array
					AActor* NewProp = GetWorld()->SpawnActor<AActor>(PropClass, Hit.ImpactPoint + ChosenProp->SpawnOffset, FRotator(), SpawnParameters);
					ActorDefinition.IslandProps.Add(NewProp);
				}
			}
		}
	}
}

void AFloatingIslandLevel::DeleteAllGeneratedActors()
{
	//Clear previously generated actors
	for (FIslandActorDefinition Island : GeneratedIslands)
	{
		Island.Actor->Destroy();

		for (AActor* Prop : Island.IslandProps)
		{
			Prop->Destroy();
		}
	}

	GeneratedIslands.Empty();
}

// Sets default values
AFloatingIslandLevel::AFloatingIslandLevel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFloatingIslandLevel::BeginPlay()
{
	Super::BeginPlay();

	if (OverrideIslandGenerationCall == false)
	{
		GenerateIslands();
	}
}

//Return a random spawn volume
AFloatingIslandSpawnVolume* AFloatingIslandLevel::PickRandomVolume()
{
	return SpawnVolumes[UKismetMathLibrary::RandomIntegerInRange(0, SpawnVolumes.Num() - 1)];
}

//Called if the use of blueprints is disabled
FIslandActorDefinition AFloatingIslandLevel::SpawnNewProceduralIsland(FVector RandomPoint)
{
	//If the island parameters list is empty return a nullptr
	if (IslandParameters.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Parameters list empty!"));
		return FIslandActorDefinition();
	}

	//Set up the transform for the island, then spawn a deferred actor
	FTransform Transform;
	Transform.SetTranslation(RandomPoint);
	Transform.SetRotation(FQuat());

	AFloatingIslandActor* Actor = GetWorld()->SpawnActorDeferred<AFloatingIslandActor>(IslandClass, Transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);

	//Give the new island a random parameter and regenerate its mesh
	if(Actor != nullptr)
	{
		int RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, AbsoluteIslandParameters.Num() - 1);
		Actor->IslandParameters = AbsoluteIslandParameters[RandomIndex];
		Actor->RegenerateIsland();
	}

	UGameplayStatics::FinishSpawningActor(Actor, Transform);

	//Set the parameters of the struct, add it to the array and return
	FIslandActorDefinition NewDefinition;

	NewDefinition.Actor = Actor;
	NewDefinition.PropsToSpawn = Actor->IslandParameters->MaxProps;

	UE::Geometry::FAxisAlignedBox3d Box = Actor->GetDynamicMeshComponent()->GetMesh()->GetBounds();
	NewDefinition.BoundsCenter = Box.Center();
	NewDefinition.BoundsExtent = Box.Extents();

	return NewDefinition;
}

//Called if the use of blueprints is enabled
FIslandActorDefinition AFloatingIslandLevel::SpawnIslandFromBlueprint(FVector RandomPoint)
{
	//If the island blueprints list is empty return a nullptr
	if (IslandBlueprints.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Blueprints list empty!"));
		return FIslandActorDefinition();
	}

	//Get a random predefined actor definition and get the class to spawn
	int RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, AbsolutePreDefinedActors.Num() - 1);
	UPreDefinedActorDefinition* IslandDefinition = AbsolutePreDefinedActors[RandomIndex];
	TSubclassOf<AActor> ActorToSpawn = IslandDefinition->BlueprintToSpawn;

	//Set up the transform, then spawn a deferred actor
	FTransform Transform;
	Transform.SetTranslation(RandomPoint);
	Transform.SetRotation(FQuat());

	AActor* Actor = GetWorld()->SpawnActorDeferred<AActor>(ActorToSpawn, Transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);
	
	//Cast the actor to the floating island class, if it is successful then it can be regenerated
	//(Not all predefined actors are static mesh!)
	AFloatingIslandActor* Island = Cast<AFloatingIslandActor>(Actor);
	
	//If the cast is successful generate the island
	if (Island != nullptr)
	{
		Island->RegenerateIsland();
	}

	UGameplayStatics::FinishSpawningActor(Actor, Transform);

	//Set the parameters of the struct, add it to the array and return
	FIslandActorDefinition NewDefinition;

	NewDefinition.Actor = Actor;
	NewDefinition.PropsToSpawn = IslandDefinition->PropsToSpawn;

	if (Island != nullptr)
	{
		//If the island is a floating island actor, get the bounds from the dynamic mesh component
		UE::Geometry::FAxisAlignedBox3d Box = Island->GetDynamicMeshComponent()->GetMesh()->GetBounds();
		NewDefinition.BoundsCenter = Box.Center();
		NewDefinition.BoundsExtent = Box.Extents();
	}
	else
	{
		//Otherwise, get it from a static mesh component
		UStaticMeshComponent* StaticMeshComponent = Actor->GetComponentByClass<UStaticMeshComponent>();

		if (!IsValid(StaticMeshComponent))
		{
			//If that isn't valid, set it to 0
			NewDefinition.BoundsCenter = FVector(0, 0, 0);
			NewDefinition.BoundsExtent = FVector(0, 0, 0);
		}
		else
		{
			NewDefinition.BoundsCenter = StaticMeshComponent->GetStaticMesh()->GetBounds().GetBox().GetCenter();
			NewDefinition.BoundsExtent = StaticMeshComponent->GetStaticMesh()->GetBounds().GetBox().GetExtent() * StaticMeshComponent->GetComponentScale();
		}
	}

	return NewDefinition;
}

// Called every frame
void AFloatingIslandLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

