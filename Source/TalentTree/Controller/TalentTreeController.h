// 
// TalentTreeController.h
// 
// Custom controller for handling the HUD and inputs.
// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TalentTreeController.generated.h"

class UInputAction;
class ATalentTreeHUD;

UCLASS()
class TALENTTREE_API ATalentTreeController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShowTalentTreeSelector;
	
	UPROPERTY()
	TObjectPtr<ATalentTreeHUD> TalentTreeHUD;

	UFUNCTION()
	void ToggleTalentTreeSelector();

protected:
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;
	
};