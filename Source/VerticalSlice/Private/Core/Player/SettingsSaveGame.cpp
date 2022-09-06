// 2022 ChronoOwl Studios


#include "Core/Player/SettingsSaveGame.h"

USettingsSaveGame::USettingsSaveGame() : USaveGame()
{
	MainVolume = 0.75f;
	SFXVolume = 0.75f;
	MusicVolume = 0.75f;
	MouseSensitivity = 1.0f;
	PlayerModel = 0;
	PrimaryHue = 0.0f;
	SecondaryHue = 0.0f;
}
