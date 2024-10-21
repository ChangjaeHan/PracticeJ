// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHpWidget.h"
#include "Components/ProgressBar.h"

void UMyHpWidget::SetHealthPercent(float Percent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Percent);
	}
}
