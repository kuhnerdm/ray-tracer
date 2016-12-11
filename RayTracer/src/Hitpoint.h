#ifndef HITPOINT_H
#define HITPOINT_H

class Hitpoint {

public:

	Hitpoint() {

	}

	Hitpoint(float t, int objID, Vector3 norm) {
		this->t = t;
		this->objID = objID;
		this->norm = norm;
	}

	float getT() {
		return t;
	}

	int getObj() {
		return objID;
	}

	Vector3 getNorm() {
		return norm;
	}

private:

	float t;
	int objID;
	Vector3 norm;

};

#endif