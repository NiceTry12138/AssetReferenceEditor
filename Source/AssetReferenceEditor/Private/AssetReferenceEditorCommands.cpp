// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetReferenceEditorCommands.h"

#define LOCTEXT_NAMESPACE "FAssetReferenceEditorModule"

void FAssetReferenceEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "AssetReferenceEditor", "Bring up AssetReferenceEditor window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
