#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HospitalInteractableInterface.h"
#include "HospitalInteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHospitalInteractableChanged, AActor*, NewActor, const TArray<FHospitalInteractionOption>&, Options);

UCLASS(ClassGroup=(Hospital), meta=(BlueprintSpawnableComponent))
class HOSPITALMP_API UHospitalInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHospitalInteractionComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hospital|Interaction") float TraceDistance = 350.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hospital|Interaction") TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;
    UPROPERTY(BlueprintAssignable) FOnHospitalInteractableChanged OnInteractableChanged;
    UPROPERTY(BlueprintReadOnly) AActor* CurrentInteractable = nullptr;
    UPROPERTY(BlueprintReadOnly) TArray<FHospitalInteractionOption> CurrentOptions;

    UFUNCTION(BlueprintCallable, Category="Hospital|Interaction") void RefreshFocusedInteractable();
    UFUNCTION(BlueprintCallable, Category="Hospital|Interaction") void TryInteract(FName InteractionId);

    UFUNCTION(Server, Reliable) void Server_ExecuteInteraction(AActor* Target, FName InteractionId);

protected:
    virtual void BeginPlay() override;
};
