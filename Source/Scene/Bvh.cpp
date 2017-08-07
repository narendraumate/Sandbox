//
//  Bvh.cpp
//
//
//  Created by Narendra Umate on 1/7/16.
//
//

#include "Bvh.h"

#include <cassert>

const int Bvh::bucketSize = 1; // Must be locked to 1 for correctness.
const int Bvh::maxDepth = 256;

Bvh::Bvh(const int& depth, const Bound3& boundingBox)
: m_depth(depth)
, m_boundingBox(boundingBox) {
	for (int i = 0; i < 2; ++i) {
		m_children[i] = nullptr;
	}
}

Bvh::Bvh(const int& depth, const Vec3& center, const Vec3 extent)
: m_depth(depth)
, m_boundingBox(center, extent) {
	for (int i = 0; i < 2; ++i) {
		m_children[i] = nullptr;
	}
}

Bvh::~Bvh() {
	if (subtreesArePresent()) {
		for (int i = 0; i < 2; ++i) {
			delete m_children[i];
			m_children[i] = nullptr;
		}
	}

	// Cleanup accepted spatials that still exist during abrupt termination
	for (auto spatial : m_acceptedSpatials) {
		if (spatial) {
			delete spatial;
			spatial = nullptr;
		}
	}
	m_acceptedSpatials.clear();

	// Cleanup rejected spatials that still exist during abrupt termination
	for (auto spatial : m_rejectedSpatials) {
		if (spatial) {
			delete spatial;
			spatial = nullptr;
		}
	}
	m_rejectedSpatials.clear();
}

bool Bvh::insert(SpatialBound3* spatialBound) {
	// Check if spatial completely fits in the tree
	if (!spatialBound->inside(m_boundingBox)) {
		if (m_depth == 0) {
			Vec3 inputBoundCornerMin = spatialBound->getWorldBoundingBox().getCornerMin();
			Vec3 inputBoundCornerMax = spatialBound->getWorldBoundingBox().getCornerMax();
			Vec3 outerBoundCornerMin = m_boundingBox.getCornerMin();
			Vec3 outerBoundCornerMax = m_boundingBox.getCornerMax();
			assert(inputBoundCornerMin.x > outerBoundCornerMin.x && inputBoundCornerMin.y > outerBoundCornerMin.y && inputBoundCornerMin.z > outerBoundCornerMin.z);
			assert(inputBoundCornerMax.x < outerBoundCornerMax.x && inputBoundCornerMax.y < outerBoundCornerMax.y && inputBoundCornerMax.z < outerBoundCornerMax.z);
			printf("Bvh not large enough.\n");
			return false;
		} else {
			m_boundingBox = enclose(m_boundingBox, spatialBound->getWorldBoundingBox());
		}
	}

	// TODO verify that this actually works
	// Fill here because of the depth limit
	if (m_depth >= maxDepth) {
		m_acceptedSpatials.push_back(spatialBound);
		// Expand main tree to fit all sub
		if (m_depth > 0) {
			m_boundingBox = spatialBound->getWorldBoundingBox();
		}
		return true;
	}

	// Fill here because of the bucket limit
	if (m_acceptedSpatials.size() < bucketSize && !subtreesArePresent()) {
		m_acceptedSpatials.push_back(spatialBound);
		// Expand main tree to fit all sub
		if (m_depth > 0) {
			m_boundingBox = spatialBound->getWorldBoundingBox();
		}
		return true;
	}

	// Subdivide
	if (!subtreesArePresent()) {
		if (m_acceptedSpatials.size() > 0) {
			// Remove old object from the main.
			SpatialBound3* oldSpatial = m_acceptedSpatials.front();
			m_acceptedSpatials.pop_front();

			// Read all bounding box variables.
			// Bound3 oldSpatialBoundingBox = oldSpatial->getWorldBoundingBox();
			// Bound3 newSpatialBoundingBox = newSpatial->getWorldBoundingBox();

			// Insert old object in a sub tree.
			m_children[NNN] = new Bvh(m_depth + 1, oldSpatial->getWorldBoundingBox());
			m_children[NNN]->insert(oldSpatial);

			// Insert new object in a sub tree.
			m_children[PPP] = new Bvh(m_depth + 1, spatialBound->getWorldBoundingBox());
			m_children[PPP]->insert(spatialBound);

			return true;
		} else {
			printf("This should never happen.\n");
			return false;
		}
	}

	// Disperse
	Bound3 enclosureNNN = enclose(m_children[NNN]->m_boundingBox, spatialBound->getWorldBoundingBox());
	Bound3 enclosurePPP = enclose(m_children[PPP]->m_boundingBox, spatialBound->getWorldBoundingBox());

	float enclosureExtentVolumeNNN = fabs(enclosureNNN.getExtent().product());
	float enclosureExtentVolumePPP = fabs(enclosurePPP.getExtent().product());
	if (enclosureExtentVolumeNNN < enclosureExtentVolumePPP) {
		if (!m_children[NNN]->insert(spatialBound)) {
			printf("This should never happen. NNN.\n");
		}
	} else {
		if (!m_children[PPP]->insert(spatialBound)) {
			printf("This should never happen. PPP.\n");
		}
	}

	return true;
}

void Bvh::update() {
	// A list of spatials that this update will update
	std::list<SpatialBound3*> spatialsToUpdate;

	// A list of spatials that this update will distribute
	std::list<SpatialBound3*> spatialsToDistribute;

	// Move all previously accepted spatials into this new list
	spatialsToUpdate.splice(spatialsToUpdate.end(), m_acceptedSpatials);

	// Update all children and move their rejected spatials into this new list
	if (subtreesArePresent()) {
		m_children[NNN]->update();
		m_children[PPP]->update();

		// Move all spatials rejected by children into this new list
		spatialsToDistribute.splice(spatialsToDistribute.end(), m_children[NNN]->m_rejectedSpatials);
		spatialsToDistribute.splice(spatialsToDistribute.end(), m_children[PPP]->m_rejectedSpatials);

		// Cull out the empty sub trees to reduce depth of recursion
		if (subtreesAreEmpty()) {
			delete m_children[NNN];
			delete m_children[PPP];
			m_children[NNN] = nullptr;
			m_children[PPP] = nullptr;
		} else {
			// TODO Investigate if this can be done in insert instead of update
			// Without this the trees would not shrink on any transform changes
			if (m_depth != 0) {
				m_boundingBox = enclose(m_children[NNN]->getBoundingBox(), m_children[PPP]->getBoundingBox());
			}
		}
	}

	// Run the update and distribute on the update list and classify it into accept and reject
	for (SpatialBound3* spatial : spatialsToUpdate) {
		spatial->update();
		if (!insert(spatial)) {
			if (m_depth == 0) {
				delete spatial;
				spatial = nullptr;
			} else {
				m_rejectedSpatials.push_back(spatial);
			}
		}
	}

	// Run the distribute on the distribute list and classify it into accept and reject
	for (SpatialBound3* spatial : spatialsToDistribute) {
		if (!insert(spatial)) {
			if (m_depth == 0) {
				delete spatial;
				spatial = nullptr;
			} else {
				m_rejectedSpatials.push_back(spatial);
			}
		}
	}
}

bool Bvh::remove(SpatialBound3* spatialBound) {
	// TODO
	return false;
}

void Bvh::collectTreeSpatialBounds(std::unordered_map<size_t, std::list<SpatialBound3*>>& spatialBounds) {
	if (subtreesArePresent()) {
		for (int i = 0; i < 2; ++i) {
			if (true) {
				m_children[i]->collectTreeSpatialBounds(spatialBounds);
			}
		}
	}
	collectNodeSpatialBounds(spatialBounds);
}

void Bvh::collectNodeSpatialBounds(std::unordered_map<size_t, std::list<SpatialBound3*>>& spatialBounds) {
	for (SpatialBound3* acceptedSpatial : m_acceptedSpatials) {
		if (true) {
			spatialBounds[acceptedSpatial->getSpatialBoundHash()].push_back(acceptedSpatial);
		}
	}
}

void Bvh::collectTreeSpatialBounds(std::unordered_map<size_t, std::list<SpatialBound3*>>& spatialBounds, const Frustum3& frustum) {
	if (subtreesArePresent()) {
		for (int i = 0; i < 2; ++i) {
			if (testIntersect(m_children[i]->m_boundingBox, frustum)) {
				m_children[i]->collectTreeSpatialBounds(spatialBounds, frustum);
			}
		}
	}
	collectNodeSpatialBounds(spatialBounds, frustum);
}

void Bvh::collectNodeSpatialBounds(std::unordered_map<size_t, std::list<SpatialBound3*>>& spatialBounds, const Frustum3& frustum) {
	for (SpatialBound3* acceptedSpatial : m_acceptedSpatials) {
		if (acceptedSpatial->intersect(frustum)) {
			spatialBounds[acceptedSpatial->getSpatialBoundHash()].push_back(acceptedSpatial);
		}
	}
}

void Bvh::collectTreeConfineBounds(std::unordered_map<size_t, std::list<Bound3>>& confineBounds) {
	if (subtreesArePresent()) {
		for (int i = 0; i < 2; ++i) {
			if (true) {
				m_children[i]->collectTreeConfineBounds(confineBounds);
			}
		}
	}
	collectNodeConfineBounds(confineBounds);
}

void Bvh::collectNodeConfineBounds(std::unordered_map<size_t, std::list<Bound3>>& confineBounds) {
	Bound3 currentBound = getBoundingBox();
	if (true) {
		confineBounds[m_depth].push_back(currentBound);
	}
}

void Bvh::collectTreeConfineBounds(std::unordered_map<size_t, std::list<Bound3>>& confineBounds, const Frustum3& frustum) {
	if (subtreesArePresent()) {
		for (int i = 0; i < 2; ++i) {
			if (testIntersect(m_children[i]->m_boundingBox, frustum)) {
				m_children[i]->collectTreeConfineBounds(confineBounds, frustum);
			}
		}
	}
	collectNodeConfineBounds(confineBounds, frustum);
}

void Bvh::collectNodeConfineBounds(std::unordered_map<size_t, std::list<Bound3>>& confineBounds, const Frustum3& frustum) {
	Bound3 currentBound = getBoundingBox();
	if (testIntersect(currentBound, frustum)) {
		confineBounds[m_depth].push_back(currentBound);
	}
}

int Bvh::getDepth() {
	return m_depth;
}

int Bvh::getChildCount() {
	return (m_children[NNN] != nullptr) ? 2 : 0;
}

Bvh* Bvh::getChild(const int& index) {
	return m_children[index];
}

Bound3 Bvh::getBoundingBox() {
	return m_boundingBox;
}

bool Bvh::treeIsEmpty() {
	if (m_acceptedSpatials.size() == 0 && m_rejectedSpatials.size() == 0) {
		if (!subtreesArePresent()) {
			return true;
		} else {
			return (subtreesAreEmpty());
		}
	} else {
		return false;
	}
}

bool Bvh::subtreesAreEmpty() {
	return (m_children[NNN]->treeIsEmpty()
			&& m_children[PPP]->treeIsEmpty());
}

bool Bvh::subtreesArePresent() {
	return (m_children[NNN] != nullptr);
}
