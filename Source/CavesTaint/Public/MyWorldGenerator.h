#pragma once

#include "CoreMinimal.h"
#include "FastNoise.h"
#include "VoxelWorldGeneratorHelpers.h"
#include <Source/VoxelGraph/Public/VoxelNodes/VoxelNoiseNodes.h>
#include "MyWorldGenerator.generated.h"

UCLASS(Blueprintable)
class UMyWorldGenerator : public UVoxelWorldGenerator
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Generator")
		float NoiseHeight = 10.f;

	//~ Begin UVoxelWorldGenerator Interface
	virtual TVoxelSharedRef<FVoxelWorldGeneratorInstance> GetInstance() override;

	//~ End UVoxelWorldGenerator Interface
};

class FVoxelMyWorldGeneratorInstance : public TVoxelWorldGeneratorInstanceHelper<FVoxelMyWorldGeneratorInstance, UMyWorldGenerator>
{
public:
	explicit FVoxelMyWorldGeneratorInstance(const UMyWorldGenerator& MyGenerator);

	//~ Begin FVoxelWorldGeneratorInstance Interface
	virtual void Init(const FVoxelWorldGeneratorInit& InitStruct) override;

	v_flt GetValueImpl(v_flt X, v_flt Y, v_flt Z, int32 LOD, const FVoxelItemStack& Items) const;
	FVoxelMaterial GetMaterialImpl(v_flt X, v_flt Y, v_flt Z, int32 LOD, const FVoxelItemStack& Items) const;

	TVoxelRange<v_flt> GetValueRangeImpl(const FIntBox& Bounds, int32 LOD, const FVoxelItemStack& Items) const;

	virtual FVector GetUpVector(v_flt X, v_flt Y, v_flt Z) const override final;
	//~ End FVoxelWorldGeneratorInstance Interface

private:
	const float NoiseHeight;
	FastNoise Noise;
};