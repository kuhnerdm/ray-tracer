#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Primative.h"
#include "GenVector.h"
#include "Material.h"
#include "Ray.h"
#include "Hitpoint.h"

class Triangle : public Primative {

public:

	Triangle() {

	}

	Triangle(Vector3 a, Vector3 b, Vector3 c, int id, Material mat) {
		this->a = a;
		this->b = b;
		this->c = c;
		this->id = id;
		this->mat = mat;
	}

	virtual bool intersect(Ray &ray, Hitpoint &hp) {
		Vector3 n = (b - a).cross(c - a).normalize();
		Vector3 e = ray.getRoot();
		Vector3 d = ray.getDirection();

		// parallel case
		if (d.dot(n) == 0) {
			return false;
		}

		float t = ((a - e).dot(n) / d.dot(n));
		Vector3 x = ray.getPointAtTime(t);
		if ((b - a).cross(x - a).dot(n) > 0 && (c - b).cross(x - b).dot(n) > 0 && (a - c).cross(x - c).dot(n) > 0) {
			hp = Hitpoint(t, this->id, n.normalize());
			return true;
		}
		
		return false;
	}

	virtual Material getMat() {
		return this->mat;
	}

	virtual Vector3 getCenter() {
		return (this->a + this->b + this->c) / 3;
	}

	virtual Vector3 getMin() {
		return Vector3(
			__min(__min(this->a[0], this->b[0]), this->c[0]),
			__min(__min(this->a[1], this->b[1]), this->c[1]),
			__min(__min(this->a[2], this->b[2]), this->c[2]));
	}

	virtual Vector3 getMax() {
		return Vector3(
			__max(__max(this->a[0], this->b[0]), this->c[0]),
			__max(__max(this->a[1], this->b[1]), this->c[1]),
			__max(__max(this->a[2], this->b[2]), this->c[2]));
	}

private:

	Vector3 a;
	Vector3 b;
	Vector3 c;
	int id;

	Material mat;

};
#endif