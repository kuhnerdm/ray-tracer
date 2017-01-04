#ifndef AABB_H
#define AABB_H

#include "GenVector.h"
#include "Primative.h"
#include <algorithm>
#include <climits>
#include <vector>

using namespace std;

class AABB : public Primative {

public:

	AABB() {

	}

	// Constructs entire tree
	AABB(vector<Primative*> primatives) {

		// Clamp all dimensions of the AABB to the min/max of the dimensions of the primatives
		this->id = -1;
		this->min = primatives[0]->getMin();
		this->max = primatives[0]->getMax();
		for (int i = 1; i < primatives.size(); i++) {
			for (int j = 0; j < 3; j++) {
				if (primatives[i]->getMin()[j] < this->min[j]) {
					this->min[j] = primatives[i]->getMin()[j];
				}
				if (primatives[i]->getMax()[j] > this->max[j]) {
					this->max[j] = primatives[i]->getMax()[j];
				}
			}
		}

		// If vector size == 1, keep a reference to the primative, make the two AABBs = NULL, and return
		if (primatives.size() == 1) {
			this->prim = primatives[0];
			this->a = NULL;
			this->b = NULL;
			return;
		}

		// Find largest dimension (0, 1, 2)
		int maxDim = 0;
		float max = this->max[0] - this->min[0];
		for (int i = 1; i < 3; i++) {
			if (this->max[i] - this->min[i] > max) {
				maxDim = i;
				max = this->max[i] - this->min[i];
			}
		}

		// Divide that dimension in half
		float midPoint = (this->min[maxDim] + this->max[maxDim]) / 2;

		// Make a vector of primatives for each half
		vector<Primative*> a = vector<Primative*>();
		vector<Primative*> b = vector<Primative*>();
		for (int i = 0; i < primatives.size(); i++) {
			if (primatives[i]->getCenter()[maxDim] < midPoint) {
				a.push_back(primatives[i]);
			}
			else {
				b.push_back(primatives[i]);
			}
		}

		// Deal with cases where all primatives end up on one side
		if (a.empty()) {
			a.push_back(b[b.size()-1]);
			b.pop_back();
		}
		if (b.empty()) {
			b.push_back(a[a.size() - 1]);
			a.pop_back();
		}

		// Construct an AABB for each half; Keep a reference to both AABBs, make the primative = NULL, and return
		this->a = new AABB(a);
		this->b = new AABB(b);
		this->prim = NULL;

	}

	// Intsersects with the AABB, not anything inside
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

	bool intersectWithTree(Ray &ray, Hitpoint &hp) {

		// Check if the ray hits the AABB. If not, then return false
		if (!this->intersect(ray, hp)) {
			return false;
		}

		// If prim is not NULL, then call intersect on prim, load result hp into current hp, and return result of prim intersect call
		if (this->prim != NULL) {
			return this->prim->intersect(ray, hp);
		}

		// Call intersect on a and b; store result hps
		Hitpoint aHP = Hitpoint(-1.0, -1, Vector3(-1, -1, -1));
		Hitpoint bHP = Hitpoint(-1.0, -1, Vector3(-1, -1, -1));
		bool hitA = this->a->intersectWithTree(ray, aHP);
		bool hitB = this->b->intersectWithTree(ray, bHP);
		
		// If both are false, then return false
		if (!(hitA || hitB)) {
			return false;
		}

		// If one is true, then load the result hp into current hp, and return true
		if (!hitA || !hitB) {
			if (hitA) {
				hp = aHP;
			}
			else {
				hp = bHP;
			}
			return true;
		}

		// If both are true, then check which hp is closer to the ray's root, load that hp into current hp, and return true
		Vector3 aPos = ray.getPointAtTime(aHP.getT());
		Vector3 bPos = ray.getPointAtTime(bHP.getT());
		if (aPos.distance(ray.getRoot()) < bPos.distance(ray.getRoot())) {
			hp = aHP;
		}
		else {
			hp = bHP;
		}
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
	Primative* prim;
	AABB* a;
	AABB* b;
};

#endif