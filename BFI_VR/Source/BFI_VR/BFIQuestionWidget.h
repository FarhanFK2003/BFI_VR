// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BFIQuestionWidget.generated.h"


/**
 * 
 */
UCLASS()
class BFI_VR_API UBFIQuestionWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetQuestionText(FString Question);
    void ShowFinalResults(FString Results);

    // Delegate to call RecordResponse from the widget
    DECLARE_DELEGATE_OneParam(FOnAnswerSelected, int32);

    FOnAnswerSelected OnAnswerSelected;

    UFUNCTION()
    void HandleButtonClicked(int32 Score);

protected:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* QuestionTextBlock;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ResultTextBlock;

    virtual bool Initialize() override;

private:
    UPROPERTY(meta = (BindWidget))
    class UButton* AnswerButton1;

    UPROPERTY(meta = (BindWidget))
    class UButton* AnswerButton2;

    UPROPERTY(meta = (BindWidget))
    class UButton* AnswerButton3;

    UPROPERTY(meta = (BindWidget))
    class UButton* AnswerButton4;

    UPROPERTY(meta = (BindWidget))
    class UButton* AnswerButton5;

    

    UFUNCTION()
    void OnButton1Clicked();

    UFUNCTION()
    void OnButton2Clicked();

    UFUNCTION()
    void OnButton3Clicked();

    UFUNCTION()
    void OnButton4Clicked();

    UFUNCTION()
    void OnButton5Clicked();

	
};
