// 2022 ChronoOwl Studios

#pragma once


#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "GameplayEffectContext.generated.h"


/**
 * 
 */
USTRUCT()
struct SHIITAKESHOWDOWN_API FGEContext : public FGameplayEffectContext
{
	GENERATED_USTRUCT_BODY()
public:
	virtual FGameplayAbilityTargetDataHandle GetTargetData()
	{
		return TargetData;
	}

	virtual void AddTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
	{
		TargetData.Append(TargetDataHandle);
	}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGEContext::StaticStruct();
	}

	virtual FGEContext* Duplicate() const override
	{
		FGEContext* NewContext = new FGEContext();
		*NewContext = *this;
		NewContext->AddActors(Actors);
		if (GetHitResult())
		{
			// Deep copy of the hit results
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		// Shallow Copy of Target Data
		NewContext->TargetData.Append(TargetData);
		return NewContext;
	}

	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

protected:
	FGameplayAbilityTargetDataHandle TargetData;
};

template<>
struct TStructOpsTypeTraits<FGEContext> : public TStructOpsTypeTraitsBase2<FGEContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};
