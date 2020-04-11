// Fill out your copyright notice in the Description page of Project Settings.


#include "UserCharacter.h"
#include "GravityMovementComponent.h"

// Sets default values
AUserCharacter::AUserCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UGravityMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AUserCharacter::BeginPlay()
{
	Super::BeginPlay();

	equipmentWidget = CreateWidget<UUserWidget>(this->GetGameInstance(), blueprintEuqipmentReference);

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

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TestNoise: %i"), result.Get()));
}

// Called every frame
void AUserCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	checkChunk();
}

// Called to bind functionality to input
void AUserCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AUserCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUserCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AUserCharacter::turnHorizontal);
	PlayerInputComponent->BindAxis("LookUp", this, &AUserCharacter::turnVertical);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AUserCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AUserCharacter::StopJump);

	PlayerInputComponent->BindAction("OpenCloseGui", IE_Pressed, this, &AUserCharacter::openEquipment);
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

void AUserCharacter::openEquipment()
{
	if (!isGuiOpen)
	{
		equipmentWidget->AddToViewport();
	}
	if(isGuiOpen)
	{
		equipmentWidget->RemoveFromParent();
	}

	isGuiOpen = !isGuiOpen;
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

	

	chunkReferences.Add(GetWorld()->SpawnActor<AChunk>(FVector(side.X * chunkSize, side.Y * chunkSize, side.Z * chunkSize), rotation, spawnParams));
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

void AUserCharacter::removeChunk_Implementation(int32 positionInArray)
{

}

UGravityMovementComponent* AUserCharacter::GetGravityMovementComponent()
{
	return Cast<UGravityMovementComponent>(GetMovementComponent());
}