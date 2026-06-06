#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HospitalGameplayTypes.h"
#include "HospitalReceptionComponent.generated.h"

class AHospitalPatient;
class AHospitalGameState;

UCLASS(ClassGroup=(Hospital), meta=(BlueprintSpawnableComponent))
class HOSPITALMP_API UHospitalReceptionComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="Hospital|Reception")
    void RegisterWalkInPatient(AHospitalPatient* Patient, FText AssignedDoctorName, FDateTime StartTime, FDateTime EndTime);

    UFUNCTION(BlueprintCallable, Category="Hospital|Reception")
    void CheckInPatient(AHospitalPatient* Patient);
};
