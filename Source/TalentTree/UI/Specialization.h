// 
// Specialization.h
// 
// Widget that manages tier locking and unlocking for talents.
// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	int32 TotalPointsSpent;

	UPROPERTY()
	int32 MaxTier;

	TMultiMap<int32, UTalentButton*> Talents;

	UPROPERTY()
	TArray<int32> TierPoints;

	UFUNCTION()
	void AddTalents(UWidget* Parent);

	UFUNCTION()
	void UpdatePointsSpent(UTalentButton* Talent, const int32 Increment);
	
	UFUNCTION()
	void TierLock(const int32 Key) const;
	
	UFUNCTION()
	void TierUnLock(const int32 Key) const;

public:
	FOnTotalPointsSpent OnTotalPointsSpend;

	UFUNCTION()
	void NoPointsAvailable() const;

	UFUNCTION()
	void PointsAvailable() const;
	
	FORCEINLINE int32 GetTotalPointsSpent() const { return TotalPointsSpent; }

protected:
	virtual void NativeConstruct() override;
	
};