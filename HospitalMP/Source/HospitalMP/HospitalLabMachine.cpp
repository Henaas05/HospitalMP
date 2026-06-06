#include "HospitalLabMachine.h"
#include "HospitalPatient.h"

bool AHospitalLabMachine::CanAnalyze(const FLabSpecimen& Specimen) const
{
    if (Specimen.SpecimenType != SupportedSpecimenType) return false;
    if (Specimen.State == ESpecimenState::Contaminated) return false;
    if (bRequiresCooling && Specimen.State != ESpecimenState::Cooled && Specimen.State != ESpecimenState::ReadyForAnalysis) return false;
    if (bRequiresCentrifuge && Specimen.State != ESpecimenState::Centrifuged && Specimen.State != ESpecimenState::ReadyForAnalysis) return false;
    return true;
}

void AHospitalLabMachine::AnalyzeSpecimen(AHospitalPatient* Patient, FGuid SpecimenId, const TArray<FNumericLabResult>& GeneratedResults)
{
    if (!Patient)
    {
        return;
    }

    Patient->Server_RegisterLabResult(SpecimenId, GeneratedResults);
}

