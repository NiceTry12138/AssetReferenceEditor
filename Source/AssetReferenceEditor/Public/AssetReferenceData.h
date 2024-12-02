// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AssetReferenceData.generated.h"

struct FReferenceInfo : public TSharedFromThis<FReferenceInfo>
{
public:
	int32 DependenciesNum;
	int32 ReferencersNum;		// 资产引用	
	int32 MemoryReference;		// 内存引用	比如 ini 中
	const FAssetData AssetData;

	FReferenceInfo(const FAssetData& InAssetData, const int32& InDependenciesNum, const int32& ReferencersNum) 
		:AssetData(InAssetData), DependenciesNum(InDependenciesNum), ReferencersNum(ReferencersNum)
	{}
};

// 资产过滤
USTRUCT()
struct FAssetDeleteConfig
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 MinDependencyNum = 0;

	UPROPERTY()
	int32 MaxDependencyNum = 0;

	UPROPERTY()
	int32 MinReferenceNum = 0;

	UPROPERTY()
	int32 MaxReferenceNum = 0;
};