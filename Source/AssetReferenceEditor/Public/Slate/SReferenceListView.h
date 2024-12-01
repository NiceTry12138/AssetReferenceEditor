// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableViewBase.h"
#include "AssetReferenceData.h"

typedef TSharedPtr<FReferenceInfo> FReferenceInfoPtr;

// FString PackageAsset, bool bIsSelect
DECLARE_DELEGATE_TwoParams(FOnAssetSelected, FString, bool);

class SAssetReferenceItemListViewRow : public STableRow<FReferenceInfoPtr>
{
public:
	SLATE_BEGIN_ARGS(SAssetReferenceItemListViewRow)
	{
	}

		SLATE_EVENT(FOnAssetSelected, OnAssetSelected)
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView, FReferenceInfoPtr InTreeElement);

protected:
	// 回调函数
		
protected:
	// 成员函数

protected:
	// 属性
};

/**
 * 
 */

class ASSETREFERENCEEDITOR_API SReferenceListView : public SListView<FReferenceInfoPtr>
{
public:
	SLATE_BEGIN_ARGS(SReferenceListView)
	{
	}
	SLATE_END_ARGS()

	virtual void Construct(const FArguments& InArgs);

protected:
	// Button Callback Functions
	FReply RefreshButtonClicked();
	FReply DeleteButtonClicked();
	FReply FileterButtonClicked();

	// ListView Callback Functions
	TSharedRef<ITableRow> OnGenerateRowListView(FReferenceInfoPtr Item, const TSharedRef<STableViewBase>& OwnerTable);

protected:
	// Tool Functions

	// Get All /Game 中的资产
	TArray<FName> GetAllAssets();

protected:
	TArray<FReferenceInfoPtr> AllAssetReferenceInfos;
	
};
