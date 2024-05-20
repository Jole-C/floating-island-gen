#include "DynamicMeshActor.h"
#include "IslandParameterDefinition.h"
#include "FloatingIslandActor.generated.h"

UCLASS()
class FLOATINGISLANDGEN_API AFloatingIslandActor : public ADynamicMeshActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UIslandParameterDefinition* IslandParameters;

	// Sets default values for this actor's properties
	AFloatingIslandActor();

	UFUNCTION(BlueprintCallable, CallInEditor, BlueprintNativeEvent, Category = "Island Generation")
	void RegenerateIsland();
	void RegenerateIsland_Implementation();

protected:
	

};