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

pmc::getZbuffer::getZbuffer(smesh::Mesh KinectMesh,smesh::Mesh tenboMesh,int in_width,int in_height)
{
	mesh = convert2pmcMesh(KinectMesh);
	TenboMesh = convert2pmcMesh(tenboMesh);

	kwidth = in_width;
	kheight = in_height;
}

pmc::Mesh pmc::getZbuffer::convert2pmcMesh(smesh::Mesh this_mesh)
{
	pmc::Mesh r_mesh;
	r_mesh.vertex_list.resize(this_mesh.numOfVertex());
	r_mesh.visibility_check.resize(this_mesh.numOfVertex());
	r_mesh.face_list.resize(this_mesh.numOfFace());
	r_mesh.getdepthImage = false;

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

	//gluLookAt(0.0,0.0,0.0,  mesh.center.elem[0],mesh.center.elem[1],mesh.center.elem[2]  ,0.0,1.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTranslatef(-mesh.center.elem[0],-mesh.center.elem[1],0);
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
	//glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, -1.0); 
	gluPerspective(40.0, (double)width/(double)height, 1.0, 1000.0);
}

void pmc::getZbuffer::Init(void)
{
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	//glShadeModel(GL_SMOOTH);
	//glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_NORMALIZE);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void pmc::getZbuffer::keyboard(unsigned char key, int x, int y)
{
	switch(key) {
	case 'd':
		saveDepthImage();
		break;

	}
}

void pmc::getZbuffer::showMatrix(double matrix[16])
{
	std::cout << matrix[0] <<" "<< matrix[4] <<" "<< matrix[8] <<" "<< matrix[12] <<std::endl;
	std::cout << matrix[1] <<" "<< matrix[5] <<" "<< matrix[9] <<" "<< matrix[13] <<std::endl;
	std::cout << matrix[2] <<" "<< matrix[6] <<" "<< matrix[10] <<" "<< matrix[14] <<std::endl;
	std::cout << matrix[3] <<" "<< matrix[7] <<" "<< matrix[11] <<" "<< matrix[15] <<std::endl;
}

void pmc::getZbuffer::saveDepthImage()
{
	std::cout << "save depth image" << std::endl;

	// デプスバッファの内容を取得する
	// ｚバッファの読み出し
	std::vector<float> z_buf(kwidth * kheight);
	distmap.resize(kwidth);
	for(int s=0;s<distmap.size();s++)
	{
		distmap[s].resize(kheight);
	}

	glReadPixels(0, 0, kwidth, kheight, GL_DEPTH_COMPONENT, GL_FLOAT, &(z_buf[0]));

	//距離画像への変換
	smImageFloat distImg(kwidth, kheight);

	double modelviewmat[16],projmat[16];
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);	// 現在のビューポートを代入
	glGetDoublev(GL_PROJECTION_MATRIX, projmat);	// 現在のプロジェクション行列を代入
	//glGetDoublev(GL_MODELVIEW_MATRIX, modelviewmat);	// 現在のモデルビュー行列を代入
	showMatrix(projmat);
	//smesh::Mesh depthmesh;

	//モデルビューとして単位行列を使う
	for(int i=0; i<16; ++i){modelviewmat[i]=0.0;}
	modelviewmat[0]=modelviewmat[5]=modelviewmat[10]=modelviewmat[15]=1.0;

	for(int cnt2=0; cnt2<kheight; ++cnt2){
		for(int cnt1=0; cnt1<kwidth; ++cnt1){
			float zval=z_buf[kwidth*cnt2+cnt1];
			double camX,camY,camZ;
			gluUnProject(cnt1,cnt2,zval,modelviewmat,projmat,viewport,&camX,&camY,&camZ);
			//std::cout << "z:" << camZ << std::endl;
			//カメラ座標系でのZが距離画像の値
			//ただし、そのままだとマイナスの値になるので、符号を逆転
			distImg(cnt1,cnt2)=-camZ;
			distmap[cnt1][cnt2] = camZ;

			pvm::Vector3D temp((float)camX + mesh.center.elem[0],(float)camY + mesh.center.elem[1],(float)camZ);
			if(temp.elem[2] > -1000) depthMesh.vertex_list.push_back(temp);

		}
	}

	visibilty();
	//smesh::Mesh tt;
	//tt = convert2smesh(depthMesh);
	const char* fileName="depthImage.bmp";
	printf("depthBuffer saved as %s\n",fileName);
	distImg.writeBMP24BitInt(fileName,0.0001);
	mesh.getdepthImage = true;
}

float pmc::getZbuffer::getNorm(pvm::Vector3D a,pvm::Vector3D b)
{
	float distance;
	float temp;
	
	temp = pow(b.elem[0] - a.elem[0],2) + pow(b.elem[1] - a.elem[1],2) + pow(b.elem[2] - a.elem[2],2);
	distance = sqrt(temp);

	return distance;
}

void pmc::getZbuffer::visibilty()
{
	std::vector<pmc::NearestPoint>n_p_list;
	n_p_list.resize(mesh.vertex_list.size());
	float aveDistance = 0;

	for(int s=0;s<mesh.vertex_list.size();s++)
	{
		float temp = getNorm(mesh.vertex_list[s],depthMesh.vertex_list[0]);
		for(int a=0;a<depthMesh.vertex_list.size();a++)
		{
			
			float temp2 = 0;
			temp2 = getNorm(mesh.vertex_list[s],depthMesh.vertex_list[a]);
			//std::cout <<"norm "<<temp2 <<std::endl;

			if(temp2 > 1000)
			{
				std::cout <<"depth "<<depthMesh.vertex_list[a].elem[0] << " "<< depthMesh.vertex_list[a].elem[1] << " "<< depthMesh.vertex_list[a].elem[2] << " "<< std::endl;
				std::cout <<"mesh " << mesh.vertex_list[s].elem[0] <<" "<< mesh.vertex_list[s].elem[1] <<" "<< mesh.vertex_list[s].elem[2] <<std::endl;
				system("PAUSE");
			}

			if(temp2 < temp)temp = temp2;

			//std::cout << "test";
			
		}
		
		n_p_list[s].norm = temp;
		aveDistance += temp;
		mesh.visibility_check[s] = true;
	}
	
	aveDistance = aveDistance/(float)mesh.vertex_list.size();

	std::cout <<"aveDistance:"<<aveDistance<<std::endl;
	Mesh test;
	for(int s=0;s<mesh.vertex_list.size();s++)
	{
		if(n_p_list[s].norm > aveDistance)
		{
			mesh.visibility_check[s] = false;
		}

		if(mesh.visibility_check[s] == true)
		{
			test.vertex_list.push_back(mesh.vertex_list[s]);
		}
	}

	
	smesh::Mesh tt = convert2smesh(test);
	tt.writeVertex("result.txt");
}

void pmc::getZbuffer::getDistanceKinect2Tenbo()
{

}