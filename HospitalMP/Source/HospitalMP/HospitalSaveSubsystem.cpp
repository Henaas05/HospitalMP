#include "HospitalSaveSubsystem.h"
#include "HospitalPatient.h"
#include "HospitalGameState.h"
#include "HospitalProgressionComponent.h"
#include "HospitalCaseDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UHospitalSaveGame* UHospitalSaveSubsystem::CreateSaveSnapshot(UObject* WorldContextObject, UHospitalProgressionComponent* ProgressionComponent)
{
    UHospitalSaveGame* Save = Cast<UHospitalSaveGame>(UGameplayStatics::CreateSaveGameObject(UHospitalSaveGame::StaticClass()));
    if (!Save || !WorldContextObject) return Save;

    UWorld* World = WorldContextObject->GetWorld();
    AHospitalGameState* GS = World ? World->GetGameState<AHospitalGameState>() : nullptr;
    if (GS)
    {
        Save->WorldData.Appointments = GS->Appointments;
        for (AHospitalPatient* Patient : GS->ActivePatients)
        {
            if (IsValid(Patient)) Save->WorldData.ActivePatients.Add(MakeSavedPatient(Patient));
        }
    }

    if (ProgressionComponent)
    {
        Save->ProgressionData.RoleProgression = ProgressionComponent->RoleProgression;
    }

    Save->WorldData.SavedAt = FDateTime::UtcNow();
    return Save;
}

bool UHospitalSaveSubsystem::SaveHospitalGame(UObject* WorldContextObject, UHospitalProgressionComponent* ProgressionComponent, const FString& SlotName)
{
    UHospitalSaveGame* Save = CreateSaveSnapshot(WorldContextObject, ProgressionComponent);
    const bool bOk = Save && UGameplayStatics::SaveGameToSlot(Save, SlotName.IsEmpty() ? DefaultSlotName : SlotName, DefaultUserIndex);
    OnSaveCompleted.Broadcast(bOk);
    return bOk;
}

UHospitalSaveGame* UHospitalSaveSubsystem::LoadHospitalGame(const FString& SlotName)
{
    const FString FinalSlot = SlotName.IsEmpty() ? DefaultSlotName : SlotName;
    return Cast<UHospitalSaveGame>(UGameplayStatics::LoadGameFromSlot(FinalSlot, DefaultUserIndex));
}

bool UHospitalSaveSubsystem::DoesHospitalSaveExist(const FString& SlotName) const
{
    const FString FinalSlot = SlotName.IsEmpty() ? DefaultSlotName : SlotName;
    return UGameplayStatics::DoesSaveGameExist(FinalSlot, DefaultUserIndex);
}

void UHospitalSaveSubsystem::ApplyLoadedWorld(UObject* WorldContextObject, UHospitalSaveGame* LoadedSave, TSubclassOf<AHospitalPatient> PatientClass)
{
    if (!WorldContextObject || !LoadedSave || !*PatientClass) return;
    UWorld* World = WorldContextObject->GetWorld();
    AHospitalGameState* GS = World ? World->GetGameState<AHospitalGameState>() : nullptr;
    if (!World || !GS || !World->GetAuthGameMode()) return; // only server should restore world actors

    GS->Appointments = LoadedSave->WorldData.Appointments;
    GS->ActivePatients.Empty();

    for (const FSavedHospitalPatient& SavedPatient : LoadedSave->WorldData.ActivePatients)
    {
        AHospitalPatient* Patient = World->SpawnActor<AHospitalPatient>(PatientClass, FVector::ZeroVector, FRotator::ZeroRotator);
        if (Patient)
        {
            ApplySavedPatient(Patient, SavedPatient);
            GS->RegisterPatient(Patient);
        }
    }
}

FSavedHospitalPatient UHospitalSaveSubsystem::MakeSavedPatient(AHospitalPatient* Patient)
{
    FSavedHospitalPatient Out;
    if (!Patient) return Out;
    Out.Identity = Patient->Identity;
    Out.FlowState = Patient->FlowState;
    Out.TriageLevel = Patient->TriageLevel;
    Out.CurrentVitals = Patient->CurrentVitals;
    Out.VisibleSymptoms = Patient->VisibleSymptoms;
    Out.Journal = Patient->Journal;
    Out.Orders = Patient->Orders;
    Out.Specimens = Patient->Specimens;
    Out.SelectedDiagnosis = Patient->SelectedDiagnosis;
    Out.Condition = Patient->Condition;
    if (Patient->CaseData)
    {
        Out.CaseAssetPath = FSoftObjectPath(Patient->CaseData->GetPathName());
    }
    return Out;
}

void UHospitalSaveSubsystem::ApplySavedPatient(AHospitalPatient* Patient, const FSavedHospitalPatient& SavedData)
{
    if (!Patient) return;
    Patient->Identity = SavedData.Identity;
    Patient->FlowState = SavedData.FlowState;
    Patient->TriageLevel = SavedData.TriageLevel;
    Patient->CurrentVitals = SavedData.CurrentVitals;
    Patient->VisibleSymptoms = SavedData.VisibleSymptoms;
    Patient->Journal = SavedData.Journal;
    Patient->Orders = SavedData.Orders;
    Patient->Specimens = SavedData.Specimens;
    Patient->SelectedDiagnosis = SavedData.SelectedDiagnosis;
    Patient->Condition = SavedData.Condition;
}
