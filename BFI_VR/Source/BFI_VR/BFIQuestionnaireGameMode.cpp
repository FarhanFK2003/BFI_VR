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
        "4. Is depressed, blue",
        "5. Is original, comes up with new ideas",
        "6. Is reserved",
        "7. Is helpful and unselfish with others",
        "8. Can be somewhat careless",
        "9. Is relaxed, handles stress well",
        "10. Is curious about many different things",
        "11. Is full of energy",
        "12. Starts quarrels with others",
        "13. Is a reliable worker",
        "14. Can be tense",
        "15. Is ingenious, a deep thinker",
        "16. Generates a lot of enthusiasm",
        "17. Has a forgiving nature",
        "18. Tends to be disorganized",
        "19. Worries a lot",
        "20. Has an active imagination",
        "21. Tends to be quiet",
        "22. Is generally trusting",
        "23. Tends to be lazy",
        "24. Is emotionally stable, not easily upset",
        "25. Is inventive",
        "26. Has an assertive personality",
        "27. Can be cold and aloof",
        "28. Perseveres until the task is finished",
        "29. Can be moody",
        "30. Values artistic, aesthetic experiences",
        "31. Is sometimes shy, inhibited",
        "32. Is considerate and kind to almost everyone",
        "33. Does things efficiently",
        "34. Remains calm in tense situations",
        "35. Prefers work that is routine",
        "36. Is outgoing, sociable",
        "37. Is sometimes rude to others",
        "38. Makes plans and follows through with them",
        "39. Gets nervous easily",
        "40. Likes to reflect, play with ideas",
        "41. Has few artistic interests",
        "42. Likes to cooperate with others",
        "43. Is easily distracted",
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
