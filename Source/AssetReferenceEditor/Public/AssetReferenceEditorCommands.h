// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "AssetReferenceEditorStyle.h"

class FAssetReferenceEditorCommands : public TCommands<FAssetReferenceEditorCommands>
{
public:

	FAssetReferenceEditorCommands()
		: TCommands<FAssetReferenceEditorCommands>(TEXT("AssetReferenceEditor"), NSLOCTEXT("Contexts", "AssetReferenceEditor", "AssetReferenceEditor Plugin"), NAME_None, FAssetReferenceEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};