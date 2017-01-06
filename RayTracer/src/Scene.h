#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Sphere.h"
#include "Material.h"
#include "Triangle.h"
#include "Ray.h"
#include "Camera.h"
#include "Light.h"
#include "AABB.h"

using namespace std;

//This might be helpful to convert from obj vectors to GenVectors
Vector3 objToGenVec(obj_vector const * objVec)
{
	Vector3 v;
	v[0] = objVec->e[0];
	v[1] = objVec->e[1];
	v[2] = objVec->e[2];
	return v;
}

Vector3 doubleArrToGenVec(double * arr) {
	Vector3 v;
	v[0] = arr[0];
	v[1] = arr[1];
	v[2] = arr[2];
	return v;
}

class Scene {
	
public:

	Scene() {

	}

	Scene(objLoader &obj) {

		printf("Starting scene build...\n");

		// various checks for invalid objs
		if (obj.camera == NULL) {
			printf("No camera found\n");
			exit(1);
		}

		if (obj.materialCount == 0) {
			printf("No materials found\n");
			exit(1);
		}

		// camera
		Vector3 pos = objToGenVec(obj.vertexList[obj.camera->camera_pos_index]);
		Vector3 at = objToGenVec(obj.vertexList[obj.camera->camera_look_point_index]);
		at = (at - pos).normalize();
		Vector3 up = objToGenVec(obj.normalList[obj.camera->camera_up_norm_index]);
		this->camera = Camera(pos, at, up);

		// spheres
		for (int i = 0; i < obj.sphereCount; i++) {
			this->primatives.push_back(new Sphere(
				objToGenVec(obj.vertexList[obj.sphereList[i]->pos_index]),
				objToGenVec(obj.normalList[obj.sphereList[i]->up_normal_index]).length(),
				this->primatives.size(),
				Material(
					doubleArrToGenVec(obj.materialList[obj.sphereList[i]->material_index]->diff),
					doubleArrToGenVec(obj.materialList[obj.sphereList[i]->material_index]->amb),
					doubleArrToGenVec(obj.materialList[obj.sphereList[i]->material_index]->spec),
					obj.materialList[obj.sphereList[i]->material_index]->shiny,
					obj.materialList[obj.sphereList[i]->material_index]->reflect)));
		}

		// triangles
		for (int i = 0; i < obj.faceCount; i++) {
			this->primatives.push_back(new Triangle(
				objToGenVec(obj.vertexList[obj.faceList[i]->vertex_index[0]]),
				objToGenVec(obj.vertexList[obj.faceList[i]->vertex_index[1]]),
				objToGenVec(obj.vertexList[obj.faceList[i]->vertex_index[2]]),
				this->primatives.size(),
				Material(
					doubleArrToGenVec(obj.materialList[obj.faceList[i]->material_index]->diff),
					doubleArrToGenVec(obj.materialList[obj.faceList[i]->material_index]->amb),
					doubleArrToGenVec(obj.materialList[obj.faceList[i]->material_index]->spec),
					obj.materialList[obj.faceList[i]->material_index]->shiny,
					obj.materialList[obj.faceList[i]->material_index]->reflect)));
		}

		// lights
		for (int i = 0; i < obj.lightPointCount; i++) {
			this->lights.push_back(new Light(
				Material(
					doubleArrToGenVec(obj.materialList[obj.lightPointList[i]->material_index]->diff),
					doubleArrToGenVec(obj.materialList[obj.lightPointList[i]->material_index]->amb),
					doubleArrToGenVec(obj.materialList[obj.lightPointList[i]->material_index]->spec),
					0.0,
					0.0),
				objToGenVec(obj.vertexList[obj.lightPointList[i]->pos_index])));
		}

		printf("Scene built!\n");
		printf("Constructing AABB tree...\n");
		this->tree = new AABB(this->getPrimatives());
		printf("AABB tree constructed!\n");
	}

	Hitpoint intersectWithScene(Ray r) {
		Hitpoint hp = Hitpoint(-1.0, -1, Vector3(-1, -1, -1));
		tree->intersectWithTree(r, hp);
		return hp;
	}

	Camera getCamera() {
		return this->camera;
	}

	vector<Primative*> getPrimatives() {
		return this->primatives;
	}

	vector<Light*> getLights() {
		return this->lights;
	}

private:
	
	vector<Primative*> primatives;
	vector<Light*> lights;
	Camera camera;
	AABB* tree;

};
#endif