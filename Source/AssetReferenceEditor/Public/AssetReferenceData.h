// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
//#include "AssetReferenceData.generated.h"

struct FReferenceInfo : public TSharedFromThis<FReferenceInfo>
{
public:
	int32 DependenciesNum;
	int32 ReferencersNum;
	FString AssetPackgetPath;
	FName AssetName;

	FReferenceInfo():
		DependenciesNum(0), ReferencersNum(0)
	{}

};

// 资产过滤
struct FAssetFilter
{

};