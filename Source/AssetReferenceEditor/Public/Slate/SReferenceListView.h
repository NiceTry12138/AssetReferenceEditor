// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableViewBase.h"
#include "Widgets/Views/STableRow.h"
#include "AssetReferenceData.h"

typedef TSharedPtr<FReferenceInfo> FReferenceInfoPtr;

enum class EAssetListViewSortType : uint8
{
	ENone,
	EDependencyNum,
	EReferenceNum
};

// FString PackageAsset, bool bIsSelect
DECLARE_DELEGATE_TwoParams(FOnAssetItemSelected, FString, bool);

class SAssetReferenceItemListViewRow : public SMultiColumnTableRow<FReferenceInfoPtr>
{
public:
	SLATE_BEGIN_ARGS(SAssetReferenceItemListViewRow)
	{
	}

		SLATE_EVENT(FOnAssetItemSelected, OnAssetSelected)
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView, FReferenceInfoPtr InElementInfo);

protected:
	// 回调函数
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override;

	FReply OnShowContentBtnClicked();
		
protected:
	// 成员函数

protected:
	// 属性
	FReferenceInfoPtr ElementInfo;
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

public:
	void Refresh();

	const TArray<FReferenceInfoPtr> GetAllAssetReferenceInfos() const;

protected:
	// ListView Callback Functions
	TSharedRef<ITableRow> OnGenerateRowListView(FReferenceInfoPtr ItemInfo, const TSharedRef<STableViewBase>& InOwnerTable);

	FReply OnChangeSortType(EAssetListViewSortType InSortType);

protected:
	// Tool Functions
	void Init();
	
	void FilterAssets(TArray<FName>& Assets);

	void SortAssetByType();

	inline bool IsLevelAsset(const FAssetData& AssetData);

	// Get All /Game 中的资产
	TArray<FAssetData> GetAllAssets();

protected:
	TArray<FReferenceInfoPtr> AllAssetReferenceInfos;
	
	EAssetListViewSortType SortType = EAssetListViewSortType::ENone;
};
