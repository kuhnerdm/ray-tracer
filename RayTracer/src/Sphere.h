#ifndef SPHERE_H
#define SPHERE_H

#include "Primative.h"
#include "Material.h"

class Sphere : public Primative {

public:

	Sphere() {

	}

	Sphere(Vector3 pos, float r, int id, Material mat) {
		this->pos = pos;
		this->r = r;
		this->id = id;
		this->mat = mat;
	}

	virtual Material getMat() {
		return this->mat;
	}

	virtual bool intersect(Ray &ray, Hitpoint &hp) {
		Vector3 d = ray.getDirection();
		float a = d.dot(d);
		float b = (d * 2).dot(ray.getRoot() - this->pos);
		float c = (ray.getRoot() - this->pos).dot(ray.getRoot() - this->pos) - pow(r, 2);

		float disc = pow(b, 2) - 4 * a * c;

		if (disc < 0) {
			return false;
		}
		else if (disc == 0) {
			float t = -b / (2 * a);
			if (t > 0) {
				hp = Hitpoint(t, this->id,
					(ray.getPointAtTime(t) - this->pos).normalize());
				return true;
			}
			else {
				return false;
			}
		}
		else {
			float lowT = (-b - sqrt(disc)) / (2 * a);
			float highT = (-b + sqrt(disc)) / (2 * a);
			if (lowT < 0 && highT < 0) {
				return false;
			}
			else if (lowT > 0 && highT > 0) {
				hp = Hitpoint(__min(lowT, highT), this->id,
					(ray.getPointAtTime(__min(lowT, highT)) - this->pos).normalize());
				return true;
			}
			else {
				hp = Hitpoint(__max(lowT, highT), this->id,
					(ray.getPointAtTime(__max(lowT, highT)) - this->pos).normalize());
				return true;
			}
		}

	}

	virtual Vector3 getCenter() {
		return this->pos;
	}

	virtual Vector3 getMin() {
		return this->pos - this->r;
	}

	virtual Vector3 getMax() {
		return this->pos + this->r;
	}

private:

	Vector3 pos;
	float r;
	int id;
	Material mat;

};
#endif