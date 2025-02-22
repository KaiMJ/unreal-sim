#include "BoneSensor.h"
// #include "Runtime/Engine/Public/BoneIndices.h"
#include "Runtime/Engine/Classes/Engine/SkeletalMesh.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Public/AnimationRuntime.h"

FBoneSensor::FBoneSensor(USkeletalMeshComponent* InSkeletalMeshComponent)
{
	this->Component = InSkeletalMeshComponent;
}

void FBoneSensor::SetBones(const TArray<FString>& InIncludedBoneNames)
{
	IncludedBoneNames = InIncludedBoneNames;
}

TArray<FBoneInfo> FBoneSensor::GetBonesInfo()
{
	TArray<FBoneInfo> BonesInfo;

	const TArray<FBoneIndexType>& RequiredBones = Component->RequiredBones;
	const USkeletalMesh* SkeletalMesh = Component->GetSkeletalMeshAsset();
	const FTransformArrayA2& ComponentSpaceTransforms = Component->GetComponentSpaceTransforms();
	const TArray<FTransform> BoneSpaceTransforms = Component->GetBoneSpaceTransforms();
	const FTransform& ComponentToWorld = Component->GetComponentToWorld();

	bool bIncludeAll = false;
	if (IncludedBoneNames.Num() == 0 || (IncludedBoneNames.Num() == 1 && IncludedBoneNames[0].IsEmpty()))
	{
		bIncludeAll = true;
	}

	for (int32 Index = 0; Index < RequiredBones.Num(); ++Index)
	{
		int32 BoneIndex = RequiredBones[Index];
		FName BoneName = SkeletalMesh->GetRefSkeleton().GetBoneName(BoneIndex);
		if (!bIncludeAll
		&& !IncludedBoneNames.Contains(BoneName.ToString()))
		{
			continue;
		}

		// Skip if the bone is not what we need

		const FTransform BoneTM = BoneSpaceTransforms[BoneIndex];
		const FTransform ComponentTM = ComponentSpaceTransforms[BoneIndex];
		const FTransform WorldTM = ComponentTM * ComponentToWorld;

		/*
		int32 ParentIndex = SkeletalMesh->RefSkeleton.GetParentIndex(BoneIndex);

		FVector Start, End;

		End = BoneTM.GetLocation();
		if (ParentIndex >= 0)
		{
			Start = (ComponentSpaceTransforms[ParentIndex] * ComponentToWorld).GetLocation();
		}
		else // No parent??
		{
			Start = ComponentToWorld.GetLocation();
		}
		*/

		FBoneInfo BoneInfo;
		BoneInfo.BoneName = BoneName.ToString();
		BoneInfo.BoneTM = BoneTM;
		BoneInfo.ComponentTM = ComponentTM;
		BoneInfo.WorldTM = WorldTM;

		BonesInfo.Add(BoneInfo);
		// UE_LOG(LogTemp, Log, TEXT("Bone: %d, Start: %s, End: %s"), BoneIndex, *Start.ToString(), *End.ToString());
	}
	return BonesInfo;
}
