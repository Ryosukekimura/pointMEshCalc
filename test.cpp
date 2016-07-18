#include "pointsMeshCalc.h"

int kWindowWidth = 512;
int kWindowHeight = 424;

//int kWindowWidth = 800;
//int kWindowHeight = 800;

pmc::pointMeshCalc *pte = 0;
pmc::getZbuffer gz;

void test(void)
{
	smesh::Mesh me;
	for( int a=0;a<3;a++)
	{
		smesh::RGBuchar col;
		col.rgba[0] = 255;
		col.rgba[1] = 255;
		col.rgba[2] = 255;
		col.rgba[3] = 255;

		pvm::Vector3D ver;
		ver.elem[0] = 0;
		ver.elem[1] = 0;
		ver.elem[2] = 0;
		me.addVertex(ver);
		me.addColor(col);
	}
	me.writeInPly("coltest.ply");
}

void test2(void)
{
	smesh::Mesh tenbo, kinect;
	
	tenbo.readObj("0-shape.obj");
	for(int a=0;a<tenbo.numOfVertex();a++)
	{
		smesh::RGBuchar col;
		col.rgba[0] = 255;
		col.rgba[1] = 0;
		col.rgba[2] = 0;
		col.rgba[3] = 255;

		tenbo.addColor(col);
	}

	tenbo.writeInPly("coltest.ply");
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

	test();
	test2();
	smesh::Mesh tenbo;
	smesh::Mesh kinect;

	tenbo.readObj("C:/Users/kimura/Documents/mycode/pointMeshCalc/pointMeshCalc/0-shape.obj");
	kinect.readObj("C:/Users/kimura/Documents/mycode/pointMeshCalc/pointMeshCalc/0-vert.obj");
	
	pte = new pmc::pointMeshCalc(tenbo,kinect);
	
	//pmc::getZbuffer temp(tenbo,kinect,kWindowWidth,kWindowHeight);
	pmc::getZbuffer temp(1,"./data1/%d-shape.obj","./data1/%d-vert.obj",kWindowWidth,kWindowHeight);
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
	
	return 0;
}