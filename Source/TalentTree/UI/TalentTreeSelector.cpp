// 
// TalentTreeSelector.cpp
// 
// Implementation of the TalentTreeSelector class.
// 

#include "TalentTreeSelector.h"
#include "Specialization.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "TalentTree/Character/TalentTreeCharacter.h"

void UTalentTreeSelector::NativeConstruct()
{
	Super::NativeConstruct();
	
	UnselectedColor = FirstSpecializationName->GetColorAndOpacity();
	bCanAddPoints = false;
	
	Close->OnClicked.AddDynamic(this, &UTalentTreeSelector::HideTalentTreeSelector);
	FirstSpecialization->OnClicked.AddDynamic(this, &UTalentTreeSelector::ShowFirstSpecialization);
	SecondSpecialization->OnClicked.AddDynamic(this, &UTalentTreeSelector::ShowSecondSpecialization);
	ThirdSpecialization->OnClicked.AddDynamic(this, &UTalentTreeSelector::ShowThirdSpecialization);
	FirstSpecializationTree->OnTotalPointsSpend.BindUObject(this, &UTalentTreeSelector::UpdatePointsSpentOnFirstSpecialization);
	SecondSpecializationTree->OnTotalPointsSpend.BindUObject(this, &UTalentTreeSelector::UpdatePointsSpentOnSecondSpecialization);
	ThirdSpecializationTree->OnTotalPointsSpend.BindUObject(this, &UTalentTreeSelector::UpdatePointsSpentOnThirdSpecialization);
	
	if (const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ATalentTreeCharacter* Character = Cast<ATalentTreeCharacter>(PlayerController->GetPawn()))
		{
			Character->OnTalentPointsUpdate.BindUObject(this, &UTalentTreeSelector::UpdateTotalPointsSpent);
		}
	}

	ShowFirstSpecialization();
}

void UTalentTreeSelector::HideTalentTreeSelector()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UTalentTreeSelector::ShowFirstSpecialization()
{
	ShowSpecialization(0);
}

void UTalentTreeSelector::ShowSecondSpecialization()
{
	ShowSpecialization(1);
}

void UTalentTreeSelector::ShowThirdSpecialization()
{
	ShowSpecialization(2);
}

void UTalentTreeSelector::UpdatePointsSpentOnFirstSpecialization(const int32 PointsSpent) const
{
	CurrentTalentPoints->SetText(FText::AsNumber(PointsSpent));
}

void UTalentTreeSelector::UpdatePointsSpentOnSecondSpecialization(const int32 PointsSpent) const
{
	CurrentTalentPoints->SetText(FText::AsNumber(PointsSpent));
}

void UTalentTreeSelector::UpdatePointsSpentOnThirdSpecialization(const int32 PointsSpent) const
{
	CurrentTalentPoints->SetText(FText::AsNumber(PointsSpent));
}

void UTalentTreeSelector::UpdateTotalPointsSpent(const int32 PointsSpent)
{
	TotalPoints->SetText(FText::AsNumber(PointsSpent));
	bool bShouldUpdate = false;
	
	if (PointsSpent > 0 && !bCanAddPoints || PointsSpent == 0)
	{
		bShouldUpdate = true;
	}

	if (bShouldUpdate)
	{
		FirstSpecializationTree->HandleTalentsVisualOnCharacterPoints(PointsSpent);
		SecondSpecializationTree->HandleTalentsVisualOnCharacterPoints(PointsSpent);
		ThirdSpecializationTree->HandleTalentsVisualOnCharacterPoints(PointsSpent);
	}
	
	if (PointsSpent > 0 )
	{
		bCanAddPoints = true;
	}
	else
	{
		bCanAddPoints = false;
	}
}

void UTalentTreeSelector::ShowSpecialization(const int32 Index) const
{
	TArray Specializations = { FirstSpecializationTree, SecondSpecializationTree, ThirdSpecializationTree };
	TArray ProfilePictures = { FirstSpecializationPfp, SecondSpecializationPfp, ThirdSpecializationPfp };
	TArray Names = { FirstSpecializationName, SecondSpecializationName, ThirdSpecializationName };

	for (int32 i = 0; i < Specializations.Num(); ++i)
	{
		if (i == Index)
		{
			ProfilePictures[i]->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			ProfilePictures[i]->SetVisibility(ESlateVisibility::Hidden);
		}
		
		if (i == Index)
		{
			Names[i]->SetColorAndOpacity(SelectedSpecialization);
		}
		else
		{
			Names[i]->SetColorAndOpacity(UnselectedColor);
		}
		
		if (i == Index)
		{
			Specializations[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			Specializations[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
	TalentName->SetText(Names[Index]->GetText());

	switch (Index)
	{
		case 0:
			UpdatePointsSpentOnFirstSpecialization(Specializations[Index]->GetTotalPointsSpent());
			break;
		case 1:
			UpdatePointsSpentOnSecondSpecialization(Specializations[Index]->GetTotalPointsSpent());
			break;
		case 2:
			UpdatePointsSpentOnThirdSpecialization(Specializations[Index]->GetTotalPointsSpent());
			break;
		default:
			break;
	}
}