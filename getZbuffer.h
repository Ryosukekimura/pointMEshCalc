#include "simplemesh.h"
#include "simpleimage.h"
#include "glut.h"

#pragma once;

namespace pmc{

	struct Face{
		unsigned short vertex[3];
	};

	struct Mesh{
		std::vector<pvm::Vector3D> vertex_list;
		std::vector<Face> face_list;
		pvm::Vector3D center;
	};

	class getZbuffer{

	public:
		int kwidth;
		int kheight;
		int resize_width_ ,resize_height_;

		Mesh mesh;

		getZbuffer(){};
		getZbuffer(Mesh in_mesh){mesh = in_mesh;};
		getZbuffer(Mesh in_mesh,int in_width,int in_height){mesh = in_mesh; kwidth = in_width; kheight = in_height;};
		getZbuffer(smesh::Mesh in_mesh);
		getZbuffer(smesh::Mesh in_mesh,int in_width,int in_height);

		Mesh convert2pmcMesh(smesh::Mesh this_mesh);
		smesh::Mesh convert2smesh(Mesh this_mesh);

		void Display(void);
		void DrawScene(Mesh this_mesh);
		void Resize(int width,int height);
		void Init(void);
		void keyboard(unsigned char key, int x, int y);

		void saveDepthImage();


	};
}