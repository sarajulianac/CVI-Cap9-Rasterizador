#pragma once

#include "ofMain.h"

// ============================================================================
// CVI - Capitulo 9: The Graphics Pipeline
// Demo: Rasterizador de triangulos hecho a mano
//
// En vez de usar ofDrawTriangle() (que delega todo a la GPU), esta app
// implementa el algoritmo de rasterizacion "a pie" en la CPU, pixel por
// pixel, para que se vea exactamente lo que el libro describe en 9.1-9.4:
//
//   9.1  Rasterization              -> funciones de arista + bounding box
//   9.2  Operaciones antes/despues  -> interpolacion baricentrica de color
//                                      (antes) y escritura de pixel (despues)
//   9.3  Simple Antialiasing        -> supersampling 4x por pixel (toggle 'a')
//   9.4  Culling de primitivas      -> orientacion por area con signo (toggle 'c')
// ============================================================================

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void mousePressed(int x, int y, int button);
		void mouseDragged(int x, int y, int button);
		void mouseReleased(int x, int y, int button);

		// --- Geometria del triangulo (3 vertices arrastrables) ---
		ofVec2f verts[3];
		ofColor vertColors[3];   // color por vertice, para interpolar
		int draggedVertex;       // -1 si no se esta arrastrando ninguno

		// --- Buffer de pixeles donde rasterizamos a mano ---
		ofImage canvasImg;
		void rasterize();        // recorre el bounding box y llena canvasImg

		// --- Funcion de arista (edge function), corazon del algoritmo 9.1 ---
		float edgeFunction(const ofVec2f& a, const ofVec2f& b, const ofVec2f& p);

		// --- Estado / toggles pedagogicos ---
		bool antialiasingOn;     // 9.3
		bool cullingOn;          // 9.4
		float signedArea;        // area con signo del triangulo actual
		bool wasCulled;          // resultado del test de culling este frame

		int pixelsRasterized;    // para el HUD
};
