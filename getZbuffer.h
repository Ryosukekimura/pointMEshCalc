#include "MyLibs.h"

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
		
		Mesh();
		Mesh(std::string filename);
		Mesh(smesh::Mesh in_mesh);

		std::vector<pvm::Vector3D> vertex_list;
		std::vector<bool> visibility_check; //true:visible flase:not visible
		std::vector<smesh::RGBuchar> color_list;
		std::vector<Face> face_list;
		pvm::Vector3D center;

		bool getdepthImage; // true:visible check finish
		
		void convert2pmcMesh(smesh::Mesh this_mesh); //smesh -> pmcmesh
		smesh::Mesh  convert2smesh(); //pmcmesh -> smesh
		void writeobj(std::string name); // obj��������
		void writeply(std::string name);// ply ��������

	private:
		pvm::Vector3D getCenter();
	};

	class getZbuffer{

	public:
		//�����_�����O�̉𑜓x��Kinect��depth�摜�ƈꏏ�ɂ��Ă� w512 h424
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

		//�R���X�g���N�^
		getZbuffer(){};
		getZbuffer(Mesh in_mesh){mesh = in_mesh;}; 
		getZbuffer(Mesh in_mesh,int in_width,int in_height){mesh = in_mesh; kwidth = in_width; kheight = in_height;};
		getZbuffer(smesh::Mesh in_mesh);
		getZbuffer(smesh::Mesh tenboMesh,smesh::Mesh KinectMesh,int in_width,int in_height);
		//�����p
		getZbuffer(int meshNum,std::string meshListName1,std::string meshListName2,int in_width,int in_height);

		//�\���֌W
		//�P���\��
		void Display(void);
		//�����\��
		void Display_all(void);
		int meshCount;
		void runAnimation(void);
		//�����_�����O����
		void DrawScene(Mesh this_mesh);
		
		//gl utility
		void Resize(int width,int height);
		void Init(void);
		void keyboard(unsigned char key, int x, int y);

		//������
		void saveDepthImage2(Mesh *this_mesh); //���b�V���̐F���������
		void outputVisibilityPoint(Mesh this_mesh,std::string name ="result.txt"); //mesh�̌����Ă镔�����o��

		
		//calc distance
		void getDistanceMesh2Mesh(Mesh mesh1,Mesh mesh2, std::vector<pvm::Vector3D> *distanceList);
		void moveMesh2Mesh(Mesh *mesh1,Mesh *mesh2);//�d�S�ړ�
		void getDistanceTenbo2Kinect();//
		void visibleCheckAndDistances(int num, Mesh mesh1, Mesh mesh2);
		void printDistance(std::vector<pvm::Vector3D> distanceList,std::string fileName = "distance.csv");
	
		//utility
		void showMatrix(double matrix[16]);
		float getNorm(pvm::Vector3D a,pvm::Vector3D b);

	};
}