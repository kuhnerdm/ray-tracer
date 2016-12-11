#ifndef CAMERA_H
#define CAMERA_H

class Camera {

public:

	Camera() {
		this->u = NULL;
		this->v = NULL;
		this->w = NULL;
		this->pos = NULL;
		this->FOV = M_PI / 2;
	}

	Camera(Vector3 pos, Vector3 at, Vector3 up) {
		this->pos = pos;
		this->w = Vector3(0.0, 0.0, 0.0) - at;
		this->u = up.cross(w);
		this->v = w.cross(u);
		this->FOV = M_PI / 2;
	}

	Vector3 getPos() {
		return pos;
	}
	
	Vector3 getU() {
		return u;
	}

	Vector3 getV() {
		return v;
	}

	Vector3 getW() {
		return w;
	}

	float getFOV() {
		return this->FOV;
	}

private:

	Vector3 u;
	Vector3 v;
	Vector3 w;
	Vector3 pos;
	float FOV;

};

#endif