//// Fill out your copyright notice in the Description page of Project Settings.

#include "Chunk.h"
#include "UserCharacter.h"

// Sets default values
AChunk::AChunk()
{
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
}

void AChunk::setBiomeColor()
{

}

void AChunk::setBiome(FastNoise *biomeNoise)
{
	int32 X = FMath::FloorToInt(GetActorLocation().X / 100.0f);
	int32 Y = FMath::FloorToInt(GetActorLocation().Y / 100.0f);
	int32 Z = FMath::FloorToInt(GetActorLocation().Z / 100.0f);

	biome = biomeNoise->GetNoise(X / 3.0f, Y / 3.0f, Z / 3.0f);

	if (biome < -0.3)
	{
		color = FVector(0.5, 0.3, 0);
	}
	else if (biome < -0.6)
	{
		color = FVector(1.0, 1.0, 0);
	}
	else if (biome < 0)
	{
		color = FVector(1.0, 0, 1.0);
	}
	else if (biome > 0.6)
	{
		color = FVector(1.0, 0.0, 0);
	}
	else if (biome > 0.3)
	{
		color = FVector(0.0, 0.0, 1.0);
	}
	else if (biome >= 0)
	{
		color = FVector(1.0, 0.5, 0.5);
	}

	dynamicInstanceOfBiomeColor = UMaterialInstanceDynamic::Create(biomeColor, NULL);
	dynamicInstanceOfBiomeColor->SetVectorParameterValue("Param", color);
	mesh->SetMaterial(0, dynamicInstanceOfBiomeColor);
}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();

	generateWalls();
	characterReference = Cast<AUserCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChunk::generateWalls()
{
	int32 X = FMath::FloorToInt(GetActorLocation().X / 100.0f);
	int32 Y = FMath::FloorToInt(GetActorLocation().Y / 100.0f);
	int32 Z = FMath::FloorToInt(GetActorLocation().Z / 100.0f);

	for (int16 x = 0; x < CHUNK_SIZE; ++x)
	{	
		for (int16 y = 0; y < CHUNK_SIZE; ++y)
		{
			for (int16 z = 0; z < CHUNK_SIZE; ++z)
			{		
				if (USimplexNoiseBPLibrary::SimplexNoise3D((x + X) * (generationRange.X), (y + Y) * (generationRange.Y), (z + Z) * generationRange.Z) < SURFACE_LEVEL)
				{
					blockTypes[x][y][z] = 1;
				}
				else
				{
					blockTypes[x][y][z] = 0;
				}
			}
		}
	}
	
	marchingCubes();
}

void AChunk::forTests_Implementation(float x, float y, float z, float value)
{
}

void AChunk::marchingCubes()
{
	int cubeIndex = 0;

	for (int x = 0; x < CHUNK_SIZE - 1; ++x)
	{
		for (int y = 0; y < CHUNK_SIZE - 1; ++y)
		{
			for (int z = 0; z < CHUNK_SIZE - 1; ++z)
			{
				float cube[8];
				setCube(x, y, z, cube);
				cubeIndex = getCubeIndex(SURFACE_LEVEL, cube);
				loadMarchingElements(x, y, z, marchingArray[cubeIndex]);
			}
		}
	}

	TArray<FColor> colors;

	TArray<FProcMeshTangent> tangents;

	mesh->CreateMeshSection(0, vertices, triangles, TArray<FVector>(), TArray<FVector2D>(), colors, tangents, true);
}

void AChunk::setCube(int& x, int& y, int& z, float cube[8])
{
	cube[0] = blockTypes[x][y][z];
	cube[1] = blockTypes[x + 1][y][z];
	cube[2] = blockTypes[x + 1][y + 1][z];
	cube[3] = blockTypes[x][y + 1][z];
	cube[4] = blockTypes[x][y][z + 1];
	cube[5] = blockTypes[x + 1][y][z + 1];
	cube[6] = blockTypes[x + 1][y + 1][z + 1];
	cube[7] = blockTypes[x][y + 1][z + 1];
}

int AChunk::getCubeIndex(float surfaceLevel, float cube[8])
{
	int cubeIndex = 0;
	for (int i = 7; i >= 0; --i)
	{
		if (cube[i] != 0)
		{
			cubeIndex = cubeIndex << 1;
			cubeIndex += 1;
		}
		else
		{
			cubeIndex = cubeIndex << 1;
		}
	}

	return cubeIndex;
}

void AChunk::drawMarchingCubeMesh(int& x, int& y, int& z, int &cubeIndex)
{
	vertices.Add(FVector(x * 100.0f, y * 100.0f, z * 100.0f));
}

void AChunk::loadMarchingElements(int& x, int& y, int& z, int points[16])
{
	for (int i = 0; i < 16; ++i)
	{
		if (points[i] == 0)
		{
			findOrCreateVertice(50, 0, 0, x, y, z);
		}
		else if (points[i] == 1)
		{
			findOrCreateVertice(100, 50, 0, x, y, z);
		}
		else if (points[i] == 2)
		{
			findOrCreateVertice(50, 100, 0, x, y, z);
		}
		else if (points[i] == 3)
		{
			findOrCreateVertice(0, 50, 0, x, y, z);
		}
		else if (points[i] == 4)
		{
			findOrCreateVertice(50, 0, 100, x, y, z);
		}
		else if (points[i] == 5)
		{
			findOrCreateVertice(100, 50, 100, x, y, z);
		}
		else if (points[i] == 6)
		{
			findOrCreateVertice(50, 100, 100, x, y, z);
		}
		else if (points[i] == 7)
		{
			findOrCreateVertice(0, 50, 100, x, y, z);
		}
		else if (points[i] == 8)
		{
			findOrCreateVertice(0, 0, 50, x, y, z);
		}
		else if (points[i] == 9)
		{
			findOrCreateVertice(100, 0, 50, x, y, z);
		}
		else if (points[i] == 10)
		{
			findOrCreateVertice(100, 100, 50, x, y, z);
		}
		else if (points[i] == 11)
		{
			findOrCreateVertice(0, 100, 50, x, y, z);
		}
	}
}

void AChunk::findOrCreateVertice(int plusX, int plusY, int plusZ, int &x, int &y, int &z)
{
	int32 findIndex = vertices.Find(FVector(x * 100.0f + plusX, y * 100.0f + plusY, z * 100.0f + plusZ));
	if (findIndex != -1)
	{
		triangles.Add(findIndex);
	}
	else
	{
		vertices.Add(FVector(x * 100.0f + plusX, y * 100.0f + plusY, z * 100.0f + plusZ));
		triangles.Add(vertices.Num() - 1);
	}
}

void AChunk::updateMesh()
{
	int32 frontSide = characterReference->chunkPosition.Find(FVector(GetActorLocation().X / characterReference->chunkSize + 1, GetActorLocation().Y / characterReference->chunkSize,
		GetActorLocation().Z / characterReference->chunkSize));
	if (frontSide != INDEX_NONE)
	{
		AChunk* frontSideChunk = characterReference->chunkReferences[frontSide];
	}

	int32 backSide = characterReference->chunkPosition.Find(FVector(GetActorLocation().X / characterReference->chunkSize - 1, GetActorLocation().Y / characterReference->chunkSize,
		GetActorLocation().Z / characterReference->chunkSize));
	if (backSide != INDEX_NONE)
	{

	}

	int32 rightSide = characterReference->chunkPosition.Find(FVector(GetActorLocation().X / characterReference->chunkSize, GetActorLocation().Y / characterReference->chunkSize + 1,
		GetActorLocation().Z / characterReference->chunkSize));
	if (rightSide != INDEX_NONE)
	{

	}

	int32 leftSide = characterReference->chunkPosition.Find(FVector(GetActorLocation().X / characterReference->chunkSize, GetActorLocation().Y / characterReference->chunkSize - 1,
		GetActorLocation().Z / characterReference->chunkSize));
	if (leftSide != INDEX_NONE)
	{

	}

	int32 topSide = characterReference->chunkPosition.Find(FVector(GetActorLocation().X / characterReference->chunkSize, GetActorLocation().Y / characterReference->chunkSize,
		GetActorLocation().Z / characterReference->chunkSize + 1));
	if (topSide != INDEX_NONE)
	{

	}

	int32 bottomSide = characterReference->chunkPosition.Find(FVector(GetActorLocation().X / characterReference->chunkSize, GetActorLocation().Y / characterReference->chunkSize,
		GetActorLocation().Z / characterReference->chunkSize - 1));
	if (bottomSide != INDEX_NONE)
	{
		AChunk* frontSideChunk = characterReference->chunkReferences[frontSide];
	}
}

void AChunk::dig(FVector actorLocation, FVector hitLocation)
{
	int roundX = hitLocation.X / 100;
	int roundY = hitLocation.Y / 100;
	int roundZ = hitLocation.Z / 100;

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("The Component Being Hit is: %i, %i, %i, %i"), int(roundX * 100 - actorLocation.X) / 100, int(roundY * 100 - actorLocation.Y) / 100, int(roundZ * 100 - actorLocation.Z) / 100, blockTypes[int(roundX * 100 - actorLocation.X) / 100][int(roundY * 100 - actorLocation.Y) / 100][int(roundZ * 100 - actorLocation.Z) / 100]));

	if (int(roundX * 100 - actorLocation.X) / 100 == 0 || int(roundX * 100 - actorLocation.X) / 100 == 16)
	{
		
	}
	if (int(roundY * 100 - actorLocation.Y) / 100 == 0 || int(roundY * 100 - actorLocation.Y) / 100 == 16)
	{

	}
	if (int(roundZ * 100 - actorLocation.Z) / 100 == 0) //|| int(roundZ * 100 - actorLocation.Z) / 100 == 16)
	{
		int32 bottomSide = characterReference->chunkPosition.Find(FVector(GetActorLocation().X / characterReference->chunkSize, GetActorLocation().Y / characterReference->chunkSize,
			GetActorLocation().Z / characterReference->chunkSize - 1));
		if (bottomSide != INDEX_NONE)
		{
			AChunk* bottomSideChunk = characterReference->chunkReferences[bottomSide];

			blockTypes[int(roundX * 100 - actorLocation.X) / 100][int(roundY * 100 - actorLocation.Y) / 100][int(roundZ * 100 - actorLocation.Z) / 100] = 0;
			vertices.Empty();
			triangles.Empty();

			int cubeIndex = 0;

			for (int x = 0; x < CHUNK_SIZE - 1; ++x)
			{
				for (int y = 0; y < CHUNK_SIZE - 1; ++y)
				{
					for (int z = 0; z < CHUNK_SIZE - 1; ++z)
					{
						float cube[8];
						setCube(x, y, z, cube);
						cubeIndex = getCubeIndex(0.2f, cube);
						loadMarchingElements(x, y, z, marchingArray[cubeIndex]);
					}
				}
			}

			TArray<FColor> colors;

			TArray<FProcMeshTangent> tangents;

			mesh->ClearMeshSection(0);
			mesh->CreateMeshSection(0, vertices, triangles, TArray<FVector>(), TArray<FVector2D>(), colors, tangents, true);

			bottomSideChunk->blockTypes[int(roundX * 100 - actorLocation.X) / 100][int(roundY * 100 - actorLocation.Y) / 100][16] = 0;

			cubeIndex = 0;

			for (int x = 0; x < CHUNK_SIZE - 1; ++x)
			{
				for (int y = 0; y < CHUNK_SIZE - 1; ++y)
				{
					for (int z = 0; z < CHUNK_SIZE - 1; ++z)
					{
						float cube[8];
						bottomSideChunk->setCube(x, y, z, cube);
						cubeIndex = getCubeIndex(0.2f, cube);
						bottomSideChunk->loadMarchingElements(x, y, z, marchingArray[cubeIndex]);
					}
				}
			}

			bottomSideChunk->mesh->ClearMeshSection(0);
			bottomSideChunk->mesh->CreateMeshSection(0, bottomSideChunk->vertices, bottomSideChunk->triangles, TArray<FVector>(), TArray<FVector2D>(), 
				colors, tangents, true);
		}
	}
	if(int(roundX * 100 - actorLocation.X) / 100 != 0 && int(roundX * 100 - actorLocation.X) / 100 != 16 && 
		int(roundY * 100 - actorLocation.Y) / 100 != 0 && int(roundY * 100 - actorLocation.Y) / 100 != 16 && 
		int(roundZ * 100 - actorLocation.Z) / 100 != 0 && int(roundZ * 100 - actorLocation.Z) / 100 != 16)
	{
		blockTypes[int(roundX * 100 - actorLocation.X) / 100][int(roundY * 100 - actorLocation.Y) / 100][int(roundZ * 100 - actorLocation.Z) / 100] = 0;
		vertices.Empty();
		triangles.Empty();

		int cubeIndex = 0;

		for (int x = 0; x < CHUNK_SIZE - 1; ++x)
		{
			for (int y = 0; y < CHUNK_SIZE - 1; ++y)
			{
				for (int z = 0; z < CHUNK_SIZE - 1; ++z)
				{
					float cube[8];
					setCube(x, y, z, cube);
					cubeIndex = getCubeIndex(0.2f, cube);
					loadMarchingElements(x, y, z, marchingArray[cubeIndex]);
				}
			}
		}

		TArray<FColor> colors;

		TArray<FProcMeshTangent> tangents;

		mesh->ClearMeshSection(0);
		mesh->CreateMeshSection(0, vertices, triangles, TArray<FVector>(), TArray<FVector2D>(), colors, tangents, true);
	}
}
