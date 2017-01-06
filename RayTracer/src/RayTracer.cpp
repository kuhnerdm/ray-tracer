#define _USE_MATH_DEFINES //This enables math constants in Windows
#include <math.h> //Math functions and some constants
#include <stdlib.h>
#include "Buffer.h"
#include "GenVector.h"
#include "obj_parser.h"
#include "objLoader.hpp"
#include "Scene.h"
#include "Shader.h"
#include "RayGenerator.h"
#include "Camera.h"
#include "Ray.h"
#include "Hitpoint.h"
#include "simplePNG.h"


#define RES 300

int main(int argc, char ** argv)
{
	Buffer<Vector3> buffer = Buffer<Vector3>(RES, RES);


	//Need at least two arguments (obj input and png output)
	if (argc < 3)
	{
		printf("Usage %s input.obj output.png\n", argv[0]);
		exit(0);
	}

	objLoader objData = objLoader();
	objData.load(argv[1]);

	Scene scene = Scene(objData);
	Shader shader = Shader(scene);

	RayGenerator generator = RayGenerator(scene.getCamera(), RES, RES);

	double highest = -1;

	//Convert vectors to RGB colors for testing results
	for (int y = 0; y<RES; y++)
	{
		for (int x = 0; x<RES; x++)
		{
			Ray r = generator.getRay(x, y);
			Hitpoint hp = scene.intersectWithScene(r);
			buffer.at(x, RES - y - 1) = shader.shade(r, hp, 1.0);

			// Get highest number across the whole buffer for scaling
			if (buffer.at(x, RES-1 - y)[0] > highest) {
				highest = buffer.at(x, RES - y - 1)[0];
			}
			if (buffer.at(x, RES-1 - y)[1] > highest) {
				highest = buffer.at(x, RES - y - 1)[1];
			}
			if (buffer.at(x, RES-1 - y)[2] > highest) {
				highest = buffer.at(x, RES - y - 1)[2];
			}
		}
	}
	// Scaled values go in cbuffer
	Buffer<Color> cbuffer = Buffer<Color>(RES, RES);

	for (int y = 0; y < RES; y++) {
		for (int x = 0; x < RES; x++) {
			cbuffer.at(x, RES - y - 1) = Color(
				buffer.at(x, RES - y - 1)[0] / highest * 255,
				buffer.at(x, RES - y - 1)[1] / highest * 255,
				buffer.at(x, RES - y - 1)[2] / highest * 255);
		}
	}

	//Write output buffer to file argv2
	simplePNG_write(argv[2], cbuffer.getWidth(), cbuffer.getHeight(), (unsigned char*)&cbuffer.at(0, 0));

	return 0;
}