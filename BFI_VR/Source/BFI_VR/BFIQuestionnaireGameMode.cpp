#include "BFIQuestionnaireGameMode.h"
#include "BFIQuestionWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"



ABFIQuestionnaireGameMode::ABFIQuestionnaireGameMode()
{
    CurrentQuestionIndex = 0;
    ExtraversionScore = 0;
    AgreeablenessScore = 0;
    ConscientiousnessScore = 0;
    NeuroticismScore = 0;
    OpennessScore = 0;
}

void ABFIQuestionnaireGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Find UIWidgetClass by tag
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("UIWidget"), FoundActors);

    if (FoundActors.Num() > 0)
    {
        AActor* UIWidgetActor = FoundActors[0]; // Get first found actor

        UWidgetComponent* WidgetComp = UIWidgetActor->FindComponentByClass<UWidgetComponent>();
        if (WidgetComp)
        {
            QuestionWidgetRef = Cast<UBFIQuestionWidget>(WidgetComp->GetWidget());

            if (QuestionWidgetRef)
            {
                UE_LOG(LogTemp, Warning, TEXT("Successfully found the UI Widget!"));
            }
        }
    }


    LoadQuestions();
    UpdateQuestion();
}

void ABFIQuestionnaireGameMode::LoadQuestions()
{
    // Load all 44 questions in order
    Questions = {
        "1. Is talkative", "2. Tends to find fault with others", "3. Does a thorough job", /* ... */ "44. Is sophisticated in art, music, or literature"
    };

    // Reverse scored questions
    ReverseScoredQuestions = { 6, 21, 2, 12, 27, 37, 8, 18, 23, 43, 9, 24, 34, 35, 41 };

    // Mapping questions to personality categories
    QuestionCategoryMap = {
        {1, "Extraversion"}, {6, "Extraversion"}, {11, "Extraversion"}, {16, "Extraversion"}, {21, "Extraversion"},
        {2, "Agreeableness"}, {7, "Agreeableness"}, {12, "Agreeableness"}, {17, "Agreeableness"}, {22, "Agreeableness"},
        {3, "Conscientiousness"}, {8, "Conscientiousness"}, {13, "Conscientiousness"}, {18, "Conscientiousness"}, {23, "Conscientiousness"},
        {4, "Neuroticism"}, {9, "Neuroticism"}, {14, "Neuroticism"}, {19, "Neuroticism"}, {24, "Neuroticism"},
        {5, "Openness"}, {10, "Openness"}, {15, "Openness"}, {20, "Openness"}, {25, "Openness"}
    };
}

void ABFIQuestionnaireGameMode::UpdateQuestion()
{
    if (CurrentQuestionIndex >= Questions.Num())
    {
        CalculateFinalScores();
        return;
    }

    if (QuestionWidgetRef)
    {
        QuestionWidgetRef->SetQuestionText(Questions[CurrentQuestionIndex]);
    }
}

void ABFIQuestionnaireGameMode::RecordResponse(int32 QuestionIndex, int32 Score)
{
    int32 AdjustedScore = ReverseScoredQuestions.Contains(QuestionIndex + 1) ? 6 - Score : Score;

    FString* Category = QuestionCategoryMap.Find(QuestionIndex + 1);
    if (Category)
    {
        if (*Category == "Extraversion") ExtraversionScore += AdjustedScore;
        else if (*Category == "Agreeableness") AgreeablenessScore += AdjustedScore;
        else if (*Category == "Conscientiousness") ConscientiousnessScore += AdjustedScore;
        else if (*Category == "Neuroticism") NeuroticismScore += AdjustedScore;
        else if (*Category == "Openness") OpennessScore += AdjustedScore;
    }

    CurrentQuestionIndex++;
    UpdateQuestion();
}

void ABFIQuestionnaireGameMode::CalculateFinalScores()
{
    if (QuestionWidgetRef)
    {
        FString Result = FString::Printf(TEXT("Extraversion: %d\nAgreeableness: %d\nConscientiousness: %d\nNeuroticism: %d\nOpenness: %d"),
            ExtraversionScore, AgreeablenessScore, ConscientiousnessScore, NeuroticismScore, OpennessScore);

        QuestionWidgetRef->ShowFinalResults(Result);
    }
}
