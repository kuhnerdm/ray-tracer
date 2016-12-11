#ifndef SHADER_H
#define SHADER_H

class Shader {

public:

	Shader() {

	}

	Shader(Scene s) {
		this->s = s;
	}

	Vector3 shade(Ray r, Hitpoint hp) {
		if (hp.getT() < 0) {
			return Vector3(0, 0, 0);
		}
		return Vector3(abs(hp.getNorm()[0]) * 255.0, abs(hp.getNorm()[1]) * 255.0, abs(hp.getNorm()[2]) * 255.0);
	}

private:

	Scene s;

};
#endif