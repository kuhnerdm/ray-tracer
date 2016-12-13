#ifndef LIGHT_H
#define LIGHT_H

#include "Material.h"
#include "GenVector.h"

class Light {

public:

	Light() {

	}

	Light(Material mat, Vector3 pos) {
		this->mat = mat;
		this->pos = pos;
	}

	Vector3 getAmbInt() {
		return this->mat.getKA();
	}

	Vector3 getDifInt() {
		return this->mat.getKD();
	}

	Vector3 getSpecInt() {
		return this->mat.getKS();
	}

	Vector3 getPos() {
		return this->pos;
	}

private:

	Material mat;
	Vector3 pos;

};
#endif