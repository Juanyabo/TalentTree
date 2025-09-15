// 
// Specialization.cpp
// 
// Implementation of the Specialization class.
// 

#include "Specialization.h"
#include "Components/Border.h"
#include "TalentTree/UI/TalentButton.h"

void USpecialization::NativeConstruct()
{
	Super::NativeConstruct();

	FTier NewTier;
	NewTier.TotalPointsSpent = 0;
	NewTier.Talents.Empty();
	Tiers.Init(NewTier, PointsRequiredForTiers.Num() + 1);
	
	if (UWidget* Root = GetRootWidget())
	{
		AddTalents(Root);
	}
	
	for (int32 i = 1; i < Tiers.Num(); ++i)
	{
		HandleTier(i, true);
	}
}

void USpecialization::AddTalents(UWidget* Parent)
{
	if (UTalentButton* Talent = Cast<UTalentButton>(Parent))
	{
		Tiers[Talent->GetTier()].Talents.Add(Talent);
		Talent->OnTalentClicked.AddDynamic(this, &USpecialization::UpdatePointsSpent);
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
	Tiers[Talent->GetTier()].TotalPointsSpent += Increment;

	if (!Tiers.IsEmpty())
	{
		for (int32 Tier = 1; Tier < Tiers.Num(); ++Tier)
		{
			int32 PointsInPreviousTiers = 0;
		
			for (int32 PreviousTier = 0; PreviousTier < Tier; ++PreviousTier)
			{
				PointsInPreviousTiers += Tiers[PreviousTier].TotalPointsSpent;
			}

			bool bShouldUnlock = false;
		
			if (PointsRequiredForTiers.IsValidIndex(Tier - 1))
			{
				if (PointsInPreviousTiers >= PointsRequiredForTiers[Tier - 1])
				{
					bShouldUnlock = true;
				}
			}

			if (!Tiers[Tier].Talents.IsEmpty())
			{
				if (bShouldUnlock && Tiers[Tier].Talents[0]->IsLocked())
				{
					HandleTier(Tier, false);
				}
				else if (!bShouldUnlock && !Tiers[Tier].Talents[0]->IsLocked())
				{
					HandleTier(Tier, true);
					Tiers[Tier].TotalPointsSpent = 0;
				}
			}
		}
	}
	
	(void) OnTotalPointsSpend.ExecuteIfBound(GetTotalPointsSpent());
}

void USpecialization::HandleTier(const int32 Tier, const bool bLock) const
{
	if (!Tiers.IsEmpty())
	{
		if (Tiers.IsValidIndex(Tier))
		{
			for (int32 Talent = 0; Talent < Tiers[Tier].Talents.Num() ; ++Talent)
			{
				Tiers[Tier].Talents[Talent]->HandleTalent(bLock);
			}
		}
	}
}

void USpecialization::HandleTalentsVisualOnCharacterPoints(const int32 TalentPointsAvailable) const
{
	if (!Tiers.IsEmpty())
	{
		for (int32 Tier = 0; Tier < Tiers.Num(); ++Tier)
		{
			if (!Tiers[Tier].Talents.IsEmpty())
			{
				for (int32 Talent = 0; Talent < Tiers[Tier].Talents.Num(); ++Talent)
				{
					if (TalentPointsAvailable > 0)
					{
						if (Tiers[Tier].Talents[Talent]->GetCounter() < Tiers[Tier].Talents[Talent]->GetMaxRank()
							&& !Tiers[Tier].Talents[Talent]->IsLocked())
						{
							Tiers[Tier].Talents[Talent]->GetRankBorder()->SetVisibility(ESlateVisibility::HitTestInvisible);
						}
					}
					else if (Tiers[Tier].Talents[Talent]->GetCounter() == 0)
					{
						Tiers[Tier].Talents[Talent]->GetRankBorder()->SetVisibility(ESlateVisibility::Collapsed);
					}
				}
			}
		}
	}
}

int32 USpecialization::GetTotalPointsSpent() const
{
	int32 PointsSpent = 0;
	
	if (!Tiers.IsEmpty())
	{
		for (int32 Tier = 0; Tier < Tiers.Num(); ++Tier)
		{
			PointsSpent += Tiers[Tier].TotalPointsSpent; 
		}
	}
	
	return PointsSpent;
}