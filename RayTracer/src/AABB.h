#ifndef AABB_H
#define AABB_H

#include "GenVector.h"
#include "Primative.h"
#include <algorithm>
#include <climits>

class AABB : public Primative {

public:

	AABB() {

	}

	AABB(Vector3 min, Vector3 max, int id) {
		this->min = min;
		this->max = max;
		this->id = id;
	}

	virtual bool intersect(Ray &ray, Hitpoint &hp) {
		const size_t vecDim = 3;
		float entrance = 0.0f;
		float exit = FLT_MAX;
		Vector3 normal = Vector3(0, 0, 0);

		for (int i = 0; i < vecDim; i++) {
			float slabA = this->min[i];
			float slabB = this->max[i];
			float invDir = 1.0f / ray.getDirection()[i];
			float origin = ray.getRoot()[i];

			float closestHit = (slabA - origin) * invDir;
			float farthestHit = (slabB - origin) * invDir;

			if (farthestHit < closestHit) {
				std::swap(closestHit, farthestHit);
			}

			bool tooClose = farthestHit < entrance;
			bool tooFar = closestHit > exit;

			if (tooClose || tooFar) {
				return false;
			}

			bool foundNewEntrance = closestHit > entrance;
			entrance = foundNewEntrance ? closestHit : entrance;

			bool foundNewExit = farthestHit < exit;
			exit = foundNewExit ? farthestHit : exit;

			if (foundNewEntrance) {
				normal = Vector3(0, 0, 0);
				normal[i] = ray.getDirection()[i] * -1;
				normal.normalize();
			}
		}

		hp = Hitpoint(entrance, this->id, normal);
		return true;
	}

	virtual Material getMat() {
		return Material();
	}

	virtual Vector3 getCenter() {
		return (this->min + this->max) / 2;
	}

	virtual Vector3 getMin() {
		return this->min;
	}

	virtual Vector3 getMax() {
		return this->max;
	}

private:

	Vector3 min;
	Vector3 max;
	int id;

};

#endif