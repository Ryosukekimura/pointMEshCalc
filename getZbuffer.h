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
		Mesh(std::string filename);
		Mesh(smesh::Mesh in_mesh);

		void convert2pmcMesh(smesh::Mesh this_mesh);
		smesh::Mesh  convert2smesh();

	private:
		pvm::Vector3D getCenter();
	};

	class getZbuffer{

	public:
		//レンダリングの解像度→Kinectのdepth画像と一緒にしてる w512 h424
		int kwidth;
		int kheight;
		int resize_width_ ,resize_height_;

		//test data
		Mesh mesh;
		Mesh TenboMesh;
		std::vector<pvm::Vector3D> distanceL;

		//calc meshes
		std::vector<Mesh> meshList1;
		std::vector<Mesh> meshList2;
		std::vector<std::vector<pvm::Vector3D>> distanceListList;

		std::vector<std::vector<float>> distmap;

		//コンストラクタ
		getZbuffer(){};
		getZbuffer(Mesh in_mesh){mesh = in_mesh;};
		getZbuffer(Mesh in_mesh,int in_width,int in_height){mesh = in_mesh; kwidth = in_width; kheight = in_height;};
		getZbuffer(smesh::Mesh in_mesh);
		getZbuffer(smesh::Mesh tenboMesh,smesh::Mesh KinectMesh,int in_width,int in_height);
		//複数用
		getZbuffer(int meshNum,std::string meshListName1,std::string meshListName2,int in_width,int in_height);

		//表示関係
		//単発表示
		void Display(void);
		//複数表示
		void Display_all(void);
		int meshCount;
		void runAnimation(void);
		//レンダリング部分
		void DrawScene(Mesh this_mesh);
		
		//gl utility
		void Resize(int width,int height);
		void Init(void);
		void keyboard(unsigned char key, int x, int y);

		//可視判定
		void saveDepthImage2(Mesh *this_mesh); //メッシュの色から可視判定
		void outputVisibilityPoint(Mesh this_mesh,std::string name ="result.txt"); //meshの見えてる部分を出力

		
		//calc distance
		void getDistanceMesh2Mesh(Mesh mesh1,Mesh mesh2, std::vector<pvm::Vector3D> *distanceList);
		void moveMesh2Mesh(Mesh *mesh1,Mesh *mesh2);
		void getDistanceTenbo2Kinect();
		void visibleCheckAndDistances(int num, Mesh mesh1, Mesh mesh2);
		void printDistance(std::vector<pvm::Vector3D> distanceList,std::string fileName = "distance.csv");
	
		//utility
		void showMatrix(double matrix[16]);
		float getNorm(pvm::Vector3D a,pvm::Vector3D b);

	};
}