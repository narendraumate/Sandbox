//
//  Octree.cpp
//
//
//  Created by Narendra Umate on 1/7/16.
//
//

#include "Octree.h"

const int Octree::bucketSize = 1;
const int Octree::maxDepth = 8;

Octree::Octree(const int& depth, const Bound3& boundingBox)
: m_depth(depth)
, m_boundingBox(boundingBox) {
	for (int i = 0; i < 8; ++i) {
		m_children[i] = nullptr;
	}
}

Octree::Octree(const int& depth, const Vec3& center, const Vec3 extent)
: m_depth(depth)
, m_boundingBox(center, extent) {
	for (int i = 0; i < 8; ++i) {
		m_children[i] = nullptr;
	}
}

Octree::~Octree() {
	if (subtreesArePresent()) {
		for (int i = 0; i < 8; ++i) {
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

bool Octree::insert(SpatialBound3* newSpatial) {
	// Check if spatial completely fits in the tree
	if (!newSpatial->inside(m_boundingBox)) {
		if (m_depth == 0) {
			printf("Octree not large enough.\n");
			return false;
		} else {
			return false;
		}
	}

	// TODO verify that this actually works
	// Fill here because of the depth limit
	if (m_depth >= maxDepth) {
		m_acceptedSpatials.push_back(newSpatial);
		return true;
	}

	// Fill here because of the bucket limit
	if (m_acceptedSpatials.size() < bucketSize && !subtreesArePresent()) {
		m_acceptedSpatials.push_back(newSpatial);
		return true;
	}

	// Subdivide
	if (!subtreesArePresent()) {
		Vec3 middle = m_boundingBox.getCenter();
		Vec3 extent = m_boundingBox.getExtent() / 2.0f;
		m_children[NNN] = new Octree(m_depth + 1, Vec3(middle.x - extent.x, middle.y - extent.y, middle.z - extent.z), extent);
		m_children[NNP] = new Octree(m_depth + 1, Vec3(middle.x - extent.x, middle.y - extent.y, middle.z + extent.z), extent);
		m_children[NPN] = new Octree(m_depth + 1, Vec3(middle.x - extent.x, middle.y + extent.y, middle.z - extent.z), extent);
		m_children[NPP] = new Octree(m_depth + 1, Vec3(middle.x - extent.x, middle.y + extent.y, middle.z + extent.z), extent);
		m_children[PNN] = new Octree(m_depth + 1, Vec3(middle.x + extent.x, middle.y - extent.y, middle.z - extent.z), extent);
		m_children[PNP] = new Octree(m_depth + 1, Vec3(middle.x + extent.x, middle.y - extent.y, middle.z + extent.z), extent);
		m_children[PPN] = new Octree(m_depth + 1, Vec3(middle.x + extent.x, middle.y + extent.y, middle.z - extent.z), extent);
		m_children[PPP] = new Octree(m_depth + 1, Vec3(middle.x + extent.x, middle.y + extent.y, middle.z + extent.z), extent);
	}

	// Disperse
	if (!m_children[NNN]->insert(newSpatial)
		&& !m_children[NNP]->insert(newSpatial)
		&& !m_children[NPN]->insert(newSpatial)
		&& !m_children[NPP]->insert(newSpatial)
		&& !m_children[PNN]->insert(newSpatial)
		&& !m_children[PNP]->insert(newSpatial)
		&& !m_children[PPN]->insert(newSpatial)
		&& !m_children[PPP]->insert(newSpatial)) {
		// No child accepted this but the spatial lies within main boundary.
		// So must be in multiple octants at once hence keep it in the main.
		m_acceptedSpatials.push_back(newSpatial);
	}

	return true;
}

void Octree::update() {
	// A list of spatials that this update will update
	std::list<SpatialBound3*> spatialsToUpdate;

	// A list of spatials that this update will distribute
	std::list<SpatialBound3*> spatialsToDistribute;

	// Move all previously accepted spatials into this new list
	spatialsToUpdate.splice(spatialsToUpdate.end(), m_acceptedSpatials);

	// Update all children and move their rejected spatials into this new list
	if (subtreesArePresent()) {
		m_children[NNN]->update();
		m_children[NNP]->update();
		m_children[NPN]->update();
		m_children[NPP]->update();
		m_children[PNN]->update();
		m_children[PNP]->update();
		m_children[PPN]->update();
		m_children[PPP]->update();

		// Move all spatials rejected by children into this new list
		spatialsToDistribute.splice(spatialsToDistribute.end(), m_children[NNN]->m_rejectedSpatials);
		spatialsToDistribute.splice(spatialsToDistribute.end(), m_children[NNP]->m_rejectedSpatials);
		spatialsToDistribute.splice(spatialsToDistribute.end(), m_children[NPN]->m_rejectedSpatials);
		spatialsToDistribute.splice(spatialsToDistribute.end(), m_children[NPP]->m_rejectedSpatials);
		spatialsToDistribute.splice(spatialsToDistribute.end(), m_children[PNN]->m_rejectedSpatials);
		spatialsToDistribute.splice(spatialsToDistribute.end(), m_children[PNP]->m_rejectedSpatials);
		spatialsToDistribute.splice(spatialsToDistribute.end(), m_children[PPN]->m_rejectedSpatials);
		spatialsToDistribute.splice(spatialsToDistribute.end(), m_children[PPP]->m_rejectedSpatials);

		// Cull out the empty sub trees to reduce depth of recursion
		if (subtreesAreEmpty()) {
			delete m_children[NNN];
			delete m_children[NNP];
			delete m_children[NPN];
			delete m_children[NPP];
			delete m_children[PNN];
			delete m_children[PNP];
			delete m_children[PPN];
			delete m_children[PPP];
			m_children[NNN] = nullptr;
			m_children[NNP] = nullptr;
			m_children[NPN] = nullptr;
			m_children[NPP] = nullptr;
			m_children[PNN] = nullptr;
			m_children[PNP] = nullptr;
			m_children[PPN] = nullptr;
			m_children[PPP] = nullptr;
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

bool Octree::remove(SpatialBound3* spatialBound) {
	// TODO
	return false;
}

void Octree::collectTreeSpatialBounds(std::list<SpatialBound3*>& spatialBounds) {
	if (subtreesArePresent()) {
		for (int i = 0; i < 8; ++i) {
			if (true) {
				m_children[i]->collectTreeSpatialBounds(spatialBounds);
			}
		}
	}
	collectNodeSpatialBounds(spatialBounds);
}

void Octree::collectNodeSpatialBounds(std::list<SpatialBound3*>& spatialBounds) {
	for (SpatialBound3* acceptedSpatial : m_acceptedSpatials) {
		if (true) {
			spatialBounds.push_back(acceptedSpatial);
		}
	}
}

void Octree::collectTreeSpatialBounds(std::list<SpatialBound3*>& spatialBounds, const Frustum3& frustum) {
	if (subtreesArePresent()) {
		for (int i = 0; i < 8; ++i) {
			if (testIntersect(m_children[i]->m_boundingBox, frustum)) {
				m_children[i]->collectTreeSpatialBounds(spatialBounds, frustum);
			}
		}
	}
	collectNodeSpatialBounds(spatialBounds, frustum);
}

void Octree::collectNodeSpatialBounds(std::list<SpatialBound3*>& spatialBounds, const Frustum3& frustum) {
	for (SpatialBound3* acceptedSpatial : m_acceptedSpatials) {
		if (acceptedSpatial->intersect(frustum)) {
			spatialBounds.push_back(acceptedSpatial);
		}
	}
}

void Octree::collectTreeBounds(std::list<Bound3>& bounds) {
	if (subtreesArePresent()) {
		for (int i = 0; i < 8; ++i) {
			if (true) {
				m_children[i]->collectTreeBounds(bounds);
			}
		}
	}
	collectNodeBounds(bounds);
}

void Octree::collectNodeBounds(std::list<Bound3>& bounds) {
	Bound3 currentBound = getBoundingBox();
	if (true) {
		bounds.push_back(currentBound);
	}
}

void Octree::collectTreeBounds(std::list<Bound3>& bounds, const Frustum3& frustum) {
	if (subtreesArePresent()) {
		for (int i = 0; i < 8; ++i) {
			if (testIntersect(m_children[i]->m_boundingBox, frustum)) {
				m_children[i]->collectTreeBounds(bounds, frustum);
			}
		}
	}
	collectNodeBounds(bounds, frustum);
}

void Octree::collectNodeBounds(std::list<Bound3>& bounds, const Frustum3& frustum) {
	Bound3 currentBound = getBoundingBox();
	if (testIntersect(currentBound, frustum)) {
		bounds.push_back(currentBound);
	}
}

int Octree::getDepth() {
	return m_depth;
}

int Octree::getChildCount() {
	return (m_children[NNN] != nullptr) ? 8 : 0;
}

Octree* Octree::getChild(const int& index) {
	return m_children[index];
}

Bound3 Octree::getBoundingBox() {
	return m_boundingBox;
}

bool Octree::treeIsEmpty() {
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

bool Octree::subtreesAreEmpty() {
	return (m_children[NNN]->treeIsEmpty()
		&& m_children[NNP]->treeIsEmpty()
		&& m_children[NPN]->treeIsEmpty()
		&& m_children[NPP]->treeIsEmpty()
		&& m_children[PNN]->treeIsEmpty()
		&& m_children[PNP]->treeIsEmpty()
		&& m_children[PPN]->treeIsEmpty()
		&& m_children[PPP]->treeIsEmpty());
}

bool Octree::subtreesArePresent() {
	return (m_children[NNN] != nullptr);
}
