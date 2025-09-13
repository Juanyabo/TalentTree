// 
// TalentTreeController.cpp
// 
// Implementation of the TalentTreeController class.
// 

#include "TalentTreeController.h"
#include "EnhancedInputComponent.h"
#include "TalentTree/UI/TalentTreeHUD.h"

void ATalentTreeController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (const TObjectPtr<UEnhancedInputComponent> EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInput->BindAction(ShowTalentTreeSelector, ETriggerEvent::Started, this, &ATalentTreeController::ToggleTalentTreeSelector);
	}
	
}

void ATalentTreeController::BeginPlay()
{
	Super::BeginPlay();

	TalentTreeHUD = Cast<ATalentTreeHUD>(GetHUD());

	SetInputMode(FInputModeGameAndUI());
	SetShowMouseCursor(true);
}

void ATalentTreeController::ToggleTalentTreeSelector()
{
	if (IsValid(TalentTreeHUD) &&  IsValid(ShowTalentTreeSelector))
	{
		TalentTreeHUD->HandleTalentTreeSelector();
	}
}