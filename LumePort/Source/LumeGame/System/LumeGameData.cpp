// Copyright Yikai Zhu.

#include "System/LumeGameData.h"
#include "System/LumeAssetManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LumeGameData)

ULumeGameData::ULumeGameData()
{
}

const ULumeGameData& ULumeGameData::ULumeGameData::Get()
{
	return ULumeAssetManager::Get().GetGameData();
}