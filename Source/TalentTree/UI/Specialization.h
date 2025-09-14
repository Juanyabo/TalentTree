// 
// Specialization.h
// 
// Widget that manages tier locking and unlocking for talents.
// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TalentTree/UI/Structs/Tier.h"
#include "Specialization.generated.h"

class UTalentButton;

DECLARE_DELEGATE_OneParam(FOnTotalPointsSpent, int32);

UCLASS()
class TALENTTREE_API USpecialization : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tiers", meta = (AllowPrivateAccess = "true"))
	TArray<int32> PointsRequiredForTiers;

	UPROPERTY()
	TArray<FTier> Tiers;
	
	UFUNCTION()
	void AddTalents(UWidget* Parent);

	UFUNCTION()
	void UpdatePointsSpent(UTalentButton* Talent, const int32 Increment);
	
	UFUNCTION()
	void HandleTier(const int32 Tier, const bool bLock) const;

public:
	FOnTotalPointsSpent OnTotalPointsSpend;

	UFUNCTION()
	void HandleTalentsVisualOnCharacterPoints(const int32 TalentPointsAvailable) const;
	
	UFUNCTION()
	int32 GetTotalPointsSpent() const;

protected:
	virtual void NativeConstruct() override;
	
};