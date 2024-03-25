#pragma once

#include "Actor.h"

#include "UnrealString.h"

class APlayerState : public AActor
{
public:
	FString& GetSavedNetworkAddress();
	FString GetPlayerName();
	int& GetPlayerID(); // for future me to deal with (this is a short on some versions).
	FString& GetPlayerNamePrivate();
	bool IsBot();
	void SetIsBot(bool NewValue);
	void SetPlayerNameInternal(const FString& S);
	void SetPlayerName(const FString& S);
	void OnRep_PlayerName();
};