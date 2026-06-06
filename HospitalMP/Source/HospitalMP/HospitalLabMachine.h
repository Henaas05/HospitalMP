#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HospitalGameplayTypes.h"
#include "HospitalLabMachine.generated.h"

class AHospitalPatient;

UCLASS(Blueprintable)
class HOSPITALMP_API AHospitalLabMachine : public AActor
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EClinicalOrderType SupportedSpecimenType = EClinicalOrderType::BloodSample;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bRequiresCooling = false;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bRequiresCentrifuge = false;

    UFUNCTION(BlueprintCallable) bool CanAnalyze(const FLabSpecimen& Specimen) const;
    UFUNCTION(BlueprintCallable) void AnalyzeSpecimen(AHospitalPatient* Patient, FGuid SpecimenId, const TArray<FNumericLabResult>& GeneratedResults);
};
