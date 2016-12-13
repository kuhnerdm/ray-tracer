#ifndef RAYGENERATOR_H
#define RAYGENERATOR_H

class RayGenerator {

public:

	RayGenerator() {
		this->camera = Camera();
		this->resx = -1;
		this->resy = -1;
	}

	RayGenerator(Camera &camera, int resx, int resy) {
		this->camera = camera;
		this->resx = resx;
		this->resy = resy;
	}

	Ray getRay(int i, int j) {
		float l = -((float) resx) / 2;
		float r = ((float) resx) / 2;
		float b = -((float) resy) / 2;
		float t = ((float) resy) / 2;

		float u = l + (r - l) * (i + 0.5) / resx;
		float v = b + (t - b) * (j + 0.5) / resy;

		float d = (1 / tan(camera.getFOV() / 2)) * (resx / 2);
		
		Vector3 s = camera.getPos() + u * camera.getU() + v * camera.getV() - d * camera.getW();

		Vector3 rayDir = (s - camera.getPos()).normalize();

		return Ray(camera.getPos(), rayDir);
	}

private:

	Camera camera;
	int resx;
	int resy;

};
#endif