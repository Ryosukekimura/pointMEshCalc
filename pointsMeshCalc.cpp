#include "pointsMeshCalc.h"



//コンストラクタ兼読み込み
pmc::pointMeshCalc::pointMeshCalc(smesh::Mesh tenbo,smesh::Mesh In_kinect)
{
	TenboMesh = tenbo;
	KinectMesh = In_kinect;	
	distances.resize(TenboMesh.numOfVertex());
	//DPP.resize(KinectMesh.numOfVertex());
}

void pmc::pointMeshCalc::getOneFace(smesh::Mesh mesh,int FaceNum,pvm::Vector3D *FacePoint)
{
	for(int a=0;a<3;a++)
	{
		const float *point;
		int p = mesh.vertexID(FaceNum,a);
		point = mesh.vertexCoord(p);

		for(int as=0;as<3;as++){
			FacePoint[a].elem[as] = point[as];
		}
	}
}

void pmc::pointMeshCalc::getOnePoint(smesh::Mesh mesh,int VertexNum,pvm::Vector3D *Vertex)
{
	const float *point;
	point = mesh.vertexCoord(VertexNum);

	for(int a=0;a<3;a++){
		Vertex->elem[a] = point[a];
	}
}

//それぞれの点の最適なメッシュとの距離を調べる(総当り)
void pmc::pointMeshCalc::getDistances()
{	
	for(int k=0;k<KinectMesh.numOfVertex();k++)
	{
		pvm::Vector3D KinectPointTemp;
		dpp::distancePointPlane DPPpool;
		float PAtemp = NULL;

		getOnePoint(KinectMesh,k,&KinectPointTemp);


		for(int s=0;s<TenboMesh.numOfFace();s++)
		{	
			dpp::distancePointPlane DPPtemp;
			pvm::Vector3D FacePointsTemp[3];

			getOneFace(TenboMesh,s,FacePointsTemp);

			//距離を測る処理
			//まず読み込み
			DPPtemp.readPointPlane(KinectPointTemp,FacePointsTemp[0],FacePointsTemp[1],FacePointsTemp[2]);
			DPPtemp.getDistance();

			if(PAtemp == NULL ) {
				PAtemp = DPPtemp.PA.getNorm();
				DPPpool = DPPtemp;
				DPPpool.RegitstFaceIndex = s;

			}else if(PAtemp > DPPtemp.PA.getNorm()){
				PAtemp = DPPtemp.PA.getNorm();
				DPPpool = DPPtemp;
				DPPpool.RegitstFaceIndex = s;
			}
		}
		DPP.push_back(DPPpool);
		std::cout <<"point: ";DPP[k].In_vertex.printVertex();
		std::cout <<"distance " <<DPP[k].abs_distance << std::endl;
		std::cout <<"porectionPoint ";
		DPP[k].ProjectionPoint.printVertex();
		std::cout <<"registered mesh :" << DPP[k].RegitstFaceIndex << std::endl;
	}

}
//重心計算
pvm::Vector3D pmc::pointMeshCalc::getCentroid(smesh::Mesh mesh)
{
	pvm::Vector3D c_temp;

	for(int a=0;a<3;a++){
		c_temp.elem[a] = 0;
	}

	int size = mesh.numOfVertex();
	for(int numver=0;numver<size;numver++)
	{
		const float *point;
		pvm::Vector3D temp;
		getOnePoint(mesh,numver,&temp);
		c_temp += temp;
	}

	for(int a=0;a<3;a++){
		c_temp.elem[a] = c_temp.elem[a] / (float)size;
	}

	return c_temp;
}

void pmc::pointMeshCalc::ShiftMesh()
{
	pvm::Vector3D kinec_cen = getCentroid(KinectMesh);
	pvm::Vector3D tenbo_cen = getCentroid(TenboMesh);
	pvm::Vector3D distancePoint = tenbo_cen - kinec_cen ;
	ShiftTenboMesh = TenboMesh;
	ShiftTenboMesh.clearVertex();
	int size = KinectMesh.numOfVertex();
	for(int s=0;s<size;s++)
	{
		pvm::Vector3D t_point,temp;
		getOnePoint(TenboMesh,s,&t_point);
		temp = t_point - distancePoint;
		ShiftTenboMesh.addVertex(temp);
	}
	

	ShiftTenboMesh.writeObj("move.obj");
}

void pmc::pointMeshCalc::GetSimpleDistances()
{
	ShiftMesh();
	int size = ShiftTenboMesh.numOfVertex();
	for(int s=0;s<size;s++)
	{
		pvm::Vector3D k_point,st_point,temp;
		getOnePoint(TenboMesh,s,&st_point);
		
		getOnePoint(KinectMesh,s,&k_point);
		temp = st_point - k_point;
		distanceAllPoint.addVertex(temp);
	}

	distanceAllPoint.writeVertex("test.txt");
}

void pmc::pointMeshCalc::test()
{
	//GetSimpleDistances();
}