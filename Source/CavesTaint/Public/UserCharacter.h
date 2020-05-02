// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Chunk.h"
#include <map>
#include "GravityCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Equipment.h"
#include "ToolBar.h"
#include "UserCharacter.generated.h"


class UGravityMovementComponent;

UCLASS()
class CAVESTAINT_API AUserCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FVector> chunkPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector currentChunkPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector lastChunkPosition;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		int32 chunkSize = 1500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		int32 renderSize = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AChunk*> chunkReferences; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isGuiOpen = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool showHUD = true;

	UPROPERTY(EditAnywhere, Category = "Widgets")
		TSubclassOf<UEquipment> blueprintEuqipmentReference;

	UPROPERTY(EditAnywhere, Category = "Widgets")
		TSubclassOf<UToolBar> toolBarReference;

	UEquipment* equipmentWidget;
	UToolBar *toolBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		APlayerController* playerController;

public:
	// Sets default values for this character's properties
	AUserCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character")
		UGravityMovementComponent* GetGravityMovementComponent();

	FastNoise biomeNoise;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void MoveForward(float Value);

	// Handles input for moving right and left.
	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION()
		void StartJump();

	// Clears jump flag when key is released.
	UFUNCTION()
		void StopJump();

	UFUNCTION()
		void turnVertical(float value);

	UFUNCTION()
		void turnHorizontal(float value);

	UFUNCTION()
		void turnMouseWheel(float value);

	UFUNCTION()
		void openEquipment();

	UFUNCTION()
		void hideHUD();

	UFUNCTION(BlueprintCallable, Category = "Chunk")
		void checkChunk();

	UFUNCTION(BlueprintCallable)
		AChunk* getCurrentChunk();

	UFUNCTION(BlueprintCallable)
		void spawnChunk(FVector side);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Character")
		void removeChunk(int32 positionInArray);
		virtual void removeChunk_Implementation(int32 positionInArray);
};

