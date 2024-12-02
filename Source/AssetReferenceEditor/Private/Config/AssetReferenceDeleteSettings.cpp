// Fill out your copyright notice in the Description page of Project Settings.


#include "Config/AssetReferenceDeleteSettings.h"

ASSETREFERENCEEDITOR_API UAssetReferenceDeleteSettings* UAssetReferenceDeleteSettings::GetSettings()
{
	return GetMutableDefault<UAssetReferenceDeleteSettings>();
}
