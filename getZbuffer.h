#include <fstream>
#include "simplemesh.h"
#include "simpleimage.h"
#include "glut.h"
#include <math.h>
#include "opencv2\opencv.hpp"

#pragma once;

namespace pmc{

	struct NearestPoint{
		pvm::Vector3D point;
		float norm;
	};

	struct Face{
		unsigned short vertex[3];
	};

	class Mesh{
	public:
		std::vector<pvm::Vector3D> vertex_list;
		std::vector<bool> visibility_check;
		std::vector<Face> face_list;
		pvm::Vector3D center;

		bool getdepthImage;
		
		Mesh();
		Mesh(smesh::Mesh in_mesh);

		void convert2pmcMesh(smesh::Mesh this_mesh);
		smesh::Mesh  convert2smesh();

	private:
		pvm::Vector3D getCenter();
	};

	class getZbuffer{

	public:
		int kwidth;
		int kheight;
		int resize_width_ ,resize_height_;


		//test data
		Mesh mesh;
		Mesh TenboMesh;

		std::vector<std::vector<float>> distmap;


		getZbuffer(){};
		getZbuffer(Mesh in_mesh){mesh = in_mesh;};
		getZbuffer(Mesh in_mesh,int in_width,int in_height){mesh = in_mesh; kwidth = in_width; kheight = in_height;};
		getZbuffer(smesh::Mesh in_mesh);
		getZbuffer(smesh::Mesh KinectMesh,smesh::Mesh tenboMesh,int in_width,int in_height);

		void Display(void);
		void DrawScene(Mesh this_mesh);
		void Resize(int width,int height);
		void Init(void);
		void keyboard(unsigned char key, int x, int y);

		Mesh saveDepthImage2(Mesh this_mesh); //メッシュの色から可視判定

		void outputVisibilityPoint(Mesh this_mesh,std::string name ="result.txt"); //meshの見えてる部分を出力

		void showMatrix(double matrix[16]);
		
		float getNorm(pvm::Vector3D a,pvm::Vector3D b);

		void getDistanceKinect2Tenbo(Mesh mesh1,Mesh mesh2);
	};
}