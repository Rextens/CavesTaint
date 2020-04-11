// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Rock.generated.h"

UCLASS()
class CAVESTAINT_API ARock : public AActor
{
	GENERATED_BODY()

public:	
	UProceduralMeshComponent *mesh;

	TArray<FVector> vertices;
	TArray<int32> triangles;

	void addTriangle(int32 v1, int32 v2, int32 v3);
	void createMesh();

	// Sets default values for this actor's properties
	ARock();

	TArray<int32> test;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
