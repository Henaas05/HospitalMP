#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "HospitalGameplayTypes.generated.h"

UENUM(BlueprintType)
enum class EHospitalRole : uint8
{
    None,
    Receptionist,
    Nurse,
    Doctor,
    Radiologist,
    LabTechnician,
    Surgeon,
    Psychologist,
    Management
};

UENUM(BlueprintType)
enum class EPatientFlowState : uint8
{
    None,
    PhoneCall,
    WalkIn,
    AwaitingRegistration,
    Registered,
    WaitingRoom,
    CalledIn,
    InConsultation,
    AwaitingNurseTask,
    AwaitingLabResult,
    AwaitingImaging,
    Diagnosed,
    InTreatment,
    AdmittedWard,
    InRecovery,
    Discharged,
    EmergencyIncoming,
    InTraumaRoom,
    InSurgery,
    Deceased
};

UENUM(BlueprintType)
enum class EAppointmentType : uint8
{
    GeneralPractice,
    FollowUp,
    Emergency,
    Psychology,
    Imaging,
    SurgeryConsult
};

UENUM(BlueprintType)
enum class EClinicalOrderType : uint8
{
    None,
    BloodPressure,
    Pulse,
    Temperature,
    Auscultation,
    Palpation,
    BloodSample,
    UrineSample,
    StoolSample,
    EKG,
    XRay,
    CT,
    MRI,
    Medication,
    IVFluid,
    Surgery,
    AdmitWard,
    Discharge,
    Referral
};

UENUM(BlueprintType)
enum class ESpecimenState : uint8
{
    None,
    Ordered,
    Collected,
    Contaminated,
    NeedsCooling,
    Cooled,
    NeedsCentrifuge,
    Centrifuged,
    ReadyForAnalysis,
    Analyzing,
    ResultReady,
    RegisteredInJournal
};

UENUM(BlueprintType)
enum class ETriageLevel : uint8
{
    Green,
    Yellow,
    Orange,
    Red
};

USTRUCT(BlueprintType)
struct FVitalSigns
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SystolicBP = 120.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DiastolicBP = 80.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Pulse = 75.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Temperature = 37.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OxygenSaturation = 98.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RespiratoryRate = 16.f;
};

USTRUCT(BlueprintType)
struct FPatientIdentity
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGuid PatientId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText FullName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Age = 30;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Sex;
};

USTRUCT(BlueprintType)
struct FJournalEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime Timestamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EHospitalRole AuthorRole = EHospitalRole::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Title;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true))
    FText Body;
};

USTRUCT(BlueprintType)
struct FAppointmentData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGuid AppointmentId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGuid PatientId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EAppointmentType Type = EAppointmentType::GeneralPractice;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime StartTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime EndTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText AssignedDoctorName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCheckedIn = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCalledIn = false;
};

USTRUCT(BlueprintType)
struct FClinicalOrder
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGuid OrderId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGuid PatientId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EClinicalOrderType OrderType = EClinicalOrderType::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EHospitalRole RequiredRole = EHospitalRole::Nurse;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCompleted = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bResultRegistered = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText ResultText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Quality = 1.f;
};

USTRUCT(BlueprintType)
struct FNumericLabResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ResultId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Value = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Unit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReferenceMin = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReferenceMax = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAbnormal = false;
};

USTRUCT(BlueprintType)
struct FLabSpecimen
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGuid SpecimenId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGuid SourceOrderId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EClinicalOrderType SpecimenType = EClinicalOrderType::BloodSample;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESpecimenState State = ESpecimenState::Ordered;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ContaminationRisk = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FNumericLabResult> NumericResults;
};

USTRUCT(BlueprintType)
struct FScoreBreakdown
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CorrectDiagnosis = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CorrectQuestions = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CorrectOrders = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CorrectTreatment = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ProcedureQuality = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Efficiency = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TriageAccuracy = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalXP = 0;
};