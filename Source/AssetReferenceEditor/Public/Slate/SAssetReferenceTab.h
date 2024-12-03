// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SBoxPanel.h"
#include "AssetReferenceData.h"

class SReferenceListView;

class ASSETREFERENCEEDITOR_API SAssetReferenceTab : public SStackBox
{
public:
	SLATE_BEGIN_ARGS(SAssetReferenceTab)
	{
	}
	SLATE_END_ARGS()

	virtual void Construct(const FArguments& InArgs);

protected:

	// Button Callback Functions
	FReply RefreshButtonClicked();
	FReply DeleteButtonClicked();
	FReply FileterButtonClicked();

protected:
	void DeleteImpl();

protected:
	TSharedPtr<SReferenceListView> ReferenceListView;

	TSharedPtr<TStructOnScope<FAssetDeleteConfig>> DeleteConfig;
};
