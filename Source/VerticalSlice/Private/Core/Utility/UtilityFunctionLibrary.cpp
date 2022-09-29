// 2022 ChronoOwl Studios
// Jacob

#include "Core/Utility/UtilityFunctionLibrary.h"


/**
 * @name Jacob	
 * @brief Gets the project version from the project settings
 * @return The Project version as a string
 */
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


