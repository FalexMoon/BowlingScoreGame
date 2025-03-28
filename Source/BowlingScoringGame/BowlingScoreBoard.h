// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "BaseFrame.h"
#include "BowlingScoreBoard.generated.h"

class UBaseFrame;

UCLASS()
class BOWLINGSCORINGGAME_API UBowlingScoreBoard : public UUserWidget
{
	GENERATED_BODY()

	public:
	
		UFUNCTION()
		void CalculateScores();

		UFUNCTION()
    	void OnGenRandomButtonClicked();

	protected:
    	virtual void NativeConstruct() override;

	private:
	
		UPROPERTY(meta = (BindWidget))
		class UButton* BP_GenRandomButton;

		UPROPERTY(meta = (BindWidget))
		class UBaseFrame* BP_BaseFrame_1;
	
};
