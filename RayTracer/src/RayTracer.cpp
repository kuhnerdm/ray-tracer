#define _USE_MATH_DEFINES //This enables math constants in Windows
#include <math.h> //Math functions and some constants

#include <stdlib.h>
#include "GenVector.h"
#include "Ray.h"
#include "simplePNG.h"
#include "Buffer.h"
#include "objLoader.hpp"
#include "Camera.h"
#include "RayGenerator.h"
#include "Hitpoint.h"
#include "Primative.h"

//This might be helpful to convert from obj vectors to GenVectors
Vector3 objToGenVec(obj_vector const * objVec)
{
	Vector3 v;
	v[0] = objVec->e[0];
	v[1] = objVec->e[1];
	v[2] = objVec->e[2];
	return v;
}

#include "Sphere.h"
#include "Scene.h"


#define RES 100

int main(int argc, char ** argv)
{
	Buffer<Color> buffer = Buffer<Color>(RES, RES);


	//Need at least two arguments (obj input and png output)
	if (argc < 3)
	{
		printf("Usage %s input.obj output.png\n", argv[0]);
		exit(0);
	}

	//TODO: load obj from file argv1
	objLoader objData = objLoader();
	objData.load(argv[1]);

	Scene scene = Scene(objData);

	RayGenerator generator = RayGenerator(scene.getCamera(), RES, RES);

	//Convert vectors to RGB colors for testing results
	for (int y = 0; y<RES; y++)
	{
		for (int x = 0; x<RES; x++)
		{
			Ray r = generator.getRay(x, y);
			Vector3 d = r.getDirection()*255.0f;
			Color c = Color(abs(int(d[0])), abs(int(d[1])), abs(int(d[2])));
			buffer.at(x, 99 - y) = c;

			// Overload with white if it hits a sphere
			for (int i = 0; i < scene.getPrimatives().size(); i++) {
				if (scene.getPrimatives().at(i)->intersect(r, Hitpoint(-1.0, -1, Vector3(-1, -1, -1)))) {
					buffer.at(x, 99 - y) = Color(255, 255, 255);
				}
			}
		}
	}

	//Write output buffer to file argv2
	simplePNG_write(argv[2], buffer.getWidth(), buffer.getHeight(), (unsigned char*)&buffer.at(0, 0));

	return 0;
}
