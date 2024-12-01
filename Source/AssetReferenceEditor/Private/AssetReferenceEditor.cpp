// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetReferenceEditor.h"
#include "AssetReferenceEditorStyle.h"
#include "AssetReferenceEditorCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName AssetReferenceEditorTabName("AssetReferenceEditor");

#define LOCTEXT_NAMESPACE "FAssetReferenceEditorModule"

void FAssetReferenceEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FAssetReferenceEditorStyle::Initialize();
	FAssetReferenceEditorStyle::ReloadTextures();

	FAssetReferenceEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FAssetReferenceEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FAssetReferenceEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FAssetReferenceEditorModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(AssetReferenceEditorTabName, FOnSpawnTab::CreateRaw(this, &FAssetReferenceEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FAssetReferenceEditorTabTitle", "AssetReferenceEditor"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FAssetReferenceEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FAssetReferenceEditorStyle::Shutdown();

	FAssetReferenceEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(AssetReferenceEditorTabName);
}

TSharedRef<SDockTab> FAssetReferenceEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FAssetReferenceEditorModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("AssetReferenceEditor.cpp"))
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

void FAssetReferenceEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(AssetReferenceEditorTabName);
}

void FAssetReferenceEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FAssetReferenceEditorCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FAssetReferenceEditorCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAssetReferenceEditorModule, AssetReferenceEditor)