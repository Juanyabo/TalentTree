// 
// TalentTreeHUD.cpp
// 
// Implementation of the TalentTreeHUD class.
// 

#include "TalentTreeHUD.h"
#include "TalentTree/UI/TalentTreeSelector.h"
#include "Blueprint/UserWidget.h"

void ATalentTreeHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsValid(TalentTreeSelectorInstance) && IsValid(TalentTreeSelectorWidget))
	{
		TalentTreeSelectorInstance = CreateWidget<UTalentTreeSelector>(GetWorld(), TalentTreeSelectorWidget);
	}

	TalentTreeSelectorInstance->AddToViewport();
	TalentTreeSelectorInstance->HideTalentTreeSelector();
}

void ATalentTreeHUD::HandleTalentTreeSelector() const
{
	if (IsValid(TalentTreeSelectorInstance))
	{
		if (!TalentTreeSelectorInstance->IsVisible())
		{
			TalentTreeSelectorInstance->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			return;
		}
		
		TalentTreeSelectorInstance->HideTalentTreeSelector();
	}
}