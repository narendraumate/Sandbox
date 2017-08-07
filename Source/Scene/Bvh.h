//
//  Bvh.h
//
//
//  Created by Narendra Umate on 1/7/16.
//
//

#pragma once

#include "Bound.h"
#include "Intersect.h"
#include "SpatialBound.h"
#include "SpatialCamera.h"
#include <list>

class Bvh {
public:
	Bvh(const int& depth, const Bound3& boundingBox);
	Bvh(const int& depth, const Vec3& center, const Vec3 extent);
	virtual ~Bvh();

	bool insert(SpatialBound3* spatialBound);
	void update();
	bool remove(SpatialBound3* spatialBound);

	void collectTreeSpatialBounds(std::unordered_map<size_t, std::list<SpatialBound3*>>& spatialBounds);
	void collectNodeSpatialBounds(std::unordered_map<size_t, std::list<SpatialBound3*>>& spatialBounds);
	void collectTreeSpatialBounds(std::unordered_map<size_t, std::list<SpatialBound3*>>& spatialBounds, const Frustum3& frustum);
	void collectNodeSpatialBounds(std::unordered_map<size_t, std::list<SpatialBound3*>>& spatialBounds, const Frustum3& frustum);

	void collectTreeConfineBounds(std::unordered_map<size_t, std::list<Bound3>>& confineBounds);
	void collectNodeConfineBounds(std::unordered_map<size_t, std::list<Bound3>>& confineBounds);
	void collectTreeConfineBounds(std::unordered_map<size_t, std::list<Bound3>>& confineBounds, const Frustum3& frustum);
	void collectNodeConfineBounds(std::unordered_map<size_t, std::list<Bound3>>& confineBounds, const Frustum3& frustum);

	int getDepth();
	int getChildCount();
	Bvh* getChild(const int& index);
	Bound3 getBoundingBox();

private:
	enum BvhChildren {
		NNN,
		PPP,
	};

	bool treeIsEmpty();
	bool subtreesAreEmpty();
	bool subtreesArePresent();

	int m_depth;
	Bound3 m_boundingBox;
	Bvh* m_children[2];
	std::list<SpatialBound3*> m_acceptedSpatials;
	std::list<SpatialBound3*> m_rejectedSpatials;

	static const int bucketSize;
	static const int maxDepth;
};
