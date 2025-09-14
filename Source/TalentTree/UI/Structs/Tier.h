#pragma once

#include "CoreMinimal.h"
#include "Tier.generated.h"

class UTalentButton;

USTRUCT(BlueprintType)
struct FTier
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UTalentButton*> Talents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalPointsSpent;
	
};