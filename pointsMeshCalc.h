#include <iostream>
#include <math.h>
#include "simplemesh.h"
#include "distancePointPlane.h"

#include "opencv2\opencv.hpp"
#include "glut.h"

#pragma once;

typedef std::vector<float> FLOATVECTOR;
typedef std::vector<pvm::Vector3D> PVM_VECTOR_VECTOR;

//メッシュと点群の差分を求めるクラス
class pointMeshCalc{

public:
	//コンストラクタ
	pointMeshCalc(){};
	//点群読み込みコンストラクタ
	pointMeshCalc(smesh::Mesh tenbo,smesh::Mesh In_kinect);

	//求めた距離
	std::vector<float> distances;
	
	//変化リスト
	smesh::Mesh distanceAllPoint;

	//距離を求める
	void getDistances();

	void GetSimpleDistances();

	//あたらしくつくったやつをテストするかんすう
	void test();



private:
	//tenboのインプット
	smesh::Mesh TenboMesh;
	
	//キネクトのインプット
	smesh::Mesh KinectMesh;

	//tenboを動かしたやつ
	smesh::Mesh ShiftTenboMesh;
	
	//ある１点とひとつのメッシュとの距離を調べるクラス
	std::vector<dpp::distancePointPlane> DPP;

	//PAを使って一番適切なメッシュを選ぶつかってないけど
	void SearchNearMesh();

	//smesh形式のフェイスが示す点を読み込む faceは３つの頂点からなるので３つの頂点を返す
	void getOneFace(smesh::Mesh mesh,int FaceNum,pvm::Vector3D *FacePoint);

	//smesh形式の頂点を読み込む 1つだけ
	void getOnePoint(smesh::Mesh mesh,int VertexNum,pvm::Vector3D *Vertex);

	//重心計算
	pvm::Vector3D getCentroid(smesh::Mesh mesh);

	void ShiftMesh();

	//読み込んだtenbo表示
	void MeshDisp();


};