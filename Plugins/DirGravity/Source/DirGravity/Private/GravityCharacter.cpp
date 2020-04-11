// Copyright 29.07.2018 Tefel. All Rights Reserved.

#include "GravityCharacter.h"
#include "GravityMovementComponent.h"

AGravityCharacter::AGravityCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UGravityMovementComponent>(ACharacter::CharacterMovementComponentName))
{
}

UGravityMovementComponent* AGravityCharacter::GetGravityMovementComponent()
{
	return Cast<UGravityMovementComponent>(GetMovementComponent());
}