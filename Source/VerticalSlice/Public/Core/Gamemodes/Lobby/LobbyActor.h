// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LobbyActor.generated.h"


UCLASS()
class VERTICALSLICE_API ALobbyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALobbyActor();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//sets view to self
	void ResetCamera();

	//this mesh can change needs to change depending on the character and color so a reference is held
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(ReplicatedUsing=OnRep_SetHue1,EditAnywhere, BlueprintReadWrite)
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

};
