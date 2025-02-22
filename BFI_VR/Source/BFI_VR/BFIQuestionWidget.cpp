#include "BFIQuestionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"  


void UBFIQuestionWidget::SetQuestionText(FString Question)
{
    if (QuestionTextBlock)
    {
        QuestionTextBlock->SetText(FText::FromString(Question));
    }
}

void UBFIQuestionWidget::ShowFinalResults(FString Results)
{
    if (ResultTextBlock)
    {
        ResultTextBlock->SetText(FText::FromString(Results));
    }
}


bool UBFIQuestionWidget::Initialize()
{
    Super::Initialize();

    if (AnswerButton1) AnswerButton1->OnClicked.AddDynamic(this, &UBFIQuestionWidget::OnButton1Clicked);
    if (AnswerButton2) AnswerButton2->OnClicked.AddDynamic(this, &UBFIQuestionWidget::OnButton2Clicked);
    if (AnswerButton3) AnswerButton3->OnClicked.AddDynamic(this, &UBFIQuestionWidget::OnButton3Clicked);
    if (AnswerButton4) AnswerButton4->OnClicked.AddDynamic(this, &UBFIQuestionWidget::OnButton4Clicked);
    if (AnswerButton5) AnswerButton5->OnClicked.AddDynamic(this, &UBFIQuestionWidget::OnButton5Clicked);

    return true;
}

void UBFIQuestionWidget::OnButton1Clicked() { HandleButtonClicked(1); }
void UBFIQuestionWidget::OnButton2Clicked() { HandleButtonClicked(2); }
void UBFIQuestionWidget::OnButton3Clicked() { HandleButtonClicked(3); }
void UBFIQuestionWidget::OnButton4Clicked() { HandleButtonClicked(4); }
void UBFIQuestionWidget::OnButton5Clicked() { HandleButtonClicked(5); }

void UBFIQuestionWidget::HandleButtonClicked(int32 Score)
{
    if (OnAnswerSelected.IsBound())
    {
        OnAnswerSelected.Execute(Score);
    }
}