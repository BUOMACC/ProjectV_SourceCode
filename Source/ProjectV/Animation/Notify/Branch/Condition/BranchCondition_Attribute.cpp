// Fill out your copyright notice in the Description page of Project Settings.


#include "BranchCondition_Attribute.h"

bool UBranchCondition_Attribute::CheckCondition(ACombatCharacter* Instigator)
{
    bool Result = false;

    if (UAbilitySystemComponent* ASC = Instigator->GetAbilitySystemComponent())
    {
        float LeftValue = ASC->GetNumericAttribute(LeftOperandAttribute);
        float RightValue = (bUseAttributeValue) ? ASC->GetNumericAttribute(RightOperandAttribute) : RightOperandValue;

        switch (Operator)
        {
        case EComparisonOperator::Greater:
            Result = LeftValue > RightValue;
            break;

        case EComparisonOperator::GreaterEqual:
            Result = LeftValue >= RightValue;
            break;

        case EComparisonOperator::Less:
            Result = LeftValue < RightValue;
            break;

        case EComparisonOperator::LessEqual:
            Result = LeftValue <= RightValue;
            break;

        case EComparisonOperator::Equal:
            Result = LeftValue == RightValue;
            break;
        }
    }
    return Result;
}
