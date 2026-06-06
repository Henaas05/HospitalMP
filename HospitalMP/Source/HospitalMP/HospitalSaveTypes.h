#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HospitalGameplayTypes.h"
#include "HospitalProgressionComponent.h"
#include "HospitalSaveTypes.generated.h"

USTRUCT(BlueprintType)
struct FSavedHospitalProgression
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FRoleProgression> RoleProgression;
};

USTRUCT(BlueprintType)
struct FSavedHospitalPatient
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FPatientIdentity Identity;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EPatientFlowState FlowState = EPatientFlowState::None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ETriageLevel TriageLevel = ETriageLevel::Green;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FVitalSigns CurrentVitals;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FText> VisibleSymptoms;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FJournalEntry> Journal;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FClinicalOrder> Orders;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FLabSpecimen> Specimens;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText SelectedDiagnosis;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Condition = 100.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FSoftObjectPath CaseAssetPath;
};

USTRUCT(BlueprintType)
struct FSavedHospitalWorld
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FSavedHospitalPatient> ActivePatients;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FAppointmentData> Appointments;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FDateTime SavedAt;
};

UCLASS(BlueprintType)
class HOSPITALMP_API UHospitalSaveGame : public USaveGame
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString SaveVersion = TEXT("HospitalCore_v03");
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FSavedHospitalWorld WorldData;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FSavedHospitalProgression ProgressionData;
};
