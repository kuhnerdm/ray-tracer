#include <vector>
using namespace std;

class Scene {
	
public:

	Scene() {

	}

	Scene(objLoader &obj) {

		// camera
		if (obj.camera != NULL) {
			this->camera = Camera(
				// pos
				Vector3(obj.vertexList[obj.camera->camera_pos_index]->e[0],
					obj.vertexList[obj.camera->camera_pos_index]->e[1],
					obj.vertexList[obj.camera->camera_pos_index]->e[2]),

				// at
				Vector3(obj.vertexList[obj.camera->camera_look_point_index]->e[0],
					obj.vertexList[obj.camera->camera_look_point_index]->e[1],
					obj.vertexList[obj.camera->camera_look_point_index]->e[2]).normalize(),

				// up
				Vector3(obj.normalList[obj.camera->camera_up_norm_index]->e[0],
					obj.normalList[obj.camera->camera_up_norm_index]->e[1],
					obj.normalList[obj.camera->camera_up_norm_index]->e[2]).normalize()
			);
		}
		else {
			printf("No camera found\n");
			exit(1);
		}

		// spheres
		for (int i = 0; i < obj.sphereCount; i++) {
			this->primatives.push_back(new Sphere(
				objToGenVec(obj.vertexList[obj.sphereList[i]->pos_index]),
				objToGenVec(obj.normalList[obj.sphereList[i]->up_normal_index]).length(),
				this->primatives.size()));
		}

		// triangles
		for (int i = 0; i < obj.faceCount; i++) {
			this->primatives.push_back(new Triangle(
				objToGenVec(obj.vertexList[obj.faceList[i]->vertex_index[0]]),
				objToGenVec(obj.vertexList[obj.faceList[i]->vertex_index[1]]),
				objToGenVec(obj.vertexList[obj.faceList[i]->vertex_index[2]]),
				this->primatives.size()));
		}
	}

	Camera getCamera() {
		return this->camera;
	}

	vector<Primative*> getPrimatives() {
		return this->primatives;
	}

private:
	
	vector<Primative*> primatives;
	Camera camera;

};