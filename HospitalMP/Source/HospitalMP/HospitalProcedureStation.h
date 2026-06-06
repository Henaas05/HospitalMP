#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HospitalGameplayTypes.h"
#include "HospitalProcedureStation.generated.h"

class AHospitalPatient;

UCLASS(Blueprintable)
class HOSPITALMP_API AHospitalProcedureStation : public AActor
{
    GENERATED_BODY()
public:
    AHospitalProcedureStation();

    UPROPERTY(EditAnywhere, BlueprintReadOnly) EClinicalOrderType ProcedureType = EClinicalOrderType::BloodPressure;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EHospitalRole RequiredRole = EHospitalRole::Nurse;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bCreatesSpecimen = false;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bRequiresMinigameQuality = true;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable) bool CanUseStation(AHospitalPatient* Patient, APlayerController* User) const;
    UFUNCTION(BlueprintCallable) void CompleteProcedure(AHospitalPatient* Patient, FGuid OrderId, float MinigameQuality, FText ResultText);
};
