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

	equipmentWidget = CreateWidget<UEquipment>(this->GetGameInstance(), blueprintEuqipmentReference);
	equipmentWidget->playerReference = this;

	toolBar = CreateWidget<UToolBar>(this->GetGameInstance(), toolBarReference);
	toolBar->playerReference = this;
	toolBar->AddToViewport();


	playerController = GetWorld()->GetFirstPlayerController();
}

// Called every frame
void AUserCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
UGravityMovementComponent* AUserCharacter::GetGravityMovementComponent()
{
	return Cast<UGravityMovementComponent>(GetMovementComponent());
}
