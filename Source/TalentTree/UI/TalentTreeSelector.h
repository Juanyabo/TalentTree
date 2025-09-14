// 
// TalentTreeSelector.h
// 
// Widget for displaying talent trees.
// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TalentTreeSelector.generated.h"

class UButton;
class UTextBlock;
class UImage;
class USpecialization;

UCLASS()
class TALENTTREE_API UTalentTreeSelector : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* Close;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* FirstSpecialization;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* SecondSpecialization;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* ThirdSpecialization;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* FirstSpecializationName;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* SecondSpecializationName;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* ThirdSpecializationName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UImage* FirstSpecializationPfp;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UImage* SecondSpecializationPfp;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UImage* ThirdSpecializationPfp;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	USpecialization* FirstSpecializationTree;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	USpecialization* SecondSpecializationTree;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	USpecialization* ThirdSpecializationTree;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* TalentName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* CurrentTalentPoints;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* TotalPoints;
	
	UPROPERTY(EditAnywhere, Category="Selecting Specializtion", meta = (AllowPrivateAccess = true))
	FLinearColor SelectedSpecialization;

	FSlateColor UnselectedColor;

	bool bCanAddPoints;

	UFUNCTION()
	void ShowFirstSpecialization();

	UFUNCTION()
	void ShowSecondSpecialization();

	UFUNCTION()
	void ShowThirdSpecialization();

	UFUNCTION()
	void UpdatePointsSpentOnFirstSpecialization(const int32 PointsSpent) const;

	UFUNCTION()
	void UpdatePointsSpentOnSecondSpecialization(const int32 PointsSpent) const;

	UFUNCTION()
	void UpdatePointsSpentOnThirdSpecialization(const int32 PointsSpent) const;

	UFUNCTION()
	void UpdateTotalPointsSpent(const int32 PointsSpent);
	
public:
	UFUNCTION()
	void HideTalentTreeSelector();
	
protected:
	virtual void NativeConstruct() override;
	
};