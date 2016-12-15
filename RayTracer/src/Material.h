#ifndef MATERIAL_H
#define MATERIAL_H

class Material {

public:

	Material() {

	}

	Material(Vector3 kd, Vector3 ka, Vector3 ks, float ns, float refl) {
		this->kd = kd;
		this->ka = ka;
		this->ks = ks;
		this->ns = ns;
		this->refl = refl;
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

	float getRefl() {
		return this->refl;
	}

private:

	Vector3 kd;
	Vector3 ka;
	Vector3 ks;
	float ns;
	float refl;
};

#endif