// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "TestAttributeSet.generated.h"

//uses macros from Attributes.h
#define ATTRIBUTER_ACCESSORS(ClassName, PropertyName)\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API UTestAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UTestAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTER_ACCESSORS(UTestAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTER_ACCESSORS(UTestAttributeSet, MaxHealth);
		
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Damage, Category = "Attributes")
	FGameplayAttributeData Damage;
	ATTRIBUTER_ACCESSORS(UTestAttributeSet, Damage);

	void AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute,
		const FGameplayAttributeData& MaxAttribute,
		float NewMaxValue,
		const FGameplayAttribute& AffectedAttributeProperty)const;
	
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Damage(const FGameplayAttributeData& OldValue);
};
