#include "HospitalProcedureStation.h"
#include "HospitalPatient.h"

AHospitalProcedureStation::AHospitalProcedureStation()
{
    bReplicates = true;
}

bool AHospitalProcedureStation::CanUseStation_Implementation(AHospitalPatient* Patient, APlayerController* User) const
{
    return Patient != nullptr;
}

void AHospitalProcedureStation::CompleteProcedure(AHospitalPatient* Patient, FGuid OrderId, float MinigameQuality, FText ResultText)
{
    if (!Patient) return;
    Patient->Server_CompleteOrder(OrderId, ResultText, MinigameQuality);

    if (bCreatesSpecimen)
    {
        FLabSpecimen Specimen;
        Specimen.SourceOrderId = OrderId;
        Specimen.SpecimenType = ProcedureType;
        Specimen.State = MinigameQuality < 0.4f ? ESpecimenState::Contaminated : ESpecimenState::Collected;
        Specimen.ContaminationRisk = 1.f - FMath::Clamp(MinigameQuality, 0.f, 1.f);
        Patient->Server_AddSpecimen(Specimen);
    }
}
