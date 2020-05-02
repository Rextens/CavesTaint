// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Equipment.generated.h"

/**
 * 
 */

class AUserCharacter;

UCLASS()
class CAVESTAINT_API UEquipment : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AUserCharacter *playerReference;
};
