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

                QuestionWidgetRef->OnAnswerSelected.BindUObject(this, &ABFIQuestionnaireGameMode::RecordResponse);

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
        "1. Is talkative", 
        "2. Tends to find fault with others", 
        "3. Does a thorough job",
        "4. Does a thorough job",
        "5. Does a thorough job",
        "6. Does a thorough job",
        "7. Does a thorough job",
        "8. Does a thorough job",
        "9. Does a thorough job",
        "10. Does a thorough job",
        "11. Does a thorough job",
        "12. Does a thorough job",
        "13. Does a thorough job",
        "14. Does a thorough job",
        "15. Does a thorough job",
        "16. Does a thorough job",
        "17. Does a thorough job",
        "18. Does a thorough job",
        "19. Does a thorough job",
        "20. Does a thorough job",
        "21. Does a thorough job",
        "22. Does a thorough job",
        "23. Does a thorough job",
        "24. Does a thorough job",
        "25. Does a thorough job",
        "26. Does a thorough job",
        "27. Does a thorough job",
        "28. Does a thorough job",
        "29. Does a thorough job",
        "30. Does a thorough job",
        "31. Does a thorough job",
        "32. Does a thorough job",
        "33. Does a thorough job",
        "34. Does a thorough job",
        "35. Does a thorough job",
        "36. Does a thorough job",
        "37. Does a thorough job",
        "38. Does a thorough job",
        "39. Does a thorough job",
        "40. Does a thorough job",
        "41. Does a thorough job",
        "42. Does a thorough job",
        "43. Does a thorough job",
        "44. Is sophisticated in art, music, or literature"
    };

    // Reverse scored questions
    ReverseScoredQuestions = { 6, 21,31, 2, 12, 27, 37, 8, 18, 23, 43, 9, 24, 34, 35, 41 };

    // Mapping questions to personality categories
    QuestionCategoryMap = {
        {1, "Extraversion"}, 
        {6, "Extraversion"}, 
        {11, "Extraversion"}, 
        {16, "Extraversion"}, 
        {21, "Extraversion"}, 
        {26, "Extraversion"}, 
        {31, "Extraversion"}, 
        {36, "Extraversion"},
        {2, "Agreeableness"}, 
        {7, "Agreeableness"}, 
        {12, "Agreeableness"}, 
        {17, "Agreeableness"}, 
        {22, "Agreeableness"}, 
        {27, "Agreeableness"}, 
        {32, "Agreeableness"}, 
        {37, "Agreeableness"}, 
        {42, "Agreeableness"},
        {3, "Conscientiousness"}, 
        {8, "Conscientiousness"}, 
        {13, "Conscientiousness"}, 
        {18, "Conscientiousness"}, 
        {23, "Conscientiousness"},
        {28, "Conscientiousness"},
        {33, "Conscientiousness"},
        {38, "Conscientiousness"},
        {43, "Conscientiousness"},
        {4, "Neuroticism"}, 
        {9, "Neuroticism"}, 
        {14, "Neuroticism"}, 
        {19, "Neuroticism"}, 
        {24, "Neuroticism"},
        {29, "Neuroticism"},
        {34, "Neuroticism"},
        {39, "Neuroticism"},
        {5, "Openness"}, 
        {10, "Openness"}, 
        {15, "Openness"}, 
        {20, "Openness"},
        {25, "Openness"},
        {30, "Openness"},
        {35, "Openness"},
        {40, "Openness"},
        {41, "Openness"},
        {44, "Openness"}
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

void ABFIQuestionnaireGameMode::RecordResponse(int32 Score)
{
    CurrentQuestionIndex++;
    UE_LOG(LogTemp, Warning, TEXT("Successfully %d found the UI Widget!"),CurrentQuestionIndex);


    if (CurrentQuestionIndex < Questions.Num())
    {
        int32 AdjustedScore = ReverseScoredQuestions.Contains(CurrentQuestionIndex + 1) ? 6 - Score : Score;

        FString* Category = QuestionCategoryMap.Find(CurrentQuestionIndex + 1);
        if (Category)
        {
            if (*Category == "Extraversion") ExtraversionScore += AdjustedScore;
            else if (*Category == "Agreeableness") AgreeablenessScore += AdjustedScore;
            else if (*Category == "Conscientiousness") ConscientiousnessScore += AdjustedScore;
            else if (*Category == "Neuroticism") NeuroticismScore += AdjustedScore;
            else if (*Category == "Openness") OpennessScore += AdjustedScore;
        }

    }
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
