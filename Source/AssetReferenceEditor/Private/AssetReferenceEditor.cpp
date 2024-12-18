// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetReferenceEditor.h"
#include "AssetReferenceEditorStyle.h"
#include "AssetReferenceEditorCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Slate/SAssetReferenceTab.h"

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
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SAssetReferenceTab)
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

	//{
	//	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
	//	{
	//		FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
	//		Section.AddMenuEntryWithCommandList(FAssetReferenceEditorCommands::Get().OpenPluginWindow, PluginCommands);
	//	}
	//}

	UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
	{
		FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
		{
			FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(
				"Asset Reference",
				FUIAction(FExecuteAction::CreateRaw(this, &FAssetReferenceEditorModule::PluginButtonClicked)),
				TAttribute<FText>(),
				TAttribute<FText>(),
				FSlateIcon()
			));
			Entry.SetCommandList(PluginCommands);
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAssetReferenceEditorModule, AssetReferenceEditor)