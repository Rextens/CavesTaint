// Fill out your copyright notice in the Description page of Project Settings.


#include "UserCharacter.h"
#include "GravityMovementComponent.h"

// Sets default values
AUserCharacter::AUserCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UGravityMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	biomeNoise.SetNoiseType(FastNoise::Cellular);
	biomeNoise.SetCellularDistanceFunction(FastNoise::Natural);

}

// Called when the game starts or when spawned
void AUserCharacter::BeginPlay()
{
	Super::BeginPlay();

	equipmentWidget = CreateWidget<UEquipment>(this->GetGameInstance(), blueprintEuqipmentReference);
	equipmentWidget->playerReference = this;

	toolBar = CreateWidget<UToolBar>(this->GetGameInstance(), toolBarReference);
	toolBar->playerReference = this;
	toolBar->AddToViewport();


	playerController = GetWorld()->GetFirstPlayerController();

	/*
	auto result = Async<int>(EAsyncExecution::Thread, []() {
		
		int x = 0; 

		for (int i = 0; i < 1000000; ++i)
		{
			for (int j = 0; j < 1000000; ++j)
			{
				x = i / j;
			}
		}

		return x;
	});
	*/

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TestNoise: %i"), result.Get()));
}

// Called every frame
void AUserCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	checkChunk();
	removeChunk();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TestNoise: %f"), getCurrentChunk()->biome));  
}

// Called to bind functionality to input
void AUserCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AUserCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUserCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AUserCharacter::turnHorizontal);
	PlayerInputComponent->BindAxis("LookUp", this, &AUserCharacter::turnVertical);

	PlayerInputComponent->BindAxis("ChooseItem", this, &AUserCharacter::turnMouseWheel);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AUserCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AUserCharacter::StopJump);

	PlayerInputComponent->BindAction("OpenCloseGui", IE_Pressed, this, &AUserCharacter::openEquipment);
	PlayerInputComponent->BindAction("HideShowHUD", IE_Pressed, this, &AUserCharacter::hideHUD);
}

void AUserCharacter::MoveForward(float Value)
{
	// Find out which way is "forward" and record that the player wants to move that way.

	if (!isGuiOpen)
	{
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AUserCharacter::MoveRight(float Value)
{
	// Find out which way is "right" and record that the player wants to move that way.
	if (!isGuiOpen)
	{
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AUserCharacter::StartJump()
{
	if (!isGuiOpen)
	{
		bPressedJump = true;
	}
}

void AUserCharacter::StopJump()
{
	if (!isGuiOpen)
	{
		bPressedJump = false;
	}
}

void AUserCharacter::turnVertical(float value)
{
	if (!isGuiOpen)
	{
		AUserCharacter::AddControllerPitchInput(value);
	}
}

void AUserCharacter::turnHorizontal(float value)
{
	if (!isGuiOpen)
	{
		AUserCharacter::AddControllerYawInput(value);
	}
}

void AUserCharacter::turnMouseWheel(float value)
{
	if (value != 0.0f)
	{
		if (toolBar->choosenSlot + value < 0)
		{
			toolBar->choosenSlot = 8 + value + 1;
		}
		else if (toolBar->choosenSlot + value > 8)
		{
			toolBar->choosenSlot = -1 + value;
		}
		else
		{
			toolBar->choosenSlot += value; 
		}
		toolBar->changeSlotInUse();
	}
}

void AUserCharacter::openEquipment()
{
	if (!isGuiOpen)
	{
		equipmentWidget->AddToViewport();
		playerController->bShowMouseCursor = true;
	}
	else
	{
		equipmentWidget->RemoveFromParent();
		playerController->bShowMouseCursor = false;
	}

	isGuiOpen = !isGuiOpen;
}

void AUserCharacter::hideHUD()
{
	if (showHUD)
	{
		toolBar->RemoveFromParent();
	}
	else
	{
		toolBar->AddToViewport();
	}
	showHUD = !showHUD;
}

void AUserCharacter::checkChunk()
{
	
	for (int x = -renderSize; x < renderSize; ++x)
	{
		for (int y = -renderSize; y < renderSize; ++y)
		{
			for (int z = -renderSize; z < renderSize; ++z)
			{
			//	if (x == 0 && y == 0 && z == 0)
			//	{
					FVector side = FVector(currentChunkPosition.X + x, currentChunkPosition.Y + y, currentChunkPosition.Z + z);

					if (chunkPosition.Find(side) == -1)
					{
						//addChunk(side);
						spawnChunk(side);
						chunkPosition.AddUnique(side);
					}
			//	}
			}
		}
	}
}

void AUserCharacter::spawnChunk(FVector side)
{
	FActorSpawnParameters spawnParams;
	FRotator rotation(0.0f, 0.0f, 0.0f);

	chunkReferences.Add(GetWorld()->SpawnActor<AChunk>(blueprintChunkReference, FVector(side.X * chunkSize, side.Y * chunkSize, side.Z * chunkSize), rotation, spawnParams));
	LAST_CHUNK->setBiome(&biomeNoise); 
}

void AUserCharacter::removeChunk()
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

AChunk* AUserCharacter::getCurrentChunk()
{
	FVector currentChunkCoordinates = FVector(currentChunkPosition.X, currentChunkPosition.Y, currentChunkPosition.Z);

	unsigned int currentChunkIndex = chunkPosition.Find(currentChunkCoordinates);
	if (currentChunkIndex != -1)
	{
		return chunkReferences[currentChunkIndex];
	}
	return nullptr;
}

UGravityMovementComponent* AUserCharacter::GetGravityMovementComponent()
{
	return Cast<UGravityMovementComponent>(GetMovementComponent());
}