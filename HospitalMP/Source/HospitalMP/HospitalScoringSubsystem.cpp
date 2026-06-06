#include "HospitalScoringSubsystem.h"
#include "HospitalPatient.h"
#include "HospitalCaseDataAsset.h"

FScoreBreakdown UHospitalScoringSubsystem::ScoreCompletedPatient(AHospitalPatient* Patient, EHospitalRole Role, int32 SecondsUsed) const
{
    FScoreBreakdown Score;
    if (!Patient || !Patient->CaseData) return Score;

    const UHospitalCaseDataAsset* Case = Patient->CaseData;
    if (Patient->SelectedDiagnosis.EqualTo(Case->CorrectDiagnosis)) Score.CorrectDiagnosis = 250;
    if (Patient->TriageLevel == Case->CorrectTriageLevel) Score.TriageAccuracy = 100;

    for (EClinicalOrderType Required : Case->RequiredOrders)
    {
        if (Patient->IsOrderCompleted(Required)) Score.CorrectOrders += 50;
    }

    for (const FClinicalOrder& Order : Patient->Orders)
    {
        if (Order.bCompleted) Score.ProcedureQuality += FMath::RoundToInt(Order.Quality * 25.f);
    }

    Score.Efficiency = FMath::Max(0, 200 - SecondsUsed / 5);
    Score.TotalXP = Score.CorrectDiagnosis + Score.CorrectQuestions + Score.CorrectOrders + Score.CorrectTreatment + Score.ProcedureQuality + Score.Efficiency + Score.TriageAccuracy;
    return Score;
}
