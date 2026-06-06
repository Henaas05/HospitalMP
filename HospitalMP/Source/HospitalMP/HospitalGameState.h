#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HospitalGameplayTypes.h"
#include "HospitalGameState.generated.h"

class AHospitalPatient;

UCLASS()
class HOSPITALMP_API AHospitalGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    AHospitalGameState();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Hospital")
    TArray<AHospitalPatient*> ActivePatients;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Hospital")
    TArray<FAppointmentData> Appointments;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Hospital")
    TArray<FClinicalOrder> GlobalOpenOrders;

    UFUNCTION(BlueprintCallable, Category = "Hospital|Patients")
    void RegisterPatient(AHospitalPatient* Patient);

    UFUNCTION(BlueprintCallable, Category = "Hospital|Patients")
    void UnregisterPatient(AHospitalPatient* Patient);

    UFUNCTION(BlueprintCallable, Category = "Hospital|Appointments")
    TArray<FAppointmentData> GetAppointmentsForDoctor(const FText& DoctorName) const;

    UFUNCTION(BlueprintCallable, Category = "Hospital|Orders")
    TArray<FClinicalOrder> GetOpenOrdersForRole(EHospitalRole InRole) const;

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Hospital|Appointments")
    void Server_AddAppointment(FAppointmentData Appointment);

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Hospital|Orders")
    void Server_AddGlobalOrder(FClinicalOrder Order);

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Hospital|Orders")
    void Server_RemoveGlobalOrder(FGuid OrderId);
};