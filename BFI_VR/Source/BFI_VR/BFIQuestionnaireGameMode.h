// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BFIQuestionWidget.h"
#include "BFIQuestionnaireGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BFI_VR_API ABFIQuestionnaireGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ABFIQuestionnaireGameMode();
    // Reference to the Widget
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    class UBFIQuestionWidget* QuestionWidgetRef;

protected:
    virtual void BeginPlay() override;

private:
    // Current Question Index
    int32 CurrentQuestionIndex;

    // Personality Scores
    int32 ExtraversionScore;
    int32 AgreeablenessScore;
    int32 ConscientiousnessScore;
    int32 NeuroticismScore;
    int32 OpennessScore;

    // Questions Array
    TArray<FString> Questions;

    // Reverse Scored Questions
    TSet<int32> ReverseScoredQuestions;

    // Mapping Question to Category
    TMap<int32, FString> QuestionCategoryMap;

    void LoadQuestions();  // Load questions into the array
    void UpdateQuestion(); // Update widget with the next question
    void RecordResponse( int32 Score); // Save user's response
    void CalculateFinalScores(); // Calculate & display final scores
	
};
