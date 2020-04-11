// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Stalactite.generated.h"

UCLASS()
class CAVESTAINT_API AStalactite : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStalactite();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
