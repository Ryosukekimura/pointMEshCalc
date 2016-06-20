#include "getZbuffer.h"

pvm::Vector3D getCenter(pmc::Mesh this_mesh)
{
	pvm::Vector3D center(0,0,0);
	int size = this_mesh.vertex_list.size();
	for(int s=0;s<size;s++)
	{
		pvm::Vector3D temp = this_mesh.vertex_list[s];
		center += temp;
	}
	
	center = center / (float)size;
	return center;
}

pmc::getZbuffer::getZbuffer(smesh::Mesh in_mesh)
{
	mesh = convert2pmcMesh(in_mesh);
}

pmc::getZbuffer::getZbuffer(smesh::Mesh in_mesh,int in_width,int in_height)
{
	mesh = convert2pmcMesh(in_mesh);

	kwidth = in_width;
	kheight = in_height;
}

pmc::Mesh pmc::getZbuffer::convert2pmcMesh(smesh::Mesh this_mesh)
{
	pmc::Mesh r_mesh;
	r_mesh.vertex_list.resize(this_mesh.numOfVertex());
	r_mesh.face_list.resize(this_mesh.numOfFace());

	for(int i = 0, h = this_mesh.numOfVertex(); i < h; i++) {
		for(int j = 0; j < 3; j++) {
			r_mesh.vertex_list[i].elem[j] = this_mesh.vertexCoord(i)[j];
		}
	}

	for(int i = 0, h = this_mesh.numOfFace(); i < h; i++) {
		for(int j = 0; j < 3; j++) {
			r_mesh.face_list[i].vertex[j] = this_mesh.vertexID(i,j);
		}
	}
	r_mesh.center = getCenter(r_mesh);
	return r_mesh;
}

smesh::Mesh pmc::getZbuffer::convert2smesh(Mesh this_mesh)
{
	smesh::Mesh r_mesh;

	for(int s=0;s<this_mesh.vertex_list.size();s++)
	{
		pvm::Vector3D temp;
		temp = this_mesh.vertex_list[s];
		r_mesh.addVertex(temp);
	}

	for(int s=0;s<this_mesh.face_list.size();s++)
	{
		Face pmctemp = this_mesh.face_list[0];
		r_mesh.addFace(pmctemp.vertex[0],pmctemp.vertex[1],pmctemp.vertex[2]);
	}

	return r_mesh;
}



void pmc::getZbuffer::Display(void)
{
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0,0.0,0.0,  mesh.center.elem[0],mesh.center.elem[1],mesh.center.elem[2]  ,0.0,1.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawScene(mesh);
	//glEnd();
	//glFlush();

	glFlush();
	//　ダブルバッファ
	glutSwapBuffers();
	
}

void pmc::getZbuffer::DrawScene(Mesh this_mesh)
{
	//glutSolidTeapot(1.0);
	glVertexPointer(3, GL_FLOAT, 0, this_mesh.vertex_list[0].elem);
	glEnableClientState(GL_VERTEX_ARRAY);

	glDrawElements(GL_TRIANGLES, this_mesh.face_list.size() * 3, GL_UNSIGNED_SHORT, this_mesh.face_list[0].vertex);
}

void pmc::getZbuffer::Resize(int width,int height)
{
	glViewport(0, 0, resize_width_ = width, resize_height_ = height);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(40.0, (double)width/(double)height, 1.0, 1000.0);
}

void pmc::getZbuffer::Init(void)
{
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	//glShadeModel(GL_SMOOTH);
	//glEnable(GL_ALPHA_TEST);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_NORMALIZE);

	

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}