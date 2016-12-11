#ifndef MATERIAL_H
#define MATERIAL_H

class Material {

public:

	Material() {

	}

	Material(Vector3 kd, Vector3 ka, Vector3 ks) {
		this->kd = kd;
		this->ka = ka;
		this->ks = ks;
	}

private:

	Vector3 kd;
	Vector3 ka;
	Vector3 ks;
};

#endif