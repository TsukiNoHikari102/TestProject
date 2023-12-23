#pragma once

#include "CoreMinimal.h"
#include "AbilityInputEnum.generated.h"

UENUM(BlueprintType)
enum class EAbilityInput : uint8
{
	None,
	Confirm,
	Cancel,
	Interact UMETA(DisplayName = "Interact"),
	Jump UMETA(DisplayName = "Jump"),
	Dash UMETA(DisplayName = "Dash"),
	Skill1 UMETA(DisplayName = "Skill1"),
	Skill2 UMETA(DisplayName = "Skill2")
};
