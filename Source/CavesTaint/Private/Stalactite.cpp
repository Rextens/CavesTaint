// Fill out your copyright notice in the Description page of Project Settings.


#include "Stalactite.h"

// Sets default values
AStalactite::AStalactite()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStalactite::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStalactite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

