// Copyright Epic Games, Inc. All Rights Reserved.

#include "SoundEQEditor.h"
#include "SoundEQEditorStyle.h"
#include "SoundEQEditorCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Blutility/Classes/EditorUtilityWidget.h"
#include "UObject/ConstructorHelpers.h"

static const FName SoundEQEditorTabName("SoundEQEditor");

#define LOCTEXT_NAMESPACE "FSoundEQEditorModule"

void FSoundEQEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FSoundEQEditorStyle::Initialize();
	FSoundEQEditorStyle::ReloadTextures();

	FSoundEQEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FSoundEQEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FSoundEQEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FSoundEQEditorModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(SoundEQEditorTabName, FOnSpawnTab::CreateRaw(this, &FSoundEQEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FSoundEQEditorTabTitle", "SoundEQEditor"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FSoundEQEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FSoundEQEditorStyle::Shutdown();

	FSoundEQEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(SoundEQEditorTabName);
}

TSharedRef<SDockTab> FSoundEQEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FSoundEQEditorModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("SoundEQEditor.cpp"))
		);
	
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
		
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FSoundEQEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(SoundEQEditorTabName);
}

void FSoundEQEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FSoundEQEditorCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FSoundEQEditorCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSoundEQEditorModule, SoundEQEditor)