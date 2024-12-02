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

	UPROPERTY(EditAnywhere, Category = "DeleteSettings", Meta = (DisplayName = "最多显示资产数量(防止卡顿)"))
	int32 MaxDisplayAssetNum = 1000;


	UPROPERTY(EditAnywhere, Category = "DeleteSettings", Meta = (DisplayName = "只展示被引用数量小于该值的资产(防止卡顿)"))
	int32 OnlyShowLessReferenceNum = 5;


	UPROPERTY(EditAnywhere, Category = "DeleteSettings", Meta = (DisplayName = "白名单资产(不会被删除)"))
	TArray<FString> WhiteAssetName;

public:

};
