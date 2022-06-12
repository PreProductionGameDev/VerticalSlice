// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "SoundEQEditorStyle.h"

class FSoundEQEditorCommands : public TCommands<FSoundEQEditorCommands>
{
public:

	FSoundEQEditorCommands()
		: TCommands<FSoundEQEditorCommands>(TEXT("SoundEQEditor"), NSLOCTEXT("Contexts", "SoundEQEditor", "SoundEQEditor Plugin"), NAME_None, FSoundEQEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};