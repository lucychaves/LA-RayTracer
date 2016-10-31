#include "objects.h"
#include <math.h>
#include <stdio.h>

// Construye la normal a partir de la ecuación del polígono
VECTOR eq2vector(PEQUATION eq){
	VECTOR normal;
	normal.x = eq.a;
	normal.y = eq.b;
	normal.z = eq.c;

	return normal;
}

long double min(long double val1, long double val2){ // Calcula el minimo entre dos valores
	if(val1 < val2){
		return val1;
	}
	return val2;
}


char max(double a, double b, double c){ // Calcula el máximo entre tres valores

     if(a >= b && a >= c){
     	return 'a';
     }

     else if(b >= a && b >= c){
     	return 'b';
     }

     return 'c';
}

POINT mapXY(int i, int j, int xmax, int ymax, int xmin, int ymin){
	POINT point;

	point.x = (((i + 0.5) * (xmax - xmin)) / H_SIZE) + xmin;
	point.y = (((j + 0.5) * (ymax - ymin)) / V_SIZE) + ymin;
	point.z = 0.0;
	return point;
}

double pointProduct(VECTOR v1, VECTOR v2){
	double x = v1.x * v2.x;
	double y = v1.y * v2.y;
	double z = v1.z * v2.z;

	return x + y + z;
}

VECTOR cruxProduct(VECTOR v, VECTOR w){
	VECTOR result;
	result.x = (v.y * w.z) - (v.z * w.y);
	result.y = (v.z * w.x) - (v.x * w.z);
	result.z = (v.x * w.y) - (v.y * w.x);

	return result;
}

double getMagnitude(VECTOR vector){
	double magnitude = sqrt(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2));
	return magnitude;
}

double getDistance(POINT p1, POINT p2){
	VECTOR distance;
	distance.x = p2.x - p1.x;
	distance.y = p2.y - p1.y;
	distance.z = p2.z - p1.z;

	return getMagnitude(distance);
}

VECTOR getVector(POINT p1, POINT p2){
	VECTOR vector;
	vector.x = p2.x - p1.x;
	vector.y = p2.y - p1.y;
	vector.z = p2.z - p1.z;

	return vector;
}

VECTOR normalizeVector(VECTOR vector){
	double magnitude = getMagnitude(vector);
	vector.x = vector.x / magnitude;
	vector.y = vector.y / magnitude;
	vector.z = vector.z / magnitude;

	return vector;
}

VECTOR getL(POINT intersection, LIGHT light){
	VECTOR L;
	L.x = light.location.x - intersection.x;
	L.y = light.location.y - intersection.y;
	L.z = light.location.z - intersection.z;

	// Normaliza el vector
	L = normalizeVector(L);
	return L;
}

VECTOR getN(OBJECT obj, POINT intersection){
	VECTOR N;
	if(obj.id == 'S'){
		N.x = (intersection.x - obj.sphere.center.x)/obj.sphere.radius;
		N.y = (intersection.y - obj.sphere.center.y)/obj.sphere.radius;
		N.z = (intersection.z - obj.sphere.center.z)/obj.sphere.radius;
	}
	else if(obj.id == 'P'){
		N = eq2vector(obj.polygon.equation);
	}
	else if(obj.id == 'C'){
		POINT o = obj.cylinder.anchor;
		VECTOR q = obj.cylinder.axis;
		double x = intersection.x;
		double y = intersection.y;
		double z = intersection.z;

		N.x = 2*(pow(q.x, 2)-1)*(q.x*(q.x*(x-o.x)+q.y*(y-o.y)+q.z*(z-o.z))+o.x-x)+
			  2*q.x*q.y*(q.y*(q.x*(x-o.x)+q.y*(y-o.y)+q.z*(z-o.z))+o.y-y)+
			  2*q.x*q.z*(q.z*(q.x*(x-o.x)+q.y*(y-o.y)+q.z*(z-o.z))+o.z-z);

		N.y = 2*(pow(q.y, 2)-1)*(q.y*(q.x*(x-o.x)+q.y*(y-o.y)+q.z*(z-o.z))+o.y-y)+
			  2*q.x*q.y*(q.x*(q.x*(x-o.x)+q.y*(y-o.y)+q.z*(z-o.z))+o.x-x)+
			  2*q.y*q.z*(q.z*(q.x*(x-o.x)+q.y*(y-o.y)+q.z*(z-o.z))+o.z-z);

		N.z = 2*(pow(q.z, 2)-1)*(q.z*(q.x*(x-o.x)+q.y*(y-o.y)+q.z*(z-o.z))+o.z-z)+
			  2*q.x*q.z*(q.x*(q.x*(x-o.x)+q.y*(y-o.y)+q.z*(z-o.z))+o.x-x)+
			  2*q.y*q.z*(q.y*(q.x*(x-o.x)+q.y*(y-o.y)+q.z*(z-o.z))+o.y-y);

		N = normalizeVector(N);
	}
	else if(obj.id == 'N'){
		POINT o = obj.cone.anchor;
		VECTOR q = obj.cone.axis;
		double x = intersection.x;
		double y = intersection.y;
		double z = intersection.z;
		double xl,yl,zl,l,H,lq;
	//	double k1 = 0.5;
	//	double k2 = 0.8;


		xl = x-o.x;
		yl = y-o.y;
		zl = z-o.z;




/*

		N.x = 2*((o.x + (xl * q.x + yl * q.y + zl * q.z)*q.x)-x)*(pow(q.x,2)-1)+
					2*((o.y + (xl * q.x + yl * q.y + zl * q.z)*q.x)-y)*q.x*q.y +
					2*((o.z + (xl * q.x + yl * q.y + zl * q.z)*q.x)-z)*q.x*q.z
		-((pow(2*k2,2)*pow(q.z,2)*q.x*(q.y*yl + q.z*zl+q.x*xl))/pow(k1,2));


		N.y = 2*((o.x + (xl * q.x + yl * q.y + zl * q.z)*q.x)-x)*q.x*q.y+
					2*((o.y + (xl * q.x + yl * q.y + zl * q.z)*q.y)-y)*(pow(q.y,2)-1) +
					2*((o.z + (xl * q.x + yl * q.y + zl * q.z)*q.z)-z)*q.y*q.z
		-((pow(2*k2,2)*q.y*pow(q.z,2)*(q.y*yl + q.z*zl+q.x*xl)/pow(k1,2)));


		N.z = 2*((o.x + (xl * q.x + yl * q.y + zl * q.z)*q.x)-x)*q.z*q.x+
					2*((o.y + (xl * q.x + yl * q.y + zl * q.z)*q.y)-y)*q.y*q.z +
					2*(pow((o.z + (xl * q.x + yl * q.y + zl * q.z)*q.z)-z,((pow(q.z,2)-1))))
		-((pow(2*k2,2)*pow(q.z,3)*(q.y*yl+q.x*xl+q.z*zl)))/pow(k1,2);

//N = normalizeVector(N);

*/

//AQUI SE DESPICHA LA HOSTIA 

		l = sqrt(pow(xl,2)+pow(yl,2)+pow(zl,2));

		xl = xl/l;
		yl = yl/l;
		zl = zl/l;

		lq = (q.x*xl + q.y*yl + q.z*zl);

		H = l/lq;

		N.x = x-(o.x + H*q.x);
		N.y = y-(o.y + H*q.y);
		N.z = z-(o.z + H*q.z);

		l = sqrt(pow(N.x,2) + pow(N.y,2) + pow(N.z,2));

		N.x = N.x/l;
		N.y = N.y/l;
		N.z = N.z/l;

		N = normalizeVector(N);

}

	return N;
}


VECTOR pointToVector(POINT point){
	VECTOR vector;
	vector.x = point.x;
	vector.y = point.y;
	vector.z = point.z;
	return vector;
}

VECTOR numberByVector(VECTOR vector, double number){
	vector.x = vector.x * number;
	vector.y = vector.y * number;
	vector.z = vector.z * number;
	return vector;
}

VECTOR substractVectors(VECTOR v1, VECTOR v2){
	v1.x = v1.x - v2.x;
	v1.y = v1.y - v2.y;
	v1.z = v1.z - v2.z;
	return v1;
}

POINT getIntersectionPoint(VECTOR vectorW, VECTOR vectorD, double t){
	POINT point;

	point.x = vectorW.x + t*vectorD.x;
	point.y = vectorW.y + t*vectorD.y;
	point.z = vectorW.z + t*vectorD.z;
	return point;
}

VECTOR rotate_cone(VECTOR axis, int grados){
//Pasa de radianes a grados primero
 int g = grados;
 VECTOR axis_aux;
 double val = PI / 180.0;
 double grados_cos = cos( g*val );
 double grados_sen = sin( g*val );
 axis_aux.x = (grados_cos * axis.x) - (grados_sen * axis.y);
 axis_aux.y = (grados_sen * axis.x) + (grados_cos * axis.y);
 axis_aux.z = 0;
 return axis_aux;

}
