#include "SceneBVH.h"
#include "DebugMacro.hpp"

#include <array>
#include <vector>
#include <iostream>
#include "Profile.h"

void SceneBVHTreeNode::UpdateBounds()
{
	TreeBounds.InvaildBounds();
	for (const auto& CurShapeInstance : BoundsShapeInstances)
	{
		TreeBounds.Expand(CurShapeInstance.ShapeBounds);
	}
}

void SceneBVHTree::BuildTree(std::vector<ShapeInstance>&& BoundsShapeInstances)
{
	PROFILE("SceneBVHBuildTree")

	if (BoundsShapeInstances.size() == 0)
	{
		return;
	}

	RootNode = TreeNodeAllocator.Allocator();
	if (RootNode)
	{
		auto TempInstances = std::move(BoundsShapeInstances);
		for (auto& Instance : TempInstances) 
		{
			if (Instance.ShapePtr->GetBounds().IsValid()) 
			{
				Instance.UpdateBounds();
				RootNode->BoundsShapeInstances.push_back(Instance);
			}
			else 
			{
				InfinityInstances.push_back(Instance);
			}
		}
		RootNode->UpdateBounds();
		RootNode->Depth = 1;

		SceneBVHState NewState;
		NewState.TotalShapeInstanceCount = RootNode->BoundsShapeInstances.size();

		RecurseSplit(RootNode, NewState);

		std::cout << "SceneBVH Total Node Count: " << NewState.TotalNodeCount << std::endl;
		std::cout << "SceneBVH Leaf Node Count: " << NewState.LeafNodeCount << std::endl;
		std::cout << "SceneBVH ShapeInstance Count: " << NewState.TotalShapeInstanceCount << std::endl;
		std::cout << "SceneBVH Mean Leaf Node ShapeInstance Count: " << static_cast<float>(NewState.TotalShapeInstanceCount) / static_cast<float>(NewState.LeafNodeCount) << std::endl;
		std::cout << "SceneBVH Max Leaf Node ShapeInstance Count: " << NewState.LeafNodeMaxShapeInstanceCount << std::endl;
		std::cout << "SceneBVH Max Leaf Node Depth: " << NewState.MaxLeafNodeDepth << std::endl;

		Nodes.reserve(NewState.TotalNodeCount);
		OrderedShapeInstances.reserve(NewState.TotalShapeInstanceCount);
		RecursiveFlatten(RootNode);
	}
}

void SceneBVHTree::RecurseSplit(SceneBVHTreeNode* SplitNode, SceneBVHState& InSceneBVHState)
{
	if (SplitNode)
	{
		InSceneBVHState.TotalNodeCount += 1;
		if (SplitNode->BoundsShapeInstances.size() == 1 || SplitNode->Depth > 32)
		{
			InSceneBVHState.AddLeafNode(SplitNode);
			return;
		}

		glm::vec3 Diagonal = SplitNode->TreeBounds.GetBoundsDiagonal();
		float MinCost = std::numeric_limits<float>::infinity();
		size_t MinSplitIndex = 0;
		Bounds LeftChildBound, RightChildBound;
		size_t LeftShapeInstanceCount = 0, RightShapeInstanceCount = 0;
		constexpr int SplitBucketCount = 12;
		std::vector<size_t> BucketShapeInstanceIndex[3][SplitBucketCount] = {};

		for (int Axis = 0; Axis < 3; Axis++)
		{
			Bounds BucketBounds[SplitBucketCount] = {};
			size_t BucketShapeInstanceCount[SplitBucketCount] = { 0 };
			size_t ShapeInstanceIndex = 0;
			for (const ShapeInstance& BoundsShapeInstance : SplitNode->BoundsShapeInstances)
			{
				size_t BucketId = glm::clamp<size_t>(
					static_cast<size_t>(glm::floor((BoundsShapeInstance.Center[Axis] - SplitNode->TreeBounds.BoundsMin[Axis]) * SplitBucketCount / Diagonal[Axis])),
					0, SplitBucketCount - 1
				);
				BucketBounds[BucketId].Expand(BoundsShapeInstance.ShapeBounds);
				BucketShapeInstanceCount[BucketId] += 1;
				BucketShapeInstanceIndex[Axis][BucketId].push_back(ShapeInstanceIndex);
				ShapeInstanceIndex += 1;
			}

			Bounds LeftBounds = BucketBounds[0];
			size_t LeftBoundsShapeInstanceCount = BucketShapeInstanceCount[0];
			for (size_t i = 1; i <= SplitBucketCount - 1; i++)
			{
				Bounds RightBounds;
				size_t RightBoundsShapeInstanceCount = 0;
				for (size_t j = SplitBucketCount - 1; j >= i; j--)
				{
					RightBounds.Expand(BucketBounds[j]);
					RightBoundsShapeInstanceCount += BucketShapeInstanceCount[j];
				}
				if (RightBoundsShapeInstanceCount == 0)
				{
					break;
				}
				if (LeftBoundsShapeInstanceCount != 0)
				{
					float Cost = LeftBounds.Area() * LeftBoundsShapeInstanceCount + RightBounds.Area() * RightBoundsShapeInstanceCount;
					if (Cost < MinCost)
					{
						MinCost = Cost;
						SplitNode->SplitAxis = Axis;
						MinSplitIndex = i;
						LeftChildBound = LeftBounds;
						RightChildBound = RightBounds;
						LeftShapeInstanceCount = LeftBoundsShapeInstanceCount;
						RightShapeInstanceCount = RightBoundsShapeInstanceCount;
					}
				}
				LeftBounds.Expand(BucketBounds[i]);
				LeftBoundsShapeInstanceCount += BucketShapeInstanceCount[i];
			}
		}

		if (MinSplitIndex == 0)
		{
			InSceneBVHState.AddLeafNode(SplitNode);
			return;
		}


		SceneBVHTreeNode* LeftChildNode = TreeNodeAllocator.Allocator();
		SceneBVHTreeNode* RightChildNode = TreeNodeAllocator.Allocator();

		SplitNode->LeftChildNode = LeftChildNode;
		SplitNode->RightChildNode = RightChildNode;

		LeftChildNode->BoundsShapeInstances.reserve(LeftShapeInstanceCount);
		RightChildNode->BoundsShapeInstances.reserve(RightShapeInstanceCount);

		for (size_t i = 0; i < MinSplitIndex; i++)
		{
			for (size_t Index : BucketShapeInstanceIndex[SplitNode->SplitAxis][i])
			{
				LeftChildNode->BoundsShapeInstances.push_back(SplitNode->BoundsShapeInstances[Index]);
			}
		}
		for (size_t i = MinSplitIndex; i < SplitBucketCount; i++)
		{
			for (size_t Index : BucketShapeInstanceIndex[SplitNode->SplitAxis][i])
			{
				RightChildNode->BoundsShapeInstances.push_back(SplitNode->BoundsShapeInstances[Index]);
			}
		}

		SplitNode->BoundsShapeInstances.clear();
		SplitNode->BoundsShapeInstances.shrink_to_fit();

		LeftChildNode->TreeBounds = LeftChildBound;
		RightChildNode->TreeBounds = RightChildBound;

		LeftChildNode->Depth = SplitNode->Depth + 1;
		RightChildNode->Depth = SplitNode->Depth + 1;

		RecurseSplit(LeftChildNode, InSceneBVHState);
		RecurseSplit(RightChildNode, InSceneBVHState);
	}
}

std::optional<HitInfo> SceneBVHTree::Intersect(const Ray& InRay, float T_Min, float T_Max) const
{
	std::optional<HitInfo> ResultInfo;
	const ShapeInstance* ResultInstance = nullptr;

	DEBUG_LINE(size_t BoundsTestCount = 0);

	glm::bvec3 RayNearDircetion = { InRay.Direction.x < 0, InRay.Direction.y < 0, InRay.Direction.z < 0 };

	glm::vec3 InverseRayDirection = 1.0f / InRay.Direction;

	
	std::array<size_t, 32 > Stack;
	auto Ptr = Stack.begin();
	size_t CurrentNodeIndex = 0;
	while (true)
	{
		auto& Node = Nodes[CurrentNodeIndex];

		DEBUG_LINE(BoundsTestCount++);

		if (!Node.TreeBounds.HasIntersection(InRay, InverseRayDirection, T_Min, T_Max))
		{
			if (Ptr == Stack.begin())
			{
				break;
			}
			CurrentNodeIndex = *(--Ptr);
			continue;
		}
		if (Node.ShapeInstanceCount == 0)
		{
			if (RayNearDircetion[Node.SplitAxis])
			{
				*(Ptr++) = CurrentNodeIndex + 1;
				CurrentNodeIndex = Node.RightchildIndex;
			}
			else
			{
				CurrentNodeIndex++;
				*(Ptr++) = Node.RightchildIndex;
			}
		}
		else
		{
			auto ShapeInstanceIterator = OrderedShapeInstances.begin() + Node.ShapeInstanceIndex;

			for (size_t i = 0; i < Node.ShapeInstanceCount; i++)
			{
				auto RayObject = InRay.ObjectSpaceFromWorldSpace(ShapeInstanceIterator->ObjectFromWorld);
				auto CurHitInfo = ShapeInstanceIterator->ShapePtr->Intersect(RayObject, T_Min, T_Max);
				DEBUG_LINE(InRay.BoundsTestCount += RayObject.BoundsTestCount);
				DEBUG_LINE(InRay.TriangleTestCount += RayObject.TriangleTestCount);
				if (CurHitInfo.has_value())
				{
					T_Max = CurHitInfo->TStep;
					ResultInfo = CurHitInfo;
					ResultInstance = &(*ShapeInstanceIterator);
				}
			++ShapeInstanceIterator;
			}
			if (Ptr == Stack.begin())
			{
				break;
			}
			CurrentNodeIndex = *(--Ptr);
		}
	}

	for (const auto& InfinityInstance : InfinityInstances)
	{
		auto RayObject = InRay.ObjectSpaceFromWorldSpace(InfinityInstance.ObjectFromWorld);
		auto CurHitInfo = InfinityInstance.ShapePtr->Intersect(RayObject, T_Min, T_Max);
		DEBUG_LINE(InRay.BoundsTestCount += RayObject.BoundsTestCount);
		DEBUG_LINE(InRay.TriangleTestCount += RayObject.TriangleTestCount);
		if (CurHitInfo) {
			T_Max = CurHitInfo->TStep;
			ResultInfo = CurHitInfo;
			ResultInstance = &InfinityInstance;
		}
	}

	if (ResultInstance)
	{
		ResultInfo->HitPos = ResultInstance->WorldFromObject * glm::vec4(ResultInfo->HitPos, 1.f);
		ResultInfo->Normal = glm::normalize(glm::vec3(glm::transpose(ResultInstance->ObjectFromWorld) * glm::vec4(ResultInfo->Normal, 0.f)));
		ResultInfo->HitMaterial = ResultInstance->InstanceMaterial;
	}

	DEBUG_LINE(InRay.BoundsTestCount += BoundsTestCount);

	return ResultInfo;
}

Bounds SceneBVHTree::GetBounds() const
{
	return Nodes[0].TreeBounds;
}

int SceneBVHTree::RecursiveFlatten(SceneBVHTreeNode* FlattenNode)
{
	SceneBVHNode NewSceneBVHNode{ FlattenNode->TreeBounds, 0, static_cast<uint16_t>(FlattenNode->BoundsShapeInstances.size()), static_cast<uint8_t>(FlattenNode->SplitAxis) };
	int Index = static_cast<int>(Nodes.size());
	Nodes.push_back(NewSceneBVHNode);
	if (NewSceneBVHNode.ShapeInstanceCount == 0)
	{
		RecursiveFlatten(FlattenNode->LeftChildNode);
		Nodes[Index].RightchildIndex = RecursiveFlatten(FlattenNode->RightChildNode);
	}
	else
	{
		Nodes[Index].ShapeInstanceIndex = static_cast<int>(OrderedShapeInstances.size());
		for (const ShapeInstance& NodeShapeInstance : FlattenNode->BoundsShapeInstances)
		{
			OrderedShapeInstances.push_back(NodeShapeInstance);
		}
	}
	return Index;
}

SceneBVHTreeNodeAllocator::SceneBVHTreeNodeAllocator()
	:BlockSize(4096)
{

}

SceneBVHTreeNodeAllocator::~SceneBVHTreeNodeAllocator()
{
	for (auto* Nodes : SceneBVHTreeNodeList)
	{
		delete[] Nodes;
	}
	SceneBVHTreeNodeList.clear();
	SceneBVHTreeNodeList.shrink_to_fit();
}

SceneBVHTreeNode* SceneBVHTreeNodeAllocator::Allocator()
{
	if (BlockSize == 4096)
	{
		SceneBVHTreeNodeList.push_back(new SceneBVHTreeNode[4096]);
		BlockSize = 0;
	}
	return &(SceneBVHTreeNodeList.back())[BlockSize++];
}
