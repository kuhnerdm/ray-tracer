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
		float ambRed = 0.0;
		float ambGreen = 0.0;
		float ambBlue = 0.0;
		float diffRed = 0.0;
		float diffGreen = 0.0;
		float diffBlue = 0.0;
		float specRed = 0.0;
		float specGreen = 0.0;
		float specBlue = 0.0;

		for (int i = 0; i < s.getLights().size(); i++) {

			// Ambient light
			ambRed += s.getPrimatives()[hp.getObj()]->getMat().getKA()[0] * s.getLights()[i]->getAmbInt()[0];
			ambGreen += s.getPrimatives()[hp.getObj()]->getMat().getKA()[1] * s.getLights()[i]->getAmbInt()[1];
			ambBlue += s.getPrimatives()[hp.getObj()]->getMat().getKA()[2] * s.getLights()[i]->getAmbInt()[2];

			Vector3 LightToObjVec = (r.getPointAtTime(hp.getT()) - s.getLights()[i]->getPos());
			float LightToObjDist = LightToObjVec.length();
			Ray LightToObj = Ray(s.getLights()[i]->getPos(), LightToObjVec.normalize());
			Hitpoint lightHP = s.intersectWithScene(LightToObj);
			if (LightToObjDist - lightHP.getT() < 0.1) { // if there's an unobstructed path from the light to the object

				// Diffuse light

				float rdRed = s.getPrimatives()[hp.getObj()]->getMat().getKD()[0];
				float rdGreen = s.getPrimatives()[hp.getObj()]->getMat().getKD()[1];
				float rdBlue = s.getPrimatives()[hp.getObj()]->getMat().getKD()[2];
				float idRed = s.getLights()[i]->getDifInt()[0];
				float idGreen = s.getLights()[i]->getDifInt()[1];
				float idBlue = s.getLights()[i]->getDifInt()[2];
				
				Vector3 uS = (Vector3(0, 0, 0) - LightToObjVec).normalize();
				Vector3 uN = lightHP.getNorm().normalize();
				Vector3 uV = Vector3(0, 0, 0) - r.getDirection().normalize();

				float diffConstant = 0.005 * LightToObjDist * LightToObjDist + 0.005 * LightToObjDist + 0.005;

				diffRed += rdRed * idRed / diffConstant * __max(uS.dot(uN), 0.0);
				diffGreen += rdGreen * idGreen / diffConstant * __max(uS.dot(uN), 0.0);
				diffBlue += rdBlue * idBlue / diffConstant * __max(uS.dot(uN), 0.0);

				// Specular Light

				float rsRed = s.getPrimatives()[hp.getObj()]->getMat().getKS()[0];
				float rsGreen = s.getPrimatives()[hp.getObj()]->getMat().getKS()[1];
				float rsBlue = s.getPrimatives()[hp.getObj()]->getMat().getKS()[2];
				float isRed = s.getLights()[i]->getSpecInt()[0];
				float isGreen = s.getLights()[i]->getSpecInt()[1];
				float isBlue = s.getLights()[i]->getSpecInt()[2];

				specRed += rsRed * isRed / diffConstant * __max(pow(uS.dot(uV), 1), 0.0);
				specGreen += rsGreen * isGreen / diffConstant * __max(pow(uS.dot(uV), 1), 0.0);
				specBlue += rsBlue * isBlue / diffConstant * __max(pow(uS.dot(uV), 1), 0.0);

			}

		}

		return Vector3(ambRed + diffRed + specRed,
			ambGreen + diffGreen + specGreen,
			ambBlue + diffGreen + specGreen);
	}

private:

	Scene s;

};
#endif