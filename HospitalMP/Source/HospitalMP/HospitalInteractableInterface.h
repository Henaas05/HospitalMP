#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HospitalGameplayTypes.h"
#include "HospitalInteractableInterface.generated.h"

class APlayerController;
class AHospitalPatient;

USTRUCT(BlueprintType)
struct FHospitalInteractionOption
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName InteractionId = NAME_None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText DisplayText;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EHospitalRole RequiredRole = EHospitalRole::None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float MaxDistance = 250.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bRequiresServerAuthority = true;
};

UINTERFACE(BlueprintType)
class HOSPITALMP_API UHospitalInteractableInterface : public UInterface
{
    GENERATED_BODY()
};

class HOSPITALMP_API IHospitalInteractableInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Hospital|Interaction")
    TArray<FHospitalInteractionOption> GetHospitalInteractionOptions(APlayerController* InteractingPlayer) const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Hospital|Interaction")
    bool CanHospitalInteract(APlayerController* InteractingPlayer, FName InteractionId) const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Hospital|Interaction")
    void ExecuteHospitalInteraction(APlayerController* InteractingPlayer, FName InteractionId);
};
