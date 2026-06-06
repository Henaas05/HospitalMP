#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HospitalGameplayTypes.h"
#include "HospitalPatient.generated.h"

class UHospitalCaseDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPatientDataChanged);

UCLASS(Blueprintable)
class HOSPITALMP_API AHospitalPatient : public ACharacter
{
    GENERATED_BODY()

public:
    AHospitalPatient();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(BlueprintAssignable) FOnPatientDataChanged OnPatientDataChanged;

    UPROPERTY(ReplicatedUsing=OnRep_PatientChanged, EditAnywhere, BlueprintReadOnly) FPatientIdentity Identity;
    UPROPERTY(ReplicatedUsing=OnRep_PatientChanged, EditAnywhere, BlueprintReadOnly) EPatientFlowState FlowState = EPatientFlowState::None;
    UPROPERTY(ReplicatedUsing=OnRep_PatientChanged, EditAnywhere, BlueprintReadOnly) ETriageLevel TriageLevel = ETriageLevel::Green;
    UPROPERTY(ReplicatedUsing=OnRep_PatientChanged, EditAnywhere, BlueprintReadOnly) FVitalSigns CurrentVitals;
    UPROPERTY(ReplicatedUsing=OnRep_PatientChanged, EditAnywhere, BlueprintReadOnly) TArray<FText> VisibleSymptoms;
    UPROPERTY(ReplicatedUsing=OnRep_PatientChanged, EditAnywhere, BlueprintReadOnly) TArray<FJournalEntry> Journal;
    UPROPERTY(ReplicatedUsing=OnRep_PatientChanged, EditAnywhere, BlueprintReadOnly) TArray<FClinicalOrder> Orders;
    UPROPERTY(ReplicatedUsing=OnRep_PatientChanged, EditAnywhere, BlueprintReadOnly) TArray<FLabSpecimen> Specimens;
    UPROPERTY(ReplicatedUsing=OnRep_PatientChanged, EditAnywhere, BlueprintReadOnly) FText SelectedDiagnosis;
    UPROPERTY(ReplicatedUsing=OnRep_PatientChanged, EditAnywhere, BlueprintReadOnly) float Condition = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly) UHospitalCaseDataAsset* CaseData;

    UFUNCTION(BlueprintCallable, Category="Hospital|Patient") void InitFromCase(UHospitalCaseDataAsset* InCaseData);
    UFUNCTION(BlueprintCallable, Category="Hospital|Patient") bool HasOrder(EClinicalOrderType Type) const;
    UFUNCTION(BlueprintCallable, Category="Hospital|Patient") bool IsOrderCompleted(EClinicalOrderType Type) const;

    UFUNCTION(Server, Reliable, BlueprintCallable) void Server_SetFlowState(EPatientFlowState NewState);
    UFUNCTION(Server, Reliable, BlueprintCallable) void Server_SetTriage(ETriageLevel NewLevel);
    UFUNCTION(Server, Reliable, BlueprintCallable) void Server_AddJournalEntry(FJournalEntry Entry);
    UFUNCTION(Server, Reliable, BlueprintCallable) void Server_AddOrder(FClinicalOrder Order);
    UFUNCTION(Server, Reliable, BlueprintCallable)
    void Server_CompleteOrder(FGuid OrderId, const FText& ResultText, float Quality);
    UFUNCTION(Server, Reliable, BlueprintCallable) void Server_AddSpecimen(FLabSpecimen Specimen);
    UFUNCTION(Server, Reliable, BlueprintCallable) void Server_UpdateSpecimenState(FGuid SpecimenId, ESpecimenState NewState);
    UFUNCTION(Server, Reliable, BlueprintCallable)
    void Server_RegisterLabResult(FGuid SpecimenId, const TArray<FNumericLabResult>& Results);
    UFUNCTION(Server, Reliable, BlueprintCallable)
    void Server_SetDiagnosis(const FText& Diagnosis);
    UFUNCTION(Server, Reliable, BlueprintCallable)
    void Server_ApplyTreatment(const FText& TreatmentName);

protected:
    UFUNCTION() void OnRep_PatientChanged();
    void ApplyEmergencyDeterioration(float DeltaSeconds);
};
