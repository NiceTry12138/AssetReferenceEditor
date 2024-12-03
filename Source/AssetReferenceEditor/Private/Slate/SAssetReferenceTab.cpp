// Fill out your copyright notice in the Description page of Project Settings.


#include "Slate/SAssetReferenceTab.h"
#include "Slate/SReferenceListView.h"
#include "IDetailsView.h"
#include "IStructureDetailsView.h"
#include "Config/AssetReferenceDeleteSettings.h"
#include "ObjectTools.h"
#include "Misc/ScopedSlowTask.h"

void SAssetReferenceTab::Construct(const FArguments& InArgs)
{
	SetOrientation(EOrientation::Orient_Vertical);

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.bAllowSearch = false;
	TSharedPtr<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObject(UAssetReferenceDeleteSettings::GetSettings());

	AddSlot()
		.AutoSize()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Fill)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Fill)
			[
				SNew(SVerticalBox)

				+ SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SButton)
					.Text(FText::FromString(TEXT("刷新")))
					.OnClicked(this, &SAssetReferenceTab::RefreshButtonClicked)
				]

				//+ SVerticalBox::Slot()
				//.AutoWidth()
				//.HAlign(HAlign_Fill)
				//.VAlign(VAlign_Fill)
				//[
				//	SNew(SButton)
				//	.Text(FText::FromString(TEXT("过滤")))
				//	.OnClicked(this, &SAssetReferenceTab::FileterButtonClicked)
				//]

				+SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SButton)
					.Text(FText::FromString(TEXT("删除")))
					.ToolTipText(FText::FromString(TEXT("一键删除所有没有被依赖的资产")))
					.OnClicked(this, &SAssetReferenceTab::DeleteButtonClicked)
				]
			]

			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				DetailsView.ToSharedRef()
			]
		];

	ReferenceListView = SNew(SReferenceListView);
	AddSlot()
		.FillSize(1.0f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			ReferenceListView.ToSharedRef()
		];
}

FReply SAssetReferenceTab::RefreshButtonClicked()
{
	if (ReferenceListView.IsValid())
	{
		ReferenceListView->Refresh();
	}

	return FReply::Handled();
}

FReply SAssetReferenceTab::DeleteButtonClicked()
{
	if (!ReferenceListView.IsValid())
	{
		return FReply::Handled();
	}

	auto Setting = UAssetReferenceDeleteSettings::GetSettings();
	for (int Index = 0; Index < Setting->FoolDeleteTimes; ++Index)
	{
		DeleteImpl();
	}

	return FReply::Handled();
}

FReply SAssetReferenceTab::FileterButtonClicked()
{
	return FReply::Handled();
}

void SAssetReferenceTab::DeleteImpl()
{
	auto Setting = UAssetReferenceDeleteSettings::GetSettings();

	auto AllShowAssets = ReferenceListView->GetAllAssetReferenceInfos();
	auto Settings = UAssetReferenceDeleteSettings::GetSettings();

	FScopedSlowTask SlowTask(AllShowAssets.Num() + 2, FText::FromString(TEXT("删除无引用资产")));
	SlowTask.MakeDialog();

	SlowTask.EnterProgressFrame(1, FText::FromString(TEXT("获取所有资产")));

	if (Setting->bConfirmDelete)
	{
		TArray<FAssetData> AssetToDeletes;
		for (const auto& Item : AllShowAssets)
		{
			if (Item->ReferencersNum > 0)
			{
				continue;
			}
			AssetToDeletes.Add(Item->AssetData);
		}
		ObjectTools::DeleteAssets(AssetToDeletes, true);

		ReferenceListView->Refresh();
		return ;
	}

	for (const auto& AssetInfo : AllShowAssets)
	{
		if ((AssetInfo->ReferencersNum + AssetInfo->MemoryReference) > 0)
		{
			continue;
		}
		FString DeleteAssetName = AssetInfo->AssetData.PackagePath.ToString() + TEXT("/") + AssetInfo->AssetData.AssetName.ToString();
		SlowTask.EnterProgressFrame(1, FText::FromString(FString::Format(TEXT("删除资产 -> {0}"), { DeleteAssetName })));
		UE_LOG(LogTemp, Log, TEXT("Delete Asset -> %s"), *DeleteAssetName);
		TArray<FAssetData> AssetToDeletes;
		AssetToDeletes.Add(AssetInfo->AssetData);
		ObjectTools::DeleteAssets(AssetToDeletes, false);
	}

	ReferenceListView->Refresh();
}
