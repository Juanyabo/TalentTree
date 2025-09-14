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
	ShowFirstSpecialization();
	
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
}

void UTalentTreeSelector::HideTalentTreeSelector()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UTalentTreeSelector::ShowFirstSpecialization()
{
	FirstSpecializationPfp->SetVisibility(ESlateVisibility::HitTestInvisible);
	SecondSpecializationPfp->SetVisibility(ESlateVisibility::Hidden);
	ThirdSpecializationPfp->SetVisibility(ESlateVisibility::Hidden);
	FirstSpecializationName->SetColorAndOpacity(SelectedSpecialization);
	SecondSpecializationName->SetColorAndOpacity(UnselectedColor);
	ThirdSpecializationName->SetColorAndOpacity(UnselectedColor);
	FirstSpecializationTree->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SecondSpecializationTree->SetVisibility(ESlateVisibility::Collapsed);
	ThirdSpecializationTree->SetVisibility(ESlateVisibility::Collapsed);
	TalentName->SetText(FirstSpecializationName->GetText());
	UpdatePointsSpentOnFirstSpecialization(FirstSpecializationTree->GetTotalPointsSpent());
}

void UTalentTreeSelector::ShowSecondSpecialization()
{
	FirstSpecializationPfp->SetVisibility(ESlateVisibility::Hidden);
	SecondSpecializationPfp->SetVisibility(ESlateVisibility::HitTestInvisible);
	ThirdSpecializationPfp->SetVisibility(ESlateVisibility::Hidden);
	FirstSpecializationName->SetColorAndOpacity(UnselectedColor);
	SecondSpecializationName->SetColorAndOpacity(SelectedSpecialization);
	ThirdSpecializationName->SetColorAndOpacity(UnselectedColor);
	FirstSpecializationTree->SetVisibility(ESlateVisibility::Collapsed);
	SecondSpecializationTree->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	ThirdSpecializationTree->SetVisibility(ESlateVisibility::Collapsed);
	TalentName->SetText(SecondSpecializationName->GetText());
	UpdatePointsSpentOnSecondSpecialization(SecondSpecializationTree->GetTotalPointsSpent());
}

void UTalentTreeSelector::ShowThirdSpecialization()
{
	FirstSpecializationPfp->SetVisibility(ESlateVisibility::Hidden);
	SecondSpecializationPfp->SetVisibility(ESlateVisibility::Hidden);
	ThirdSpecializationPfp->SetVisibility(ESlateVisibility::HitTestInvisible);
	FirstSpecializationName->SetColorAndOpacity(UnselectedColor);
	SecondSpecializationName->SetColorAndOpacity(UnselectedColor);
	ThirdSpecializationName->SetColorAndOpacity(SelectedSpecialization);
	FirstSpecializationTree->SetVisibility(ESlateVisibility::Collapsed);
	SecondSpecializationTree->SetVisibility(ESlateVisibility::Collapsed);
	ThirdSpecializationTree->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	TalentName->SetText(ThirdSpecializationName->GetText());
	UpdatePointsSpentOnThirdSpecialization(ThirdSpecializationTree->GetTotalPointsSpent());
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

void UTalentTreeSelector::UpdateTotalPointsSpent(const int32 PointsSpent) const
{
	TotalPoints->SetText(FText::AsNumber(PointsSpent));
	if (PointsSpent == 0)
	{
		FirstSpecializationTree->NoPointsAvailable();
		SecondSpecializationTree->NoPointsAvailable();
		ThirdSpecializationTree->NoPointsAvailable();
	}
	else if (PointsSpent >= 1)
	{
		FirstSpecializationTree->PointsAvailable();
		SecondSpecializationTree->PointsAvailable();
		ThirdSpecializationTree->PointsAvailable();
	}
}