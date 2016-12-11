#ifndef RAY_H
#define RAY_H

class Ray {

public:

	Ray() {
		this->root = NULL;
		this->dir = NULL;
	}

	Ray(Vector3 root, Vector3 dir) {
		this->root = root;
		this->dir = dir;
	}

	Vector3 getPointAtTime(float t) {
		return root + (dir * t);
	}

	Vector3 getRoot() {
		return this->root;
	}

	Vector3 getDirection() {
		return this->dir;
	}

private:

	Vector3 root;
	Vector3 dir;

};
#endif