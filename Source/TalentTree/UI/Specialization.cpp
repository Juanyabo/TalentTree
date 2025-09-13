// 
// Specialization.cpp
// 
// Implementation of the Specialization class.
// 

#include "Specialization.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "TalentTree/UI/TalentButton.h"

void USpecialization::NativeConstruct()
{
	Super::NativeConstruct();
	
	TotalPointsSpent = 0;
	MaxTier = 0; 
	if (UWidget* Root = GetRootWidget())
	{
		AddTalents(Root);
	}
	for (const auto& Elem : Talents)
	{
		if (Elem.Key > MaxTier)
		{
			MaxTier = Elem.Key;
		}
	}
	TierPoints.Init(0, MaxTier + 1);
	for (int32 i = 1; i <= MaxTier; ++i)
	{
		TierLock(i);
	}
}

void USpecialization::AddTalents(UWidget* Parent)
{
	if (UTalentButton* TalentButton = Cast<UTalentButton>(Parent))
	{
		TalentButton->OnTalentClicked.AddDynamic(this, &USpecialization::UpdatePointsSpent);
		Talents.Add(TalentButton->GetTier(), TalentButton);
	}

	if (const UPanelWidget* Panel = Cast<UPanelWidget>(Parent))
	{
		for (int32 i = 0; i < Panel->GetChildrenCount(); i++)
		{
			AddTalents(Panel->GetChildAt(i));
		}
	}
}

void USpecialization::UpdatePointsSpent(UTalentButton* Talent, const int32 Increment)
{
	if (TierPoints.IsValidIndex(Talent->GetTier()))
	{
		TierPoints[Talent->GetTier()] += Increment;
	}
	TotalPointsSpent += Increment;
	(void) OnTotalPointsSpend.ExecuteIfBound(TotalPointsSpent);
	for (int32 i = 1; i <= MaxTier; ++i)
	{
		int32 PointsInPreviousTiers = 0;
		for (int32 j = 0; j < i; ++j)
		{
			PointsInPreviousTiers += TierPoints[j];
		}
		TArray<UTalentButton*> Tier;
		Talents.MultiFind(i, Tier);
		bool bShouldUnlock = (PointsInPreviousTiers >= PointsRequiredForTiers[i - 1]);
		if (bShouldUnlock && Tier.Num() > 0 && Tier[0]->IsLocked())
		{
			TierUnLock(i);
		}
		else if (!bShouldUnlock && Tier.Num() > 0 && !Tier[0]->IsLocked())
		{
			TierLock(i);
			TierPoints[i] = 0;
		}
	}
}

void USpecialization::TierLock(const int32 Key) const
{
	TArray<UTalentButton*> Tier;
	Talents.MultiFind(Key, Tier);
	for (UTalentButton* Talent : Tier)
	{
		if (IsValid(Talent))
		{
			Talent->LockTalent();
		}
	}
}

void  USpecialization::TierUnLock(const int32 Key) const
{
	TArray<UTalentButton*> Tier;
	Talents.MultiFind(Key, Tier);
	for (UTalentButton* Talent : Tier)
	{
		if (IsValid(Talent))
		{
			Talent->UnlockTalent();
		}
	}
}

void USpecialization::NoPointsAvailable() const
{
	TArray<UTalentButton*> Tier;
	for (int32 i = 0; i <= MaxTier; ++i)
	{
		Talents.MultiFind(i, Tier);
		for (const UTalentButton* Talent : Tier)
		{
			if (Talent->GetCounter() == 0)
			{
				Talent->GetRankBorder()->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		Tier.Empty();
	}
}

void USpecialization::PointsAvailable() const
{
	TArray<UTalentButton*> Tier;
	for (int32 i = 0; i <= MaxTier; ++i)
	{
		Talents.MultiFind(i, Tier);
		for (const UTalentButton* Talent : Tier)
		{
			if (Talent->GetCounter() < Talent->GetMaxRank() && !Talent->IsLocked())
			{
				Talent->GetRankBorder()->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
		Tier.Empty();
	}
}