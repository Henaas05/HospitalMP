#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HospitalPatientSpawner.generated.h"

class AHospitalPatient;
class UHospitalCaseDataAsset;

UCLASS(Blueprintable)
class HOSPITALMP_API AHospitalPatientSpawner : public AActor
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TSubclassOf<AHospitalPatient> PatientClass;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<UHospitalCaseDataAsset*> AvailableCases;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FText DefaultPatientName = FText::FromString(TEXT("Ola Nordmann"));

    UFUNCTION(BlueprintCallable) AHospitalPatient* SpawnWalkInPatient();
    UFUNCTION(BlueprintCallable) AHospitalPatient* SpawnEmergencyPatient();
};
