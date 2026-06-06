#include "HospitalGameState.h"
#include "HospitalPatient.h"
#include "Net/UnrealNetwork.h"

AHospitalGameState::AHospitalGameState()
{
    bReplicates = true;
}

void AHospitalGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AHospitalGameState, ActivePatients);
    DOREPLIFETIME(AHospitalGameState, Appointments);
    DOREPLIFETIME(AHospitalGameState, GlobalOpenOrders);
}

void AHospitalGameState::RegisterPatient(AHospitalPatient* Patient)
{
    if (!HasAuthority() || !IsValid(Patient))
    {
        return;
    }

    ActivePatients.AddUnique(Patient);
    ForceNetUpdate();
}

void AHospitalGameState::UnregisterPatient(AHospitalPatient* Patient)
{
    if (!HasAuthority() || !IsValid(Patient))
    {
        return;
    }

    ActivePatients.Remove(Patient);
    ForceNetUpdate();
}

TArray<FAppointmentData> AHospitalGameState::GetAppointmentsForDoctor(const FText& DoctorName) const
{
    TArray<FAppointmentData> Result;

    for (const FAppointmentData& Appointment : Appointments)
    {
        if (Appointment.AssignedDoctorName.EqualTo(DoctorName))
        {
            Result.Add(Appointment);
        }
    }

    return Result;
}

TArray<FClinicalOrder> AHospitalGameState::GetOpenOrdersForRole(EHospitalRole InRole) const
{
    TArray<FClinicalOrder> Result;

    for (const FClinicalOrder& Order : GlobalOpenOrders)
    {
        if (!Order.bCompleted && Order.RequiredRole == InRole)
        {
            Result.Add(Order);
        }
    }

    return Result;
}

void AHospitalGameState::Server_AddAppointment_Implementation(FAppointmentData Appointment)
{
    if (!HasAuthority())
    {
        return;
    }

    if (!Appointment.AppointmentId.IsValid())
    {
        Appointment.AppointmentId = FGuid::NewGuid();
    }

    Appointments.Add(Appointment);
    ForceNetUpdate();
}

void AHospitalGameState::Server_AddGlobalOrder_Implementation(FClinicalOrder Order)
{
    if (!HasAuthority())
    {
        return;
    }

    if (!Order.OrderId.IsValid())
    {
        Order.OrderId = FGuid::NewGuid();
    }

    GlobalOpenOrders.Add(Order);
    ForceNetUpdate();
}

void AHospitalGameState::Server_RemoveGlobalOrder_Implementation(FGuid OrderId)
{
    if (!HasAuthority())
    {
        return;
    }

    GlobalOpenOrders.RemoveAll([OrderId](const FClinicalOrder& Order)
        {
            return Order.OrderId == OrderId;
        });

    ForceNetUpdate();
}