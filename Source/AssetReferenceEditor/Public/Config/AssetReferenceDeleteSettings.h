// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AssetReferenceDeleteSettings.generated.h"

/**
 * 
 */
UCLASS()
class ASSETREFERENCEEDITOR_API UAssetReferenceDeleteSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = Settings)
	static UAssetReferenceDeleteSettings* GetSettings();

public:
	UPROPERTY(EditAnywhere, Category = "DeleteSettings", Meta = (DisplayName = "搜索时跳过Level"))
	bool bNoSearchLevelAsset = true;

	UPROPERTY(EditAnywhere, Category = "DeleteSettings", Meta = (DisplayName = "检索时跳过DT"))
	bool bSkipDataTable = true;

	UPROPERTY(EditAnywhere, Category = "DeleteSettings", Meta = (DisplayName = "检索时跳过蓝图"))
	bool bSkipBlueprint = true;

	UPROPERTY(EditAnywhere, Category = "DeleteSettings", Meta = (DisplayName = "检查内存引用"))
	bool bCheckMemoryReference = true;

	UPROPERTY(EditAnywhere, Category = "DeleteSettings", Meta = (DisplayName = "最多显示资产数量(防止卡顿)"))
	int32 MaxDisplayAssetNum = 1000;


	UPROPERTY(EditAnywhere, Category = "DeleteSettings", Meta = (DisplayName = "只展示被引用数量小于该值的资产(防止卡顿)"))
	int32 OnlyShowLessReferenceNum = 5;

	UPROPERTY(EditAnywhere, Category = "DeleteSettings", Meta = (DisplayName = "白名单资产(不会被删除)"))
	FString SearchStartContent = TEXT("/Game");

	UPROPERTY(EditAnywhere, Category = "DeleteSettings", Meta = (DisplayName = "弹出确定框", Tooltip = "弹出删除确认框"))
	bool bConfirmDelete = false;

	UPROPERTY(EditAnywhere, Category = "DeleteSettings", Meta = (DisplayName = "自动删除次数", Tooltip = "资产链式引用，需要频繁删除时使用", ClampMin=1))
	int FoolDeleteTimes = 1;

	UPROPERTY(EditAnywhere, Category = "DeleteSettings", Meta = (DisplayName = "白名单资产(不会被删除)"))
	TArray<FString> WhiteAssetName;

public:

};
