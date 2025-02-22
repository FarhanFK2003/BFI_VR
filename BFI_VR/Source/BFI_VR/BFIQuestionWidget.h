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

protected:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* QuestionTextBlock;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ResultTextBlock;
	
};
