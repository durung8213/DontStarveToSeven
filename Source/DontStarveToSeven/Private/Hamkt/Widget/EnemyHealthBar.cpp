// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Widget/EnemyHealthBar.h"
#include "Hamkt/Enemy/EnemyBase.h"
#include "Engine/Engine.h"

float UEnemyHealthBar::GetPercent() const
{
    // HealthOwner�� ��ȿ�� ��ü�� ��� �ְ�, �������̽��� �����ߴ��� Ȯ��
    if (HealthOwner.GetObject() && HealthOwner.GetInterface())
    {
        // Execute_ �� ȣ�� (BlueprintNativeEvent)
        float Current = IBPI_EnemyAI::Execute_GetCurrentHealth(HealthOwner.GetObject());
        float Max = IBPI_EnemyAI::Execute_GetMaxHealth(HealthOwner.GetObject());

        if (Max > 0.f)
        {
            return Current / Max;
        }
    }
    return 0.f;
}
