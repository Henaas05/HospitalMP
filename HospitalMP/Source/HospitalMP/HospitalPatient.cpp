#include "HospitalPatient.h"
#include "HospitalCaseDataAsset.h"
#include "Net/UnrealNetwork.h"

AHospitalPatient::AHospitalPatient()
{
    bReplicates = true;
    PrimaryActorTick.bCanEverTick = true;
    Identity.PatientId = FGuid::NewGuid();
}

void AHospitalPatient::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AHospitalPatient, Identity);
    DOREPLIFETIME(AHospitalPatient, FlowState);
    DOREPLIFETIME(AHospitalPatient, TriageLevel);
    DOREPLIFETIME(AHospitalPatient, CurrentVitals);
    DOREPLIFETIME(AHospitalPatient, VisibleSymptoms);
    DOREPLIFETIME(AHospitalPatient, Journal);
    DOREPLIFETIME(AHospitalPatient, Orders);
    DOREPLIFETIME(AHospitalPatient, Specimens);
    DOREPLIFETIME(AHospitalPatient, SelectedDiagnosis);
    DOREPLIFETIME(AHospitalPatient, Condition);
}

void AHospitalPatient::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (HasAuthority())
    {
        ApplyEmergencyDeterioration(DeltaSeconds);
    }
}

void AHospitalPatient::InitFromCase(UHospitalCaseDataAsset* InCaseData)
{
    if (!HasAuthority() || !InCaseData) return;
    CaseData = InCaseData;
    CurrentVitals = InCaseData->InitialVitals;
    VisibleSymptoms = InCaseData->Symptoms;
    TriageLevel = ETriageLevel::Green;
    FlowState = EPatientFlowState::AwaitingRegistration;
    OnPatientDataChanged.Broadcast();
}

bool AHospitalPatient::HasOrder(EClinicalOrderType Type) const
{
    return Orders.ContainsByPredicate([Type](const FClinicalOrder& O){ return O.OrderType == Type; });
}

bool AHospitalPatient::IsOrderCompleted(EClinicalOrderType Type) const
{
    return Orders.ContainsByPredicate([Type](const FClinicalOrder& O){ return O.OrderType == Type && O.bCompleted; });
}

void AHospitalPatient::Server_SetFlowState_Implementation(EPatientFlowState NewState) { FlowState = NewState; OnPatientDataChanged.Broadcast(); }
void AHospitalPatient::Server_SetTriage_Implementation(ETriageLevel NewLevel) { TriageLevel = NewLevel; OnPatientDataChanged.Broadcast(); }

void AHospitalPatient::Server_AddJournalEntry_Implementation(FJournalEntry Entry)
{
    Entry.Timestamp = FDateTime::Now();
    Journal.Add(Entry);
    OnPatientDataChanged.Broadcast();
}

void AHospitalPatient::Server_AddOrder_Implementation(FClinicalOrder Order)
{
    Order.OrderId = FGuid::NewGuid();
    Order.PatientId = Identity.PatientId;
    Orders.Add(Order);
    FlowState = EPatientFlowState::AwaitingNurseTask;
    OnPatientDataChanged.Broadcast();
}

void AHospitalPatient::Server_CompleteOrder_Implementation(FGuid OrderId, const FText& ResultText, float Quality)
{
    for (FClinicalOrder& Order : Orders)
    {
        if (Order.OrderId == OrderId)
        {
            Order.bCompleted = true;
            Order.ResultText = ResultText;
            Order.Quality = FMath::Clamp(Quality, 0.f, 1.f);
            break;
        }
    }
    OnPatientDataChanged.Broadcast();
}

void AHospitalPatient::Server_AddSpecimen_Implementation(FLabSpecimen Specimen)
{
    Specimen.SpecimenId = FGuid::NewGuid();
    Specimens.Add(Specimen);
    FlowState = EPatientFlowState::AwaitingLabResult;
    OnPatientDataChanged.Broadcast();
}

void AHospitalPatient::Server_UpdateSpecimenState_Implementation(FGuid SpecimenId, ESpecimenState NewState)
{
    for (FLabSpecimen& Specimen : Specimens)
    {
        if (Specimen.SpecimenId == SpecimenId)
        {
            Specimen.State = NewState;
            break;
        }
    }
    OnPatientDataChanged.Broadcast();
}

void AHospitalPatient::Server_RegisterLabResult_Implementation(FGuid SpecimenId, const TArray<FNumericLabResult>& Results)
{
    for (FLabSpecimen& Specimen : Specimens)
    {
        if (Specimen.SpecimenId == SpecimenId)
        {
            Specimen.NumericResults = Results;
            Specimen.State = ESpecimenState::ResultReady;
            break;
        }
    }

    OnPatientDataChanged.Broadcast();
    ForceNetUpdate();
}

void AHospitalPatient::Server_SetDiagnosis_Implementation(const FText& Diagnosis)
{
    SelectedDiagnosis = Diagnosis;
    FlowState = EPatientFlowState::Diagnosed;
    OnPatientDataChanged.Broadcast();
}

void AHospitalPatient::Server_ApplyTreatment_Implementation(const FText& TreatmentName)
{
    FlowState = EPatientFlowState::InTreatment;
    FJournalEntry Entry;
    Entry.Title = FText::FromString(TEXT("Treatment applied"));
    Entry.Body = TreatmentName;
    Journal.Add(Entry);
    Condition = FMath::Min(100.f, Condition + 20.f);
    OnPatientDataChanged.Broadcast();
}

void AHospitalPatient::ApplyEmergencyDeterioration(float DeltaSeconds)
{
    if (!CaseData || CaseData->EmergencyDeteriorationRate <= 0.f) return;
    const bool bCriticalState = FlowState == EPatientFlowState::EmergencyIncoming || FlowState == EPatientFlowState::InTraumaRoom;
    if (!bCriticalState) return;

    Condition -= CaseData->EmergencyDeteriorationRate * DeltaSeconds;
    if (Condition <= 0.f)
    {
        Condition = 0.f;
        FlowState = EPatientFlowState::Deceased;
    }
}

void AHospitalPatient::OnRep_PatientChanged()
{
    OnPatientDataChanged.Broadcast();
}
