#pragma once

#include "FortAthenaMutator.h"

#include "reboot.h"
#include "CurveTable.h"

class AFortAthenaMutator_TDM : public AFortAthenaMutator
{
public:
	float GetNumKillsForVictory()
	{
		static auto NumKillsForVictoryOffset = GetOffset("NumKillsForVictory");

		if (NumKillsForVictoryOffset != -1)
		{
			auto& NumKillsForVictoryAsScalableFloat = Get<FScalableFloat>(NumKillsForVictoryOffset);
			auto CurveTable = NumKillsForVictoryAsScalableFloat.GetCurve().CurveTable;
			auto RowName = NumKillsForVictoryAsScalableFloat.GetCurve().RowName;
			float NumKillsForVictory = UDataTableFunctionLibrary::EvaluateCurveTableRow(CurveTable, RowName, 0.f);
			return NumKillsForVictory;
		}

		return 0;
	}

	static UClass* StaticClass()
	{
		static auto Class = FindObject<UClass>(L"/Script/FortniteGame.FortAthenaMutator_TDM");
		return Class;
	}
};