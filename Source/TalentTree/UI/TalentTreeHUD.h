// 
// TalentTreeHUD.h
// 
// Custom HUD for creation of widgets and managing their visibility.
// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TalentTreeHUD.generated.h"

class UTalentTreeSelector;

UCLASS()
class TALENTTREE_API ATalentTreeHUD : public AHUD
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UTalentTreeSelector> TalentTreeSelectorWidget;
	
	UPROPERTY()
	UTalentTreeSelector* TalentTreeSelectorInstance;

public:
	UFUNCTION()
	void HandleTalentTreeSelector() const;
	
	FORCEINLINE UTalentTreeSelector* GetTalentTreeSelector() const { return TalentTreeSelectorInstance ;};
	
protected:
	virtual void BeginPlay() override;
	
};