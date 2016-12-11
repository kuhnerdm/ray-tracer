#ifndef PRIMATIVE_H
#define PRIMATIVE_H

#include "Ray.h"
#include "Hitpoint.h"
#include "Material.h"

class Primative {

public:

	virtual bool intersect(Ray &ray, Hitpoint &hp) = 0;
	virtual Material getMat() = 0;

private:



};
#endif