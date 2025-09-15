// 
// TalentButton.cpp
// 
// Implementation of the TalentButton class.
// 

#include "TalentButton.h"
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
		DescriptionTabYPositionReduced = CanvasSlot->GetPosition().Y;
		DescriptionTabYSize = CanvasSlot->GetSize().Y;
		DescriptionTabYSizeReduced = CanvasSlot->GetSize().Y;
	}

	DescriptionTabYPositionReduced += POSITION_TO_REDUCE_DESCRIPTION_WHEN_MAXED_OR_LOCKED;
	DescriptionTabYSizeReduced -= SIZE_TO_REDUCE_DESCRIPTION_WHEN_MAXED_OR_LOCKED;
	AvailableDescription = Requirement->GetText();
	
	AvailableRankColor = FLinearColor(
		CurrentRank->GetColorAndOpacity().GetSpecifiedColor().R,
		CurrentRank->GetColorAndOpacity().GetSpecifiedColor().G,
		CurrentRank->GetColorAndOpacity().GetSpecifiedColor().B,
		CurrentRank->GetColorAndOpacity().GetSpecifiedColor().A
		);
	
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
	
	if (const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ATalentTreeCharacter* Character = Cast<ATalentTreeCharacter>(PlayerController->GetPawn()))
		{
			TalentTreeCharacter = Character;
		}
	}

	Talent->SetStyle(ButtonStyle);
	Title->SetText(FText::FromString(TalentName));
	TotalRank->SetText(FText::AsNumber(MaxRank));
	CurrentDescription->SetText(FText::FromString(Description));
	NextDescription->SetText(FText::FromString(Description));
	
	Talent->OnClicked.AddDynamic(this, &UTalentButton::AddRank);
	Talent->OnHovered.AddDynamic(this, &UTalentButton::ShowDescriptionTab);
	Talent->OnUnhovered.AddDynamic(this, &UTalentButton::HideDescriptionTab);

	HideDescriptionTab();
}

FReply UTalentButton::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		HandleRankIncrement(false);
		return FReply::Handled();
	}
	
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UTalentButton::AddRank()
{
	HandleRankIncrement(true);
}

void UTalentButton::ShowDescriptionTab()
{
	DescriptionTab->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UTalentButton::HideDescriptionTab()
{
	DescriptionTab->SetVisibility(ESlateVisibility::Collapsed);
}

void UTalentButton::HandleRankIncrement(const bool bIsAdding)
{
	if (bIsAdding)
	{
		if (!bIsLocked && TalentTreeCharacter->GetTalentPoints() > 0 && Counter < MaxRank)
		{
			ApplyRankChange(1);
		}
	}
	else
	{
		if (!bIsLocked && Counter > 0)
		{
			ApplyRankChange(-1);
		}
		else if (bIsLocked && Counter > 0)
		{
			ResetRank();
		}
	}
}

void UTalentButton::ApplyRankChange(const int32 Increment)
{
	Counter += Increment;
	TalentTreeCharacter->SetTalentPoints(TalentTreeCharacter->GetTalentPoints() - Increment);
	OnTalentClicked.Broadcast(this, Increment);
	(void)TalentTreeCharacter->OnTalentPointsUpdate.ExecuteIfBound(TalentTreeCharacter->GetTalentPoints());
	HandleRankVisual();
}

void UTalentButton::ResetRank()
{
	TalentTreeCharacter->SetTalentPoints(TalentTreeCharacter->GetTalentPoints() + Counter);
	OnTalentClicked.Broadcast(this, -Counter);
	Counter = 0;
	HandleRankVisual();
}

void UTalentButton::HandleRankVisual() const
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
		UpdateDescriptionTab();
		return;
	}
	
	if (Counter == MaxRank)
	{
		UpdateDescriptionTab();
	}
}

void UTalentButton::HandleTalent(const bool bLock)
{
	bIsLocked = bLock;
	
	if (bIsLocked)
	{
		HandleRankIncrement(false);
	}
	
	UpdateVisualState();
}

void UTalentButton::UpdateVisualState() const
{
	FButtonStyle ButtonStyle = Talent->GetStyle();
	
	if (bIsLocked)
	{
		ButtonStyle.SetNormal(DisabledBrush);
		ButtonStyle.SetHovered(DisabledBrush);
		ButtonStyle.SetPressed(DisabledBrush);
		RankBorder->SetVisibility(ESlateVisibility::Collapsed);
		Requirement->SetColorAndOpacity(LockedColor);
		HandleNextDescription(false);
	}
	else
	{
		ButtonStyle.SetNormal(NormalBrush);
		ButtonStyle.SetHovered(HoveredBrush);
		ButtonStyle.SetPressed(PressedBrush);
		RankBorder->SetVisibility(ESlateVisibility::HitTestInvisible);
		Requirement->SetColorAndOpacity(AvailableRankColor);
		HandleNextDescription(true);
	}
	
	Talent->SetStyle(ButtonStyle);
}

void UTalentButton::UpdateDescriptionTab() const
{
	if (Counter < MaxRank)
	{
		CurrentRank->SetColorAndOpacity(AvailableRankColor);
		Requirement->SetVisibility(ESlateVisibility::HitTestInvisible);
		HandleNextDescription(true);
	}
	else
	{
		CurrentRank->SetColorAndOpacity(MaxedColor);
		Requirement->SetVisibility(ESlateVisibility::Collapsed);
		HandleNextDescription(false);
	}
}

void UTalentButton::HandleNextDescription(const bool bShow) const
{
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(DescriptionTab->Slot))
	{
		FVector2D Pos = CanvasSlot->GetPosition();
		FVector2D Size = CanvasSlot->GetSize();
		
		if (bShow)
		{
			NextRankVB->SetVisibility(ESlateVisibility::HitTestInvisible);
			Pos.Y = DescriptionTabYPosition;
			Size.Y = DescriptionTabYSize;
		}
		else
		{
			NextRankVB->SetVisibility(ESlateVisibility::Collapsed);
			Pos.Y = DescriptionTabYPositionReduced;
			Size.Y = DescriptionTabYSizeReduced;
		}
		
		CanvasSlot->SetPosition(Pos);
		CanvasSlot->SetSize(Size);
	}
}