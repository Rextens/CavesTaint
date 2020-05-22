// Copyright 2020 Phyronnaz

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphSchema.h"
#include "Engine/Texture.h"
#include "Engine/Texture2D.h"
#include "Engine/StaticMesh.h"
#include "Engine/EngineTypes.h"
#include "Misc/MessageDialog.h"
#include "Logging/MessageLog.h"
#include "Async/Async.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/Package.h"
#include "Misc/UObjectToken.h"
#include "Misc/App.h"
#include "Materials/MaterialInterface.h"
#include "Curves/RichCurve.h"
#include "Curves/CurveFloat.h"
#include "Curves/CurveLinearColor.h"

#include "VoxelMessages.h"
#include "VoxelRange.h"
#include "VoxelGlobals.h"
#include "VoxelContext.h"
#include "Runtime/VoxelNodeType.h"
#include "VoxelAxisDependencies.h"
#include "VoxelPinCategory.h"
#include "Compilation/VoxelCompilationEnums.h"

#include "VoxelNode.h"
#include "VoxelGraphErrorReporter.h"