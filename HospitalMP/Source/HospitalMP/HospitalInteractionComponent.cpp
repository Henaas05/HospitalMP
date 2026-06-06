#include "HospitalInteractionComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"

UHospitalInteractionComponent::UHospitalInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UHospitalInteractionComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UHospitalInteractionComponent::RefreshFocusedInteractable()
{
    APlayerController* PC = Cast<APlayerController>(GetOwner());
    if (!PC) return;

    FVector Start; FRotator Rotation;
    PC->GetPlayerViewPoint(Start, Rotation);
    const FVector End = Start + Rotation.Vector() * TraceDistance;

    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(HospitalInteractionTrace), false, PC->GetPawn());
    AActor* NewActor = nullptr;
    TArray<FHospitalInteractionOption> NewOptions;

    if (GetWorld() && GetWorld()->LineTraceSingleByChannel(Hit, Start, End, TraceChannel, Params))
    {
        AActor* HitActor = Hit.GetActor();
        if (HitActor && HitActor->GetClass()->ImplementsInterface(UHospitalInteractableInterface::StaticClass()))
        {
            NewActor = HitActor;
            NewOptions = IHospitalInteractableInterface::Execute_GetHospitalInteractionOptions(HitActor, PC);
        }
    }

    if (NewActor != CurrentInteractable || NewOptions.Num() != CurrentOptions.Num())
    {
        CurrentInteractable = NewActor;
        CurrentOptions = NewOptions;
        OnInteractableChanged.Broadcast(CurrentInteractable, CurrentOptions);
    }
}

void UHospitalInteractionComponent::TryInteract(FName InteractionId)
{
    APlayerController* PC = Cast<APlayerController>(GetOwner());
    if (!PC || !CurrentInteractable) return;

    if (!CurrentInteractable->GetClass()->ImplementsInterface(UHospitalInteractableInterface::StaticClass())) return;
    if (!IHospitalInteractableInterface::Execute_CanHospitalInteract(CurrentInteractable, PC, InteractionId)) return;

    Server_ExecuteInteraction(CurrentInteractable, InteractionId);
}

void UHospitalInteractionComponent::Server_ExecuteInteraction_Implementation(AActor* Target, FName InteractionId)
{
    APlayerController* PC = Cast<APlayerController>(GetOwner());
    if (!PC || !Target) return;
    if (!Target->GetClass()->ImplementsInterface(UHospitalInteractableInterface::StaticClass())) return;
    if (!IHospitalInteractableInterface::Execute_CanHospitalInteract(Target, PC, InteractionId)) return;

    IHospitalInteractableInterface::Execute_ExecuteHospitalInteraction(Target, PC, InteractionId);
}
