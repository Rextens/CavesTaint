// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ToolBar.generated.h"

class AUserCharacter;

UCLASS()
class CAVESTAINT_API UToolBar : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AUserCharacter *playerReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 choosenSlot = 0;

	UFUNCTION(BlueprintNativeEvent)
		void changeSlotInUse();
		void changeSlotInUse_Implementation();
};
