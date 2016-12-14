#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Sphere.h"
#include "Material.h"
#include "Triangle.h"
#include "Ray.h"
#include "Camera.h"
#include "Light.h"

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

class Scene {
	
public:

	Scene() {

	}

	Scene(objLoader &obj) {

		// camera
		if (obj.camera != NULL) {
			Vector3 pos = Vector3(obj.vertexList[obj.camera->camera_pos_index]->e[0],
				obj.vertexList[obj.camera->camera_pos_index]->e[1],
				obj.vertexList[obj.camera->camera_pos_index]->e[2]);
			
			Vector3 at = Vector3(obj.vertexList[obj.camera->camera_look_point_index]->e[0],
				obj.vertexList[obj.camera->camera_look_point_index]->e[1],
				obj.vertexList[obj.camera->camera_look_point_index]->e[2]);

			at = (at - pos).normalize();

			Vector3 up = Vector3(obj.normalList[obj.camera->camera_up_norm_index]->e[0],
				obj.normalList[obj.camera->camera_up_norm_index]->e[1],
				obj.normalList[obj.camera->camera_up_norm_index]->e[2]).normalize();

			this->camera = Camera(pos, at, up);
		}
		else {
			printf("No camera found\n");
			exit(1);
		}

		// spheres
		for (int i = 0; i < obj.sphereCount; i++) {
			if (obj.materialCount == 0) {
				this->primatives.push_back(new Sphere(
					objToGenVec(obj.vertexList[obj.sphereList[i]->pos_index]),
					objToGenVec(obj.normalList[obj.sphereList[i]->up_normal_index]).length(),
					this->primatives.size(),
					Material()));
			}
			else {
				this->primatives.push_back(new Sphere(
					objToGenVec(obj.vertexList[obj.sphereList[i]->pos_index]),
					objToGenVec(obj.normalList[obj.sphereList[i]->up_normal_index]).length(),
					this->primatives.size(),
					Material(
						Vector3(
							obj.materialList[obj.sphereList[i]->material_index]->diff[0],
							obj.materialList[obj.sphereList[i]->material_index]->diff[1],
							obj.materialList[obj.sphereList[i]->material_index]->diff[2]),
						Vector3(
							obj.materialList[obj.sphereList[i]->material_index]->amb[0],
							obj.materialList[obj.sphereList[i]->material_index]->amb[1],
							obj.materialList[obj.sphereList[i]->material_index]->amb[2]),
						Vector3(
							obj.materialList[obj.sphereList[i]->material_index]->spec[0],
							obj.materialList[obj.sphereList[i]->material_index]->spec[1],
							obj.materialList[obj.sphereList[i]->material_index]->spec[2]),
						obj.materialList[obj.sphereList[i]->material_index]->shiny)));
			}
		}

		// triangles
		for (int i = 0; i < obj.faceCount; i++) {
			if (obj.materialCount == 0) {
				this->primatives.push_back(new Triangle(
					objToGenVec(obj.vertexList[obj.faceList[i]->vertex_index[0]]),
					objToGenVec(obj.vertexList[obj.faceList[i]->vertex_index[1]]),
					objToGenVec(obj.vertexList[obj.faceList[i]->vertex_index[2]]),
					this->primatives.size(),
					Material()));
			}
			else {
				this->primatives.push_back(new Triangle(
					objToGenVec(obj.vertexList[obj.faceList[i]->vertex_index[0]]),
					objToGenVec(obj.vertexList[obj.faceList[i]->vertex_index[1]]),
					objToGenVec(obj.vertexList[obj.faceList[i]->vertex_index[2]]),
					this->primatives.size(),
					Material(
						Vector3(
							obj.materialList[obj.faceList[i]->material_index]->diff[0],
							obj.materialList[obj.faceList[i]->material_index]->diff[1],
							obj.materialList[obj.faceList[i]->material_index]->diff[2]),
						Vector3(
							obj.materialList[obj.faceList[i]->material_index]->amb[0],
							obj.materialList[obj.faceList[i]->material_index]->amb[1],
							obj.materialList[obj.faceList[i]->material_index]->amb[2]),
						Vector3(
							obj.materialList[obj.faceList[i]->material_index]->spec[0],
							obj.materialList[obj.faceList[i]->material_index]->spec[1],
							obj.materialList[obj.faceList[i]->material_index]->spec[2]),
						obj.materialList[obj.faceList[i]->material_index]->shiny)));
			}
		}

		// lights
		for (int i = 0; i < obj.lightPointCount; i++) {
			this->lights.push_back(new Light(
				Material(
					Vector3(
						obj.materialList[obj.lightPointList[i]->material_index]->diff[0],
						obj.materialList[obj.lightPointList[i]->material_index]->diff[1],
						obj.materialList[obj.lightPointList[i]->material_index]->diff[2]),
					Vector3(
						obj.materialList[obj.lightPointList[i]->material_index]->amb[0],
						obj.materialList[obj.lightPointList[i]->material_index]->amb[1],
						obj.materialList[obj.lightPointList[i]->material_index]->amb[2]),
					Vector3(
						obj.materialList[obj.lightPointList[i]->material_index]->spec[0],
						obj.materialList[obj.lightPointList[i]->material_index]->spec[1],
						obj.materialList[obj.lightPointList[i]->material_index]->spec[2]),
					0.0),
				objToGenVec(obj.vertexList[obj.lightPointList[i]->pos_index])));
		}
	}

	Hitpoint intersectWithScene(Ray r) {
		Vector3 d = r.getDirection()*255.0f;
		Color c = Color(0, 0, 0);
		float closestT = -1;
		Hitpoint closestHP = Hitpoint(-1.0, -1, Vector3(-1, -1, -1));
		for (int i = 0; i < this->getPrimatives().size(); i++) {
			Hitpoint hp = Hitpoint(-1.0, -1, Vector3(-1, -1, -1));
			if (this->getPrimatives().at(i)->intersect(r, hp)) {
				if (hp.getT() > 0 && (hp.getT() < closestT || closestT == -1)) {
					closestT = hp.getT();
					closestHP = hp;
				}
			}
		}
		return closestHP;
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

};
#endif