#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HospitalGameplayTypes.h"
#include "HospitalCaseDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FCaseQuestionAnswer
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly) FName QuestionId;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FText QuestionText;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(MultiLine=true)) FText AnswerText;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bImportantForScore = false;
};

USTRUCT(BlueprintType)
struct FExpectedLabValue
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly) FName ValueName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float Value = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float NormalLow = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float NormalHigh = 0.f;
};

UCLASS(BlueprintType)
class HOSPITALMP_API UHospitalCaseDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FName CaseId;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FText CaseName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FText CorrectDiagnosis;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FText> Symptoms;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FVitalSigns InitialVitals;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) ETriageLevel CorrectTriageLevel = ETriageLevel::Green;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FCaseQuestionAnswer> AnamnesisAnswers;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<EClinicalOrderType> UsefulOrders;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<EClinicalOrderType> RequiredOrders;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FExpectedLabValue> LabResults;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FText> ValidTreatments;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float EmergencyDeteriorationRate = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bCanBecomeEmergency = false;
};
