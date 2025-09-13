// 
// TalentButton.cpp
// 
// Implementation of the TalentButton class.
// 

#include "TalentButton.h"
#include "TalentTreeSelector.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "TalentTree/Character/TalentTreeCharacter.h"

void UTalentButton::NativeConstruct()
{
	Super::NativeConstruct();

	Counter = 0;
	bIsLocked = false;
	if (const UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(DescriptionTab->Slot))
	{
		DescriptionTabYPosition = CanvasSlot->GetPosition().Y;
		DescriptionTabYSize = CanvasSlot->GetSize().Y;
	}
	AvailableRankColor = FLinearColor(
		CurrentRank->GetColorAndOpacity().GetSpecifiedColor().R,
		CurrentRank->GetColorAndOpacity().GetSpecifiedColor().G,
		CurrentRank->GetColorAndOpacity().GetSpecifiedColor().B,
		CurrentRank->GetColorAndOpacity().GetSpecifiedColor().A
		);
	
	//Changes only the image and keeps all other configurations to button styles
	FButtonStyle ButtonStyle = Talent->GetStyle();
	NormalBrush = ButtonStyle.Normal;
	HoveredBrush = ButtonStyle.Hovered;
	PressedBrush = ButtonStyle.Pressed;
	DisabledBrush = ButtonStyle.Disabled;
	NormalBrush.SetResourceObject(Image);
	HoveredBrush.SetResourceObject(Image);
	PressedBrush.SetResourceObject(Image);
	DisabledBrush.SetResourceObject(Image);
	ButtonStyle.SetNormal(NormalBrush);
	ButtonStyle.SetHovered(HoveredBrush);
	ButtonStyle.SetPressed(PressedBrush);
	ButtonStyle.SetDisabled(DisabledBrush);
	Talent->SetStyle(ButtonStyle);
	
	Title->SetText(FText::FromString(TalentName));
	TotalRank->SetText(FText::AsNumber(MaxRank));
	CurrentDescription->SetText(FText::FromString(Description));
	NextDescription->SetText(FText::FromString(Description));
	HideDescriptionTab();

	//Button Bindings
	Talent->OnClicked.AddDynamic(this, &UTalentButton::AddRank);
	Talent->OnHovered.AddDynamic(this, &UTalentButton::ShowDescriptionTab);
	Talent->OnUnhovered.AddDynamic(this, &UTalentButton::HideDescriptionTab);

	if (const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ATalentTreeCharacter* Character = Cast<ATalentTreeCharacter>(PlayerController->GetPawn()))
		{
			TalentTreeCharacter = Character;
		}
	}
}

FReply UTalentButton::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if (!bIsLocked)
		{
			if (Counter > 0)
			{
				--Counter;
				TalentTreeCharacter->SetTalentPoints(TalentTreeCharacter->GetTalentPoints() + 1);
				OnTalentClicked.Broadcast(this, -1);
				(void) TalentTreeCharacter->OnTalentPointsUpdate.ExecuteIfBound(TalentTreeCharacter->GetTalentPoints());
				VisualUpdate();
			}
			return FReply::Handled();
		}
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UTalentButton::AddRank()
{
	if (!bIsLocked && TalentTreeCharacter->GetTalentPoints() > 0)
	{
		if (Counter < MaxRank)
		{
			++Counter;
			TalentTreeCharacter->SetTalentPoints(TalentTreeCharacter->GetTalentPoints() - 1);
			OnTalentClicked.Broadcast(this, 1);
			(void) TalentTreeCharacter->OnTalentPointsUpdate.ExecuteIfBound(TalentTreeCharacter->GetTalentPoints());
			VisualUpdate();
		}
	}
}

void UTalentButton::ShowDescriptionTab()
{
	DescriptionTab->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UTalentButton::HideDescriptionTab()
{
	DescriptionTab->SetVisibility(ESlateVisibility::Collapsed);
}

void UTalentButton::VisualUpdate() const
{
	const FLinearColor CurrentRankColor = FLinearColor(
		CurrentRank->GetColorAndOpacity().GetSpecifiedColor().R,
		CurrentRank->GetColorAndOpacity().GetSpecifiedColor().G,
		CurrentRank->GetColorAndOpacity().GetSpecifiedColor().B,
		CurrentRank->GetColorAndOpacity().GetSpecifiedColor().A
		);
	CurrentRank->SetText(FText::AsNumber(Counter));
	CurrentRankTab->SetText(FText::AsNumber(Counter));
	if (!AvailableRankColor.Equals(CurrentRankColor))
	{
		CurrentRank->SetColorAndOpacity(AvailableRankColor);
		NextRankVB->SetVisibility(ESlateVisibility::HitTestInvisible);
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(DescriptionTab->Slot))
		{
			FVector2D Pos = CanvasSlot->GetPosition();
			FVector2D Size = CanvasSlot->GetSize();
			Pos.Y = DescriptionTabYPosition;
			Size.Y = DescriptionTabYSize;
			CanvasSlot->SetPosition(Pos);
			CanvasSlot->SetSize(Size);
		}
	}
	if (Counter == MaxRank)
	{
		CurrentRank->SetColorAndOpacity(TalentMaxedColor);
		NextRankVB->SetVisibility(ESlateVisibility::Collapsed);
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(DescriptionTab->Slot))
		{
			FVector2D Pos = CanvasSlot->GetPosition();
			FVector2D Size = CanvasSlot->GetSize();
			Pos.Y += POSITION_TO_REDUCE_DESCRIPTION_WHEN_MAXED;
			Size.Y -= SIZE_TO_REDUCE_DESCRIPTION_WHEN_MAXED;
			CanvasSlot->SetPosition(Pos);
			CanvasSlot->SetSize(Size);
		}
	}
}

void UTalentButton::LockTalent()
{
	bIsLocked = true;
	FButtonStyle ButtonStyle = Talent->GetStyle();
	ButtonStyle.SetNormal(DisabledBrush);
	ButtonStyle.SetHovered(DisabledBrush);
	ButtonStyle.SetPressed(DisabledBrush);
	Talent->SetStyle(ButtonStyle);
	if (Counter > 0)
	{
		OnTalentClicked.Broadcast(this, -Counter);
		TalentTreeCharacter->SetTalentPoints(TalentTreeCharacter->GetTalentPoints() + Counter);
		Counter = 0;
		VisualUpdate();
	}
	RankBorder->SetVisibility(ESlateVisibility::Collapsed);
}

void UTalentButton::UnlockTalent()
{
	bIsLocked = false;
	FButtonStyle ButtonStyle = Talent->GetStyle();
	ButtonStyle.SetNormal(NormalBrush);
	ButtonStyle.SetHovered(HoveredBrush);
	ButtonStyle.SetPressed(PressedBrush);
	Talent->SetStyle(ButtonStyle);
	RankBorder->SetVisibility(ESlateVisibility::HitTestInvisible);
}