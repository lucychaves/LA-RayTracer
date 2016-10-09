// Autores: Lucy Chaves, Andrés Salazar
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "objects.h"
#include "operations.h"
#include "sphere.h"
#include "lights.h"

COLOR **frameBuffer;
POINT eye;
COLOR background;
long double Ia; // INTENSIDAD DEL AMBIENTE

OBJECT objects[3000];
int sizeObjects = 0;

LIGHT lights[3000];
int numLights = 0;

int xmax, ymax, xmin, ymin;

void init_buffer(){
   int i, j;
   frameBuffer = (COLOR **)malloc(H_SIZE * sizeof(COLOR*));
   for (i = 0; i < H_SIZE; i++){
        frameBuffer[i] = (COLOR *)malloc(V_SIZE * sizeof(COLOR));
       }

   for (i = 0; i < H_SIZE; i++) {
        for (j = 0; j < V_SIZE; j++)
            {
             frameBuffer[i][j].r = 0.0;
             frameBuffer[i][j].g = 0.0;
             frameBuffer[i][j].b = 0.0;
            }
       }
}

void draw_scene(){	
   static int last_x = 0;
   int i, j;
   COLOR color;

   for (i = 0; i < last_x; i++){
        for (j = 0; j < V_SIZE; j++)
            {
             glColor3f (0.0,0.0,0.0);
             glBegin (GL_POINTS);
             glVertex2i (i,j);
             glEnd();
            }
   }
   for (i = last_x; i < H_SIZE; i++){
   	for (j = 0; j < V_SIZE; j++){
          glColor3f (frameBuffer[i][j].r,frameBuffer[i][j].g,frameBuffer[i][j].b);
          glBegin(GL_POINTS);
          glVertex2i(i,j);
          glEnd();
          last_x = i;
         }
      }
      
   glFlush();
}

// Borra todo lo dibujado en la ventana
void clear(){
	int x,y;
	for(x = 0; x < H_SIZE; x++){
		for(y = 0; y < V_SIZE; y++){
			frameBuffer[x][y].r = 0.0;
			frameBuffer[x][y].g = 0.0;
			frameBuffer[x][y].b = 0.0;
		
			glColor3f (frameBuffer[x][y].r,frameBuffer[x][y].g,frameBuffer[x][y].b);
			glBegin(GL_POINTS);
			glVertex2i(x, y);
			glEnd();
		}
	}
	
	   glFlush();
}

void setBackground(double r, double g, double b){
   background.r = r;
   background.g = g;
   background.b = b;
}

void setEye(double x, double y, double z){
   eye.x = x;
   eye.y = y;
   eye.z = z;
}

void setWindow(int pxmin, int pymin, int pxmax, int pymax){
	xmin = pxmin;
	ymin = pymin;
	xmax = pxmax;
	ymax = pymax;
}

void addObject(OBJECT newObject){
	objects[sizeObjects] = newObject;
	sizeObjects++;
}

POINT getIntersectionPoint(VECTOR vectorW, VECTOR vectorD, double t){
	POINT point;

	point.x = vectorW.x + t*vectorD.x;
	point.y = vectorW.y + t*vectorD.y;
	point.z = vectorW.z + t*vectorD.z;
	return point;
}

OBJECT obj;
int intersectionFlag = 0;
POINT firstIntersection(VECTOR vectorW, VECTOR vectorD){
	INTERSECTION intersection;
	OBJECT object;
	POINT intersectionPoint;
	double tmin = 9000000;
	int i;

	for(i = 0; i < sizeObjects; i++){
		object = objects[i];
		//calcular interseccion

		if(object.id == 'S'){
			//calcular interseccion esfera
			SPHERE sphere = object.sphere;
			intersection = findIntersection_sphere(vectorD, eye, sphere.center, sphere.radius);
		}
		else if(object.id == 'C'){
			//calcular interseccion cilindro
		}
		else if(object.id == 'P'){
			//calcular interseccion poligono
		}
		else if(object.id == 'N'){
			//calcular interseccion cono
		}
		if(intersection.flag == 1 && intersection.tmin < tmin){
			tmin = intersection.tmin;
			obj = object;
			intersectionPoint = getIntersectionPoint(vectorW, vectorD, tmin);
			intersectionFlag = 1;
		}
	}
	//printf("X = %lf, Y = %lf, Z = %lf\n",intersectionPoint.x, intersectionPoint.y, intersectionPoint.z);
	return intersectionPoint;
}

COLOR getColor(VECTOR vectorW, VECTOR vectorD){
	COLOR color;
	POINT intersection;
	int k;
	
	intersection = firstIntersection(vectorW, vectorD);
	if(intersectionFlag == 0){
		//printf("BG\n");
		color = background;
	}

	else{
		/*
		long double I = 0.0; // INTENSIDAD
		for(k=0; k<numLights; k++){
			VECTOR L = getL(intersection, lights[k]);
			* CALCULAR VECTOR N *
			double pointProd = pointProduct(L, N);
			if(pointProd > 0.0){
				long double fatt = getFatt(lights[k]);
				I += getIntensity(pointProd, obj, fatt, light[k].intensity);
			}
		}
		I += Ia*obj.ka;
		I = min(1.0, I);
		*/
		color = obj.color; // * I;
		printf("C\n");	
	}
	intersectionFlag = 0;
	return color;
}



void tracer(){
	int i, j;
	POINT w; // (xw, yw, zw)
	VECTOR d; // (xd, yd, zd)
	COLOR color;
	
	
	for(i = 0; i < H_SIZE; i++){
		for(j = 0; j < V_SIZE; j++){
		
			//mapXY(int x, int y, int xmax, int ymax, int xmin, int ymin)
			w = mapXY(i, j, xmax, ymax, xmin, ymin); 
			d = normalizeVector(w);
			color = getColor(pointToVector(eye), d);
			frameBuffer[i][j] = color;
		}
	}
	
	//saveFile(frameBuffer);
}


int main(int argc, char** argv){
   init_buffer();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(H_SIZE,V_SIZE);
   glutCreateWindow("Proyecto 2: RayTracer");
   glClear(GL_COLOR_BUFFER_BIT);
   gluOrtho2D(-0.5, H_SIZE +0.5, -0.5, V_SIZE + 0.5);
   glutDisplayFunc(draw_scene);


   setBackground(0.8, 0.8, 0.8);
   setEye(50.0, 65.0, -50.0);
   setWindow(0, 0, 400, 400);

   POINT c;
   COLOR cl;

   c.x = 50.0;
   c.y = 65.0;
   c.z = 80.0;

   cl.r = 0.0;
   cl.g = 1.0;
   cl.b = 0.0;

   createSphere(100.0, c, cl);
   tracer();
   glutMainLoop();
	
}


