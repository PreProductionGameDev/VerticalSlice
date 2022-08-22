// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "GameFramework/Actor.h"
#include "BElement.generated.h"

class AFP_Character;
class UPlayerGameplayAbility;

UCLASS()
class VERTICALSLICE_API ABElement : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABElement();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly, Replicated)
	AFP_Character* OwningPlayer;

public:	
	// Adding the GAS Abilities
	UFUNCTION()
	virtual void AddAbilities();
	
	// Removing the GAS Abilities
	UFUNCTION(BlueprintCallable)
	virtual void RemoveAbilities();

	// Ability HUD component
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* Texture;

	//Ability Data for adding and removing utility ability
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UPlayerGameplayAbility> UtilityAbility;
	FGameplayAbilitySpecHandle UtilityAbilityHandle;

	//Ability Data for adding and removing ability
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UPlayerGameplayAbility> MovementAbility;
	FGameplayAbilitySpecHandle MovementAbilityHandle;
	
	

};
