// Fill out your copyright notice in the Description page of Project Settings.


#include "Slate/SReferenceListView.h"

const FName ReferenceNumName(TEXT("引用数量"));
const FName DependencyNumName(TEXT("依赖数量"));
const FName AssetScreenName(TEXT("资产图标"));
const FName AssetName(TEXT("资产名称"));
//const FName MaterialStatusNameHeaderIdName(TEXT("OptionsNameHeaderId"));

void SAssetReferenceItemListViewRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView, FReferenceInfoPtr InTreeElement)
{
	STableRow<FReferenceInfoPtr>::Construct
	(
		STableRow<FReferenceInfoPtr>::FArguments()
		.ShowWires(true)
		.Content()
		[
			//SNew(STextBlock)
			//	.Text(this, &SAssetReferenceItemListViewRow::GetName)
			//	.ColorAndOpacity_Lambda
			//	(
			//		[this]()
			//		{
			//			return WeakTreeElement.IsValid() ? WeakTreeElement.Pin()->TextColor : FSlateColor::UseForeground();
			//		}
			//	)
		],
		InOwnerTableView
	);
}

void SReferenceListView::Construct(const FArguments& InArgs)
{

	SListView::Construct(
		SListView::FArguments()
		.SelectionMode(ESelectionMode::None)					// 自己处理点击事件
		.ListItemsSource(&AllAssetReferenceInfos)
		.OnGenerateRow(this, &SReferenceListView::OnGenerateRowListView)
		.HeaderRow(
			SNew(SHeaderRow)

			+ SHeaderRow::Column(AssetName)
			.DefaultLabel(FText::FromName(AssetName))
			.HAlignCell(EHorizontalAlignment::HAlign_Left)

			+ SHeaderRow::Column(AssetScreenName)
			.DefaultLabel(FText::FromName(AssetScreenName))
			.HAlignCell(EHorizontalAlignment::HAlign_Left)

			+ SHeaderRow::Column(ReferenceNumName)
			.DefaultLabel(FText::FromName(ReferenceNumName))
			.HAlignCell(EHorizontalAlignment::HAlign_Left)

			+ SHeaderRow::Column(DependencyNumName)
			.DefaultLabel(FText::FromName(DependencyNumName))
			.HAlignCell(EHorizontalAlignment::HAlign_Left)
		)
	);
}

FReply SReferenceListView::RefreshButtonClicked()
{

	return FReply::Handled();
}

FReply SReferenceListView::DeleteButtonClicked()
{
	
	return FReply::Handled();
}

FReply SReferenceListView::FileterButtonClicked()
{

	return FReply::Handled();
}

TSharedRef<ITableRow> SReferenceListView::OnGenerateRowListView(FReferenceInfoPtr Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return TSharedRef<ITableRow>();
}

TArray<FName> SReferenceListView::GetAllAssets()
{
	return TArray<FName>();
}