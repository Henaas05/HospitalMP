#include "HospitalProgressionComponent.h"

void UHospitalProgressionComponent::AddXP(EHospitalRole Role, int32 XPAmount)
{
    FRoleProgression* Found = RoleProgression.FindByPredicate([Role](const FRoleProgression& P){ return P.Role == Role; });
    if (!Found)
    {
        FRoleProgression NewP; NewP.Role = Role; RoleProgression.Add(NewP); Found = &RoleProgression.Last();
    }
    Found->XP += XPAmount;
    Found->Level = FMath::Max(1, 1 + Found->XP / 1000);
}

int32 UHospitalProgressionComponent::GetLevel(EHospitalRole Role) const
{
    const FRoleProgression* Found = RoleProgression.FindByPredicate([Role](const FRoleProgression& P){ return P.Role == Role; });
    return Found ? Found->Level : 1;
}

bool UHospitalProgressionComponent::IsFeatureUnlocked(FName FeatureId, EHospitalRole Role) const
{
    const int32 Level = GetLevel(Role);
    if (FeatureId == "EmergencyDepartment") return Level >= 3;
    if (FeatureId == "Surgery") return Level >= 6;
    if (FeatureId == "AdvancedImaging") return Level >= 4;
    return true;
}
