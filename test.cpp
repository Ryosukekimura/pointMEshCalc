#include "pointsMeshCalc.h"
#include "addColorFromDistance.h"

int kWindowWidth = 512;
int kWindowHeight = 424;

//int kWindowWidth = 800;
//int kWindowHeight = 800;

pmc::pointMeshCalc *pte = 0;
pmc::getZbuffer gz;

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
	//test();
	//test2();
	
	//pmc::getZbuffer temp(14,"./data1/%d-shape.obj","./data1/%d-vert.obj",kWindowWidth,kWindowHeight);
	pmc::getZbuffer temp(1217,"./max-c-k2-result-lr13-1/%d-shape.obj","./max-c-k2-result-lr13-1/%d-vert.obj",kWindowWidth,kWindowHeight);
	
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

int _main(void)
{
	pmc::Mesh test;
	pmc::getZbuffer getz(1217,"./max-c-k2-result-lr13-1/%d-shape.obj","./max-c-k2-result-lr13-1/%d-vert.obj",0,0);
	char moveName[100];

	for(int a=0;a<getz.meshList1.size();a++)
	{
		sprintf(moveName,"./mrshape/rdmove%04d-shape.ply",a);
		getz.moveMesh2Mesh(&getz.meshList2[a],&getz.meshList1[a]);
		getz.meshList1[a].writeply(moveName);
		sprintf(moveName,"./mrvert/rdmove%04d-vert.ply",a);
		getz.meshList2[a].writeply(moveName);
	}

	return 0;
}