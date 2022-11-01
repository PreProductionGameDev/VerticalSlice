// 2022 ChronoOwl Studios


#include "Core/Abilities/GameplayEffects/GameplayEffectContext.h"

bool FGEContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	return Super::NetSerialize(Ar, Map, bOutSuccess) && TargetData.NetSerialize(Ar, Map, bOutSuccess);
}
