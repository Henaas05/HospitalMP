#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "HospitalGameplayTypes.h"
#include "HospitalScoringSubsystem.generated.h"

class AHospitalPatient;

UCLASS()
class HOSPITALMP_API UHospitalScoringSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="Hospital|Scoring")
    FScoreBreakdown ScoreCompletedPatient(AHospitalPatient* Patient, EHospitalRole Role, int32 SecondsUsed) const;
};
