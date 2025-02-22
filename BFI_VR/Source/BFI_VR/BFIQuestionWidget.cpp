#include "BFIQuestionWidget.h"
#include "Components/TextBlock.h"

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
