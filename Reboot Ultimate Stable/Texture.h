#pragma once

#include "Object.h"
#include "UnrealString.h"
#include "reboot.h"
#include "inc.h"

class UTexture : public UObject // UStreamableRenderAsset
{
public:
};

class UTexture2D : public UTexture
{
public:
	void* GetPlatformData()
	{
		static auto ImportedSizeOffset = FindOffsetStruct("/Script/Engine.Texture2D", "ImportedSize");

		return *(void**)(__int64(this) + (ImportedSizeOffset + sizeof(UObject*)));
	}

	int GetSizeX()
	{
		static auto Blueprint_GetSizeXFn = FindObject<UFunction>(L"/Script/Engine.Texture2D.Blueprint_GetSizeX");
		int ReturnValue;
		this->ProcessEvent(Blueprint_GetSizeXFn, &ReturnValue);
		return ReturnValue;
	}

	int GetSizeY()
	{
		static auto Blueprint_GetSizeYFn = FindObject<UFunction>(L"/Script/Engine.Texture2D.Blueprint_GetSizeY");
		int ReturnValue;
		this->ProcessEvent(Blueprint_GetSizeYFn, &ReturnValue);
		return ReturnValue;
	}

	static void ConvertTextureToImage(UTexture2D* Texture, FString FilePath, FString FileName)
	{
		static auto DefaultKismetRenderingLibrary = FindObject("/Script/Engine.Default__KismetRenderingLibrary");
		static auto ExportTexture2DFn = FindObject<UFunction>(L"/Script/Engine.KismetRenderingLibrary.ExportTexture2D");

		struct
		{
			UObject* WorldContextObject;
			UTexture2D* Texture;
			FString FilePath;
			FString FileName;
		}UKismetRenderingLibrary_ExportTexture2D_Params{ GetWorld() , Texture, FilePath , FileName };

		DefaultKismetRenderingLibrary->ProcessEvent(ExportTexture2DFn, &UKismetRenderingLibrary_ExportTexture2D_Params);
	}

	static UClass* StaticClass()
	{
		static auto Class = FindObject<UClass>(L"/Script/Engine.Texture2D");
		return Class;
	}
};