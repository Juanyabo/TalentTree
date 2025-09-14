// 
// TalentButton.h
// 
// Custom button representing a talent, showing its description and improvements for the player.
// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TalentButton.generated.h"

class UButton;
class UBorder;
class UTextBlock;
class UVerticalBox;
class ATalentTreeCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTalentClicked, UTalentButton* , Talent, int32, increment);

UCLASS()
class TALENTTREE_API UTalentButton : public UUserWidget
{
	GENERATED_BODY()

	const float POSITION_TO_REDUCE_DESCRIPTION_WHEN_MAXED = 50.f;
	const float SIZE_TO_REDUCE_DESCRIPTION_WHEN_MAXED = 100.f;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* Talent;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UBorder* RankBorder;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* CurrentRank;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UBorder* DescriptionTab;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* Title;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* CurrentRankTab;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* TotalRank;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* CurrentDescription;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UVerticalBox* NextRankVB;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* NextDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock", meta = (AllowPrivateAccess = "true"))
	int32 Tier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Talent", meta = (AllowPrivateAccess = "true"))
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rank", meta = (AllowPrivateAccess = "true"))
	FLinearColor TalentMaxedColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rank", meta = (AllowPrivateAccess = "true"))
	int32 MaxRank;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Description", meta = (AllowPrivateAccess = "true"))
	FString TalentName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Description", meta = (AllowPrivateAccess = "true"))
	FString Description;

	UPROPERTY()
	int32 Counter;

	UPROPERTY()
	bool bIsLocked;

	UPROPERTY()
	float DescriptionTabYPosition;

	UPROPERTY()
	float DescriptionTabYSize;

	UPROPERTY()
	FLinearColor AvailableRankColor;

	UPROPERTY()
	FSlateBrush NormalBrush;

	UPROPERTY()
	FSlateBrush HoveredBrush;

	UPROPERTY()
	FSlateBrush PressedBrush;

	UPROPERTY()
	FSlateBrush DisabledBrush;

	UPROPERTY()
	ATalentTreeCharacter* TalentTreeCharacter;

	UFUNCTION()
	void AddRank();

	UFUNCTION()
	void ShowDescriptionTab();

	UFUNCTION()
	void HideDescriptionTab();

	UFUNCTION()
	void HandleRankIncrement(const bool bIsAdding);

	UFUNCTION()
	void ApplyRankChange(const int32 Increment);

	UFUNCTION()
	void ResetRank();
	
	UFUNCTION()
	void HandleRankVisual() const;

	UFUNCTION()
	void UpdateVisualState() const;
	
	UFUNCTION()
	void UpdateDescriptionTab() const;

public:
	FOnTalentClicked OnTalentClicked;

	UFUNCTION()
	void LockTalent();

	UFUNCTION()
	void UnlockTalent();

	FORCEINLINE UBorder* GetRankBorder() const { return RankBorder; }
	FORCEINLINE int32 GetMaxRank() const { return MaxRank; }
	FORCEINLINE int32 GetTier() const { return Tier; }
	FORCEINLINE int32 GetCounter() const { return Counter; }
	FORCEINLINE bool IsLocked() const { return bIsLocked; }

protected:
	virtual void NativeConstruct() override;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
};