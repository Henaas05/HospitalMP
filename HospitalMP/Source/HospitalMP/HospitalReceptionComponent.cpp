#include "HospitalReceptionComponent.h"
#include "HospitalPatient.h"
#include "HospitalGameState.h"
#include "Kismet/GameplayStatics.h"

void UHospitalReceptionComponent::RegisterWalkInPatient(AHospitalPatient* Patient, FText AssignedDoctorName, FDateTime StartTime, FDateTime EndTime)
{
    if (!Patient) return;
    AHospitalGameState* GS = GetWorld() ? GetWorld()->GetGameState<AHospitalGameState>() : nullptr;
    if (!GS) return;

    FAppointmentData A;
    A.PatientId = Patient->Identity.PatientId;
    A.AssignedDoctorName = AssignedDoctorName;
    A.Type = EAppointmentType::GeneralPractice;
    A.StartTime = StartTime;
    A.EndTime = EndTime;
    GS->Server_AddAppointment(A);
    Patient->Server_SetFlowState(EPatientFlowState::Registered);
}

void UHospitalReceptionComponent::CheckInPatient(AHospitalPatient* Patient)
{
    if (Patient) Patient->Server_SetFlowState(EPatientFlowState::WaitingRoom);
}
