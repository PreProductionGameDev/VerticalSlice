// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NetworkInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNetworkInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *	Interface used to Invert the Dependency for the Menu System
 *	Handles creating virtual functions for the Join and Host from the Networked Game Instance
 */
class VERTICALSLICE_API INetworkInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Host() = 0;

	virtual void Join(uint32 Index) = 0;

	virtual void RefreshServerList(class UServerFinder* InServerFinder) = 0;
};
