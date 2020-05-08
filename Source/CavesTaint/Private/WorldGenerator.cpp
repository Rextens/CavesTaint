// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGenerator.h"
#include "UserCharacter.h"

// Sets default values
AWorldGenerator::AWorldGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	biomeNoise.SetNoiseType(FastNoise::Cellular);
	biomeNoise.SetCellularDistanceFunction(FastNoise::Natural);
}

// Called when the game starts or when spawned
void AWorldGenerator::BeginPlay()
{
	Super::BeginPlay();

	characterReference = Cast<AUserCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void AWorldGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	checkChunk();
	removeChunk();
}

void AWorldGenerator::checkChunk()
{
	for (int x = -renderSize; x < renderSize; ++x)
	{
		for (int y = -renderSize; y < renderSize; ++y)
		{
			for (int z = -renderSize; z < renderSize; ++z)
			{
				FVector side = FVector(currentChunkPosition.X + x, currentChunkPosition.Y + y, currentChunkPosition.Z + z);

				if (chunkPosition.Find(side) == -1)
				{
					spawnChunk(side);
					chunkPosition.AddUnique(side);
				}
			}
		}
	}
}

void AWorldGenerator::spawnChunk(FVector side)
{
	FActorSpawnParameters spawnParams;
	FRotator rotation(0.0f, 0.0f, 0.0f);

	chunkReferences.Add(GetWorld()->SpawnActor<AChunk>(blueprintChunkReference, FVector(side.X * chunkSize, side.Y * chunkSize, side.Z * chunkSize), rotation, spawnParams));
	LAST_CHUNK->setBiome(&biomeNoise);
	LAST_CHUNK->worldGeneratorReference = this;
}

void AWorldGenerator::removeChunk()
{
	for (int i = 0; i < chunkPosition.Num(); ++i)
	{
		if (abs(currentChunkPosition.X * chunkSize - chunkReferences[i]->GetActorLocation().X) > chunkSize * (renderSize + 1) ||
			abs(currentChunkPosition.Y * chunkSize - chunkReferences[i]->GetActorLocation().Y) > chunkSize * (renderSize + 1) ||
			abs(currentChunkPosition.Z * chunkSize - chunkReferences[i]->GetActorLocation().Z) > chunkSize * (renderSize + 1))
		{
			chunkReferences[i]->Destroy();
			chunkReferences.RemoveAt(i);
			chunkPosition.RemoveAt(i);
		}
	}
}

AChunk* AWorldGenerator::getCurrentChunk()
{
	FVector currentChunkCoordinates = FVector(currentChunkPosition.X, currentChunkPosition.Y, currentChunkPosition.Z);

	unsigned int currentChunkIndex = chunkPosition.Find(currentChunkCoordinates);
	if (currentChunkIndex != -1)
	{
		return chunkReferences[currentChunkIndex];
	}
	
	return nullptr;
}

