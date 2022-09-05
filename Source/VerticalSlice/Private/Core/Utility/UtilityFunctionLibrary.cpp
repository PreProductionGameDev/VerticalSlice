// 2022 ChronoOwl Studios


#include "Core/Utility/UtilityFunctionLibrary.h"

FString UUtilityFunctionLibrary::GetAppVersion()
{
	FString AppVersion;
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		AppVersion,
		GGameIni
	);

	return AppVersion;
}
