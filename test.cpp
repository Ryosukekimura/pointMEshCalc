#include "pointsMeshCalc.h"

int kWindowWidth = 640;
int kWindowHeight = 480;

void test()
{
	dpp::Vector3D in_P,m0,m1,m2;

	in_P.x = 0;
	in_P.y = 0;
	in_P.z = 1;

	m0.x = 0;
	m0.y = 0;
	m0.z = 0;

	m1.x = 1;
	m1.y = 0;
	m1.z = 0;

	m2.x = 0;
	m2.y = 1;
	m2.z = 0;

	dpp::distancePointPlane dispPP(in_P,m0,m1,m2);
	
	std::cout << dispPP.getDistance() << std::endl;
}

void test2()
{
	smesh::Mesh tenbo;
	smesh::Mesh kinect;
	
	tenbo.readObj("C:/Users/kimura/Documents/mycode/pointMeshCalc/pointMeshCalc/0-shape.obj");
	kinect.readObj("C:/Users/kimura/Documents/mycode/pointMeshCalc/pointMeshCalc/0-vert.obj");

	pointMeshCalc test(tenbo,kinect);
	//test.getDistances();

	test.test();

}

int main(int argc, char *argv[])
{

	test2();
	system("PAUSE");


	return 0;
}