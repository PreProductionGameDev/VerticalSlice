// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LobbyActor.generated.h"

/**
 *	The Actor class used in the Lobby Scene
 *	Player has no ability to posses or move, but handles replication of Color Values
 */
UCLASS()
class VERTICALSLICE_API ALobbyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALobbyActor();
	// Setting Replication Variables
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Sets view to self
	void ResetCamera();

	// This mesh can change needs to change depending on the character and color so a reference is held
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(ReplicatedUsing = OnRep_SetHue1, EditAnywhere, BlueprintReadWrite)
	float DoRep_Hue1;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnRep_SetHue1();
	
	UPROPERTY(ReplicatedUsing=OnRep_SetHue2,EditAnywhere, BlueprintReadWrite)
	float DoRep_Hue2;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable )
	void OnRep_SetHue2();

	UPROPERTY(ReplicatedUsing=OnRep_SetHue3,EditAnywhere, BlueprintReadWrite)
	float DoRep_Hue3;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable )
	void OnRep_SetHue3();

	UPROPERTY(ReplicatedUsing=OnRep_SetCurrentMesh, EditAnywhere, BlueprintReadWrite)
	int DoRep_CurrentMesh;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnRep_SetCurrentMesh();
};
