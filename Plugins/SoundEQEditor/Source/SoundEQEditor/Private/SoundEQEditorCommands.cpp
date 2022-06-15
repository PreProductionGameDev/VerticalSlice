// Copyright Epic Games, Inc. All Rights Reserved.

#include "SoundEQEditorCommands.h"

#define LOCTEXT_NAMESPACE "FSoundEQEditorModule"

void FSoundEQEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "SoundEQEditor", "Bring up SoundEQEditor window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
