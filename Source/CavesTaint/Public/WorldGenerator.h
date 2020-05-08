// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chunk.h"
#include "FastNoise.h"
#include "WorldGenerator.generated.h"

class AUserCharacter;

#define LAST_CHUNK chunkReferences[chunkReferences.Num()-1]

UCLASS()
class CAVESTAINT_API AWorldGenerator : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AWorldGenerator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FVector> chunkPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector currentChunkPosition; //expressed in units, so chunk at possition X = 3200, shall be just 2

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector lastChunkPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		int32 chunkSize = 1600;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		int32 renderSize = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AChunk*> chunkReferences;

	UPROPERTY(EditAnywhere, Category = "World Generation")
		TSubclassOf<AChunk> blueprintChunkReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AUserCharacter *characterReference;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Chunk")
		void checkChunk();

	UFUNCTION(BlueprintCallable)
		AChunk* getCurrentChunk();

	UFUNCTION(BlueprintCallable)
		void spawnChunk(FVector side);

	UFUNCTION(BlueprintCallable)
		void removeChunk();

	FastNoise biomeNoise;
};
