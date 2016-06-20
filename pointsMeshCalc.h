#include <iostream>
#include <math.h>
#include "simplemesh.h"
#include "distancePointPlane.h"
#include "getZbuffer.h"

#include "opencv2\opencv.hpp"
#include "glut.h"

#pragma once;

namespace pmc
{

typedef std::vector<float> FLOATVECTOR;
typedef std::vector<pvm::Vector3D> PVM_VECTOR_VECTOR;

struct pmvMesh{
	std::vector<pvm::Vector3D> vertex_list;
	std::vector<dpp::Face> face_list;
};

//���b�V���Ɠ_�Q�̍��������߂�N���X
class pointMeshCalc{

public:
	//�R���X�g���N�^
	pointMeshCalc(){};
	//�_�Q�ǂݍ��݃R���X�g���N�^
	pointMeshCalc(smesh::Mesh tenbo,smesh::Mesh In_kinect);

	//���߂�����
	std::vector<float> distances;
	
	//�ω����X�g
	smesh::Mesh distanceAllPoint;

	//���������߂�
	void getDistances();

	void GetSimpleDistances();

	//�d�S�v�Z
	pvm::Vector3D getCentroid(smesh::Mesh mesh);

	//�����炵������������e�X�g���邩�񂷂�
	void test();

	//�\���p

	


private:
	//tenbo�̃C���v�b�g
	smesh::Mesh TenboMesh;
	
	//�L�l�N�g�̃C���v�b�g
	smesh::Mesh KinectMesh;

	//tenbo�𓮂��������
	smesh::Mesh ShiftTenboMesh;
	
	//����P�_�ƂЂƂ̃��b�V���Ƃ̋����𒲂ׂ�N���X
	std::vector<dpp::distancePointPlane> DPP;

	//PA���g���Ĉ�ԓK�؂ȃ��b�V����I�Ԃ����ĂȂ�����
	void SearchNearMesh();

	//smesh�`���̃t�F�C�X�������_��ǂݍ��� face�͂R�̒��_����Ȃ�̂łR�̒��_��Ԃ�
	void getOneFace(smesh::Mesh mesh,int FaceNum,pvm::Vector3D *FacePoint);

	//smesh�`���̒��_��ǂݍ��� 1����
	void getOnePoint(smesh::Mesh mesh,int VertexNum,pvm::Vector3D *Vertex);

	
	void ShiftMesh();

	//�ǂݍ���tenbo�\��
	void MeshDisp();


};
}