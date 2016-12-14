#ifndef MATERIAL_H
#define MATERIAL_H

class Material {

public:

	Material() {

	}

	Material(Vector3 kd, Vector3 ka, Vector3 ks, float ns) {
		this->kd = kd;
		this->ka = ka;
		this->ks = ks;
		this->ns = ns;
	}

	Vector3 getKD() {
		return this->kd;
	}

	Vector3 getKA() {
		return this->ka;
	}

	Vector3 getKS() {
		return this->ks;
	}

	float getNS() {
		return this->ns;
	}

private:

	Vector3 kd;
	Vector3 ka;
	Vector3 ks;
	float ns;
};

#endif