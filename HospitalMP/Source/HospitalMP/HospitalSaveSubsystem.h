#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HospitalSaveTypes.h"
#include "HospitalSaveSubsystem.generated.h"

class AHospitalPatient;
class AHospitalGameState;
class UHospitalProgressionComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHospitalSaveCompleted, bool, bSuccess);

UCLASS(BlueprintType)
class HOSPITALMP_API UHospitalSaveSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hospital|Save") FString DefaultSlotName = TEXT("HospitalSave_01");
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hospital|Save") int32 DefaultUserIndex = 0;
    UPROPERTY(BlueprintAssignable) FOnHospitalSaveCompleted OnSaveCompleted;

    UFUNCTION(BlueprintCallable, Category="Hospital|Save") UHospitalSaveGame* CreateSaveSnapshot(UObject* WorldContextObject, UHospitalProgressionComponent* ProgressionComponent);
    UFUNCTION(BlueprintCallable, Category="Hospital|Save") bool SaveHospitalGame(UObject* WorldContextObject, UHospitalProgressionComponent* ProgressionComponent, const FString& SlotName);
    UFUNCTION(BlueprintCallable, Category="Hospital|Save") UHospitalSaveGame* LoadHospitalGame(const FString& SlotName);
    UFUNCTION(BlueprintCallable, Category="Hospital|Save") bool DoesHospitalSaveExist(const FString& SlotName) const;
    UFUNCTION(BlueprintCallable, Category="Hospital|Save") void ApplyLoadedWorld(UObject* WorldContextObject, UHospitalSaveGame* LoadedSave, TSubclassOf<AHospitalPatient> PatientClass);

    UFUNCTION(BlueprintCallable, Category="Hospital|Save") static FSavedHospitalPatient MakeSavedPatient(AHospitalPatient* Patient);
    UFUNCTION(BlueprintCallable, Category="Hospital|Save") static void ApplySavedPatient(AHospitalPatient* Patient, const FSavedHospitalPatient& SavedData);
};
