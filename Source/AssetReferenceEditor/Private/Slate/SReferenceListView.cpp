// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetRegistry/AssetRegistryModule.h"
#include "Slate/SReferenceListView.h"
#include "Tool/ToolLibrary.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "Config/AssetReferenceDeleteSettings.h"

const FName ReferenceNumName(TEXT("被引用数量"));
const FName DependencyNumName(TEXT("依赖资产数量"));
const FName AssetPathName(TEXT("资产路径"));
const FName AssetName(TEXT("资产名称"));
const FName AssetOpBtns(TEXT("操作按钮"));

void SAssetReferenceItemListViewRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView, FReferenceInfoPtr InElementInfo)
{
	ElementInfo = InElementInfo;
	SMultiColumnTableRow<FReferenceInfoPtr>::Construct(FSuperRowType::FArguments(), InOwnerTableView);
}

TSharedRef<SWidget> SAssetReferenceItemListViewRow::GenerateWidgetForColumn(const FName& ColumnName)
{
	if (ColumnName.IsEqual(AssetName))
	{
		return SNew(STextBlock).Text(FText::FromName(ElementInfo->AssetData.AssetName));
	}
	else if (ColumnName.IsEqual(AssetPathName))
	{
		return SNew(STextBlock).Text(FText::FromName(ElementInfo->AssetData.PackagePath));
	}
	else if (ColumnName.IsEqual(ReferenceNumName))
	{
		return SNew(STextBlock).Text(FText::FromString(FString::FromInt(ElementInfo->ReferencersNum)));
	}
	else if (ColumnName.IsEqual(DependencyNumName))
	{
		return SNew(STextBlock).Text(FText::FromString(FString::FromInt(ElementInfo->DependenciesNum)));
	}
	else if (ColumnName.IsEqual(AssetOpBtns))
	{
		return SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("显示目录")))
				.OnClicked(this, &SAssetReferenceItemListViewRow::OnShowContentBtnClicked)
			];
	}
	return SNullWidget::NullWidget;
}

FReply SAssetReferenceItemListViewRow::OnShowContentBtnClicked()
{
	const TArray<FAssetData>& Assets = { ElementInfo->AssetData };
	const FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	ContentBrowserModule.Get().SyncBrowserToAssets(Assets);

	return FReply::Handled();
}

void SReferenceListView::Construct(const FArguments& InArgs)
{
	Init();

	SListView::Construct(
		SListView::FArguments()
		.SelectionMode(ESelectionMode::Single)					// 自己处理点击事件
		.ListItemsSource(&AllAssetReferenceInfos)
		.OnGenerateRow(this, &SReferenceListView::OnGenerateRowListView)
		.HeaderRow(
			SNew(SHeaderRow)

			+ SHeaderRow::Column(AssetName)
			.DefaultLabel(FText::FromName(AssetName))
			.HAlignCell(EHorizontalAlignment::HAlign_Left)
			
			+ SHeaderRow::Column(AssetPathName)
			.DefaultLabel(FText::FromName(AssetPathName))
			.HAlignCell(EHorizontalAlignment::HAlign_Left)

			+ SHeaderRow::Column(ReferenceNumName)
			.HAlignCell(EHorizontalAlignment::HAlign_Left)
			[
				SNew(SButton)
				.Text(FText::FromName(ReferenceNumName))
				.OnClicked(this, &SReferenceListView::OnChangeSortType, EAssetListViewSortType::EReferenceNum)
			]

			+ SHeaderRow::Column(DependencyNumName)
			.HAlignCell(EHorizontalAlignment::HAlign_Left)
			[
				SNew(SButton)
				.Text(FText::FromName(DependencyNumName))
				.OnClicked(this, &SReferenceListView::OnChangeSortType, EAssetListViewSortType::EDependencyNum)
			]

			+ SHeaderRow::Column(AssetOpBtns)
			.DefaultLabel(FText::FromName(AssetOpBtns))
			.HAlignCell(EHorizontalAlignment::HAlign_Left)
			
		)
	);

}

void SReferenceListView::Refresh()
{
	AllAssetReferenceInfos.Reset();

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	
	static const UE::AssetRegistry::EDependencyCategory Categories = UE::AssetRegistry::EDependencyCategory::All;
	static const UE::AssetRegistry::EDependencyQuery Flags = UE::AssetRegistry::EDependencyQuery::NoRequirements;

	auto Setting = UAssetReferenceDeleteSettings::GetSettings();

	auto AllAssets = GetAllAssets();
	for (const auto& AssetData : AllAssets)
	{
		if (Setting->bNoSearchLevelAsset && IsLevelAsset(AssetData))
		{
			continue;
		}

		TArray<FName> ReferencerNames;
		AssetRegistry.GetReferencers(AssetData.PackageName, ReferencerNames, Categories, Flags);
		FilterAssets(ReferencerNames);

		TArray<FName> DependencyNames;
		AssetRegistry.GetDependencies(AssetData.PackageName, DependencyNames, Categories, Flags);
		FilterAssets(DependencyNames);

		if (ReferencerNames.Num() > Setting->OnlyShowLessReferenceNum)
		{
			continue;
		}

		TSharedRef<FReferenceInfo> RowData = MakeShared<FReferenceInfo>(AssetData, DependencyNames.Num(), ReferencerNames.Num());
		AllAssetReferenceInfos.Add(RowData);

		if (AllAssetReferenceInfos.Num() > Setting->MaxDisplayAssetNum)
		{
			break;
		}
	}
}

TSharedRef<ITableRow> SReferenceListView::OnGenerateRowListView(FReferenceInfoPtr ItemInfo, const TSharedRef<STableViewBase>& InOwnerTable)
{
	return SNew(SAssetReferenceItemListViewRow, InOwnerTable, ItemInfo);
}

FReply SReferenceListView::OnChangeSortType(EAssetListViewSortType InSortType)
{
	SortType = InSortType;
	 
	SortAssetByType();

	return FReply::Handled();
}

void SReferenceListView::Init()
{
	Refresh();
}

void SReferenceListView::FilterAssets(TArray<FName>& Assets)
{
	int32 Num = Assets.Num();
	TArray<int> RemoveIndex;
	for (int Index = Num - 1; Index >= 0; --Index)
	{
		FName AssetItemName = Assets[Index];
		if (AssetItemName.ToString().StartsWith(TEXT("/Script")))
		{
			Assets.Remove(AssetItemName);
		}
	}

}

void SReferenceListView::SortAssetByType()
{
	AllAssetReferenceInfos.Sort([this](const FReferenceInfoPtr& A, const FReferenceInfoPtr& B) {
		if (SortType == EAssetListViewSortType::EDependencyNum)
		{
			if (A->DependenciesNum != B->DependenciesNum)
			{
				return A->DependenciesNum < B->DependenciesNum;
			}
			return A->ReferencersNum < B->ReferencersNum;
		}
		else if (SortType == EAssetListViewSortType::EReferenceNum)
		{
			if (A->ReferencersNum != B->ReferencersNum)
			{
				return A->ReferencersNum < B->ReferencersNum;
			}
			return A->DependenciesNum < B->DependenciesNum;
		}
		return true;
	});
}

inline bool SReferenceListView::IsLevelAsset(const FAssetData& AssetData)
{
	auto CheckAssetName = AssetData.AssetClassPath.GetAssetName().ToString();
	return AssetData.AssetClassPath.GetAssetName().ToString().Equals(TEXT("World"));
	//return false;
}

TArray<FAssetData> SReferenceListView::GetAllAssets()
{
	return UToolLibrary::GetAllAssetPackages();
}

const TArray<FReferenceInfoPtr> SReferenceListView::GetAllAssetReferenceInfos() const
{
	return AllAssetReferenceInfos;
}
