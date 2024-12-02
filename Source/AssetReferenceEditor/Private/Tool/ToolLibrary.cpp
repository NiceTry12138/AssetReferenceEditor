// Fill out your copyright notice in the Description page of Project Settings.


#include "Tool/ToolLibrary.h"
#include "Engine/ObjectLibrary.h"
#include "AssetRegistry/AssetRegistryHelpers.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Config/AssetReferenceDeleteSettings.h"

TArray<FAssetData> UToolLibrary::GetAllAssetPackages()
{
	//UObjectLibrary* ObjectLibrary = UObjectLibrary::CreateLibrary(UBlueprint::StaticClass(), false, GIsEditor);
	//if (ObjectLibrary)
	//{
	//	ObjectLibrary->AddToRoot();
	//}

	//ObjectLibrary->LoadAssetDataFromPath(TEXT("/Game"));
	//TArray<FAssetData> AssetDatas;
	//ObjectLibrary->GetAssetDataList(AssetDatas);

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(AssetRegistryConstants::ModuleName);
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	AssetRegistry.SearchAllAssets(true /* bSynchronousSearch */);
	TArray<FAssetData> AssetDataList;
	AssetRegistry.GetAllAssets(AssetDataList);

	TArray<FAssetData> Result;

	for (auto& AssetItem : AssetDataList)
	{
		bool bAdd = AssetItem.PackagePath.ToString().StartsWith(UAssetReferenceDeleteSettings::GetSettings()->SearchStartContent);
		bAdd &= !AssetItem.PackagePath.ToString().StartsWith(TEXT("/Game/__ExternalActors__"));
		bAdd &= !AssetItem.PackagePath.ToString().StartsWith(TEXT("/Game/__ExternalObjects__"));
		if (bAdd)
		{
			Result.Add(AssetItem);
		}
	}

	return Result;
}
