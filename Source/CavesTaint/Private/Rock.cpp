// Fill out your copyright notice in the Description page of Project Settings.


#include "Rock.h"

// Sets default values
ARock::ARock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
	//RootComponent = mesh;
}

// Called when the game starts or when spawned
void ARock::BeginPlay()
{
	Super::BeginPlay();
	createMesh();
}

// Called every frame
void ARock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARock::addTriangle(int32 v1, int32 v2, int32 v3)
{
	triangles.Add(v1);
	triangles.Add(v2);
	triangles.Add(v3);
}

void ARock::createMesh()
{
	vertices.Add(FVector(100, 0, 0)); //0
	vertices.Add(FVector(100, 0, 100)); //1
	vertices.Add(FVector(100, 100, 100)); //2
	vertices.Add(FVector(100, 100, 0)); //3

	//Back face of cube
	addTriangle(0, 1, 2);	
	addTriangle(0, 2, 3);

	vertices.Add(FVector(0, 0, 0)); //4
	vertices.Add(FVector(0, 0, 100)); //5
	vertices.Add(FVector(0, 100, 100)); //6
	vertices.Add(FVector(0, 100, 0)); //7

	addTriangle(4, 6, 5);
	addTriangle(4, 7, 6);

	addTriangle(4, 1, 0);
	addTriangle(4, 5, 1);

	addTriangle(7, 2, 6);
	addTriangle(7, 3, 2);

	addTriangle(5, 2, 1);
	addTriangle(5, 6, 2);

	addTriangle(4, 0, 3);
	addTriangle(4, 3, 7);

	TArray<FColor> colors;

	TArray<FProcMeshTangent> tangents;

	mesh->CreateMeshSection(0, vertices, triangles, TArray<FVector>(), TArray<FVector2D>(), colors, tangents, true);
}