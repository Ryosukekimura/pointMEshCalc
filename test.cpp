#include "pointsMeshCalc.h"

int kWindowWidth = 512;
int kWindowHeight = 424;

//int kWindowWidth = 800;
//int kWindowHeight = 800;

pmc::pointMeshCalc *pte = 0;
pmc::getZbuffer gz;

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
	//smesh::Mesh tenbo;
	//smesh::Mesh kinect;

	//tenbo.readObj("C:/Users/kimura/Documents/mycode/pointMeshCalc/pointMeshCalc/0-shape.obj");
	//kinect.readObj("C:/Users/kimura/Documents/mycode/pointMeshCalc/pointMeshCalc/0-vert.obj");

	//pointMeshCalc test(tenbo,kinect);
	////test.getDistances();

	//test.test();

}

static void display(void)
{
	//gz.Display();
	gz.Display_all();
}

void resize(int w, int h)
{
	kWindowWidth = w;
	kWindowHeight = h;
	gz.Resize(w,h);
}

static void idle(void)
{
	glutPostRedisplay();
}

static void keyboard(unsigned char key, int x, int y)
{
	gz.keyboard(key,x,y);
}

static void init(void)
{
	gz.Init();
}
int main(int argc, char *argv[])
{

	//test2();
	smesh::Mesh tenbo;
	smesh::Mesh kinect;

	tenbo.readObj("C:/Users/kimura/Documents/mycode/pointMeshCalc/pointMeshCalc/0-shape.obj");
	kinect.readObj("C:/Users/kimura/Documents/mycode/pointMeshCalc/pointMeshCalc/0-vert.obj");
	kinect.writeVertex("kinect_out.txt");
	pte = new pmc::pointMeshCalc(tenbo,kinect);
	
	//pmc::getZbuffer temp(tenbo,kinect,kWindowWidth,kWindowHeight);
	pmc::getZbuffer temp(14,"./data1/%d-shape.obj","./data1/%d-vert.obj",kWindowWidth,kWindowHeight);
	gz = temp;

	system("PAUSE");

	glutInit(&argc, argv);
	glutInitWindowSize(kWindowWidth,kWindowHeight);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("test.cpp");
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	init();

	glutMainLoop();
	
	
	std::cout <<"OK!"<<std::endl;
	system("PAUSE");
	return 0;
}