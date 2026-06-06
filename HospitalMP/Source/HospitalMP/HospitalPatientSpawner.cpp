#include "HospitalPatientSpawner.h"
#include "HospitalPatient.h"
#include "HospitalCaseDataAsset.h"
#include "HospitalGameState.h"

AHospitalPatient* AHospitalPatientSpawner::SpawnWalkInPatient()
{
    if (!HasAuthority() || !PatientClass || AvailableCases.Num() == 0) return nullptr;
    AHospitalPatient* Patient = GetWorld()->SpawnActor<AHospitalPatient>(PatientClass, GetActorLocation(), GetActorRotation());
    if (!Patient) return nullptr;
    Patient->Identity.FullName = DefaultPatientName;
    Patient->Identity.Age = FMath::RandRange(18, 90);
    Patient->InitFromCase(AvailableCases[FMath::RandRange(0, AvailableCases.Num()-1)]);
    Patient->Server_SetFlowState(EPatientFlowState::WalkIn);
    if (AHospitalGameState* GS = GetWorld()->GetGameState<AHospitalGameState>()) GS->RegisterPatient(Patient);
    return Patient;
}

AHospitalPatient* AHospitalPatientSpawner::SpawnEmergencyPatient()
{
    AHospitalPatient* Patient = SpawnWalkInPatient();
    if (Patient)
    {
        Patient->Server_SetFlowState(EPatientFlowState::EmergencyIncoming);
        Patient->Condition = 65.f;
    }
    return Patient;
}
