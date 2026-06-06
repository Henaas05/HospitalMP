#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HospitalGameplayTypes.h"
#include "HospitalProgressionComponent.generated.h"

USTRUCT(BlueprintType)
struct FRoleProgression
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EHospitalRole Role = EHospitalRole::None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 XP = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Level = 1;
};

UCLASS(ClassGroup=(Hospital), meta=(BlueprintSpawnableComponent))
class HOSPITALMP_API UHospitalProgressionComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FRoleProgression> RoleProgression;

    UFUNCTION(BlueprintCallable) void AddXP(EHospitalRole Role, int32 XPAmount);
    UFUNCTION(BlueprintCallable) int32 GetLevel(EHospitalRole Role) const;
    UFUNCTION(BlueprintCallable) bool IsFeatureUnlocked(FName FeatureId, EHospitalRole Role) const;
};
