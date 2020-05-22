#include "MyWorldGenerator.h"

TVoxelSharedRef<FVoxelWorldGeneratorInstance> UMyWorldGenerator::GetInstance()
{
	return MakeVoxelShared<FVoxelMyWorldGeneratorInstance>(*this);
}

///////////////////////////////////////////////////////////////////////////////

FVoxelMyWorldGeneratorInstance::FVoxelMyWorldGeneratorInstance(const UMyWorldGenerator& MyGenerator): NoiseHeight(MyGenerator.NoiseHeight)
{
}

void FVoxelMyWorldGeneratorInstance::Init(const FVoxelWorldGeneratorInit& InitStruct)
{
	static const FName SeedName = "MySeed";
	Noise.SetSeed(InitStruct.Seeds.Contains(SeedName) ? InitStruct.Seeds[SeedName] : 1337);
}

v_flt FVoxelMyWorldGeneratorInstance::GetValueImpl(v_flt X, v_flt Y, v_flt Z, int32 LOD, const FVoxelItemStack& Items) const
{
	const float Height = Noise.GetPerlin_3D(X, Y, Z, 0.01f);

	int returningValue = 1;

	if (Height < -0.5f)
	{
		returningValue = -1;
	}

	return Height;
}

FVoxelMaterial FVoxelMyWorldGeneratorInstance::GetMaterialImpl(v_flt X, v_flt Y, v_flt Z, int32 LOD, const FVoxelItemStack& Items) const
{
	return FVoxelMaterial::CreateFromColor(FColor::Red);
	// return FVoxelMaterial::CreateFromSingleIndex(0);
	// return FVoxelMaterial::CreateFromDoubleIndex(0, 1, 0.5f);
}

TVoxelRange<v_flt> FVoxelMyWorldGeneratorInstance::GetValueRangeImpl(const FIntBox& Bounds, int32 LOD, const FVoxelItemStack& Items) const
{
	// Return the values that GetValueImpl can return in Bounds
	// Used to skip chunks where the value does not change
	// Be careful, if wrong your world will have holes!
	// By default return infinite range to be safe
	return TVoxelRange<v_flt>::Infinite();

	// Example for the GetValueImpl above

	// Noise is between -1 and 1
	const TVoxelRange<v_flt> Height = TVoxelRange<v_flt>(-1, 1) * NoiseHeight;

	// Z can go from min to max
	TVoxelRange<v_flt> Value = TVoxelRange<v_flt>(Bounds.Min.Z, Bounds.Max.Z) - Height;

	Value /= 5;

	return Value;
}

FVector FVoxelMyWorldGeneratorInstance::GetUpVector(v_flt X, v_flt Y, v_flt Z) const
{
	// Used by spawners
	return FVector::UpVector;
}