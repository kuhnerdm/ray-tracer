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

		Vector3 amb = Vector3(0, 0, 0);
		Vector3 diff = Vector3(0, 0, 0);
		Vector3 spec = Vector3(0, 0, 0);

		for (int i = 0; i < s.getLights().size(); i++) {

			// Ambient light
			amb += (s.getPrimatives()[hp.getObj()]->getMat().getKA() * s.getLights()[i]->getAmbInt());

			Vector3 LightToObjVec = (r.getPointAtTime(hp.getT()) - s.getLights()[i]->getPos());
			Vector3 sVec = -LightToObjVec;
			float LightToObjDist = LightToObjVec.length();
			Ray LightToObj = Ray(s.getLights()[i]->getPos(), LightToObjVec.normalize());
			Hitpoint lightHP = s.intersectWithScene(LightToObj);
			if (LightToObjDist - lightHP.getT() < 0.1) { // if there's an unobstructed path from the light to the object

				// Diffuse light
				 
				Vector3 rd = s.getPrimatives()[hp.getObj()]->getMat().getKD();
				Vector3 id = s.getLights()[i]->getDifInt()[i];
				
				Vector3 uS = (-LightToObjVec).normalize();
				Vector3 uN = lightHP.getNorm().normalize();
				Vector3 uV = -(r.getDirection()).normalize();
				Vector3 uR = (-sVec + 2 * (sVec.dot(uN)) * uN).normalize();

				diff += (rd * id * __max(uS.dot(uN), 0.0));

				// Specular Light

				float ns = s.getPrimatives()[hp.getObj()]->getMat().getNS();

				Vector3 rs = s.getPrimatives()[hp.getObj()]->getMat().getKS();
				Vector3 is = s.getLights()[i]->getSpecInt();

				spec += (rs * is * pow(__max(uR.dot(uV), 0.0), ns));

			}

		}

		return amb + diff + spec;
	}

private:

	Scene s;

};
#endif