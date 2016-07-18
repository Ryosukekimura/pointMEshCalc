#include "getZbuffer.h"

///////////////////////////////////////////////
///////////////////mesh class//////////////////
///////////////////////////////////////////////

pmc::Mesh::Mesh()
{
	vertex_list.resize(0);
	visibility_check.resize(0);
	face_list.resize(0);
	getdepthImage = false;
	FlagColor = false;
}

pmc::Mesh::Mesh(smesh::Mesh in_mesh)
{
	convert2pmcMesh(in_mesh);
}

pvm::Vector3D pmc::Mesh::getCenter()
{
	pvm::Vector3D c(0,0,0);
	int size = vertex_list.size();
	for(int s=0;s<size;s++)
	{
		pvm::Vector3D temp = vertex_list[s];
		c += temp;
	}

	c = c / (float)size;
	center = c;
	return c;
}

void pmc::Mesh::convert2pmcMesh( smesh::Mesh this_mesh)
{
	
	vertex_list.resize(this_mesh.numOfVertex());
	visibility_check.resize(this_mesh.numOfVertex());
	face_list.resize(this_mesh.numOfFace());
	getdepthImage = false;

	if(this_mesh.flagPointColored() == true) 
	{
		color_list.resize(this_mesh.numOfVertex());
		FlagColor = true;
	}
	

	for(int i = 0, h = this_mesh.numOfVertex(); i < h; i++) {
		for(int j = 0; j < 3; j++) {
			vertex_list[i].elem[j] = this_mesh.vertexCoord(i)[j];
		}
		
		visibility_check[i] = false;
		
		if(this_mesh.flagPointColored() == false) continue;
		
		for(int j = 0; j < 4;j++) {
			
			color_list[i].rgba[j] = this_mesh.vertexColor(i)[j];
		}
		
	}
	
	for(int i = 0, h = this_mesh.numOfFace(); i < h; i++) {
		for(int j = 0; j < 3; j++) {
			face_list[i].vertex[j] = this_mesh.vertexID(i,j);
		}
	}
	getCenter();
	
}

smesh::Mesh pmc::Mesh::convert2smesh()
{
	smesh::Mesh r_mesh;

	for(int s=0;s<vertex_list.size();s++)
	{
		pvm::Vector3D temp;
		temp = vertex_list[s];
		r_mesh.addVertex(temp);
		
		if(FlagColor == true || color_list.size() >= vertex_list.size())
		{
			smesh::RGBuchar tempc;
			tempc = color_list[s];
			r_mesh.addColor(tempc);
		}
	}

	for(int s=0;s<face_list.size();s++)
	{
		Face pmctemp = face_list[s];
		r_mesh.addFace(pmctemp.vertex[0],pmctemp.vertex[1],pmctemp.vertex[2]);
	}

	return r_mesh;
}


void pmc::Mesh::readply(std::string name)
{
	smesh::Mesh in;
	in.readPLY(name.c_str());
	this->convert2pmcMesh(in);
}

void pmc::Mesh::readobj(std::string name)
{
	smesh::Mesh in;
	in.readObj(name.c_str());
	this->convert2pmcMesh(in);
}

void pmc::Mesh::writeobj(std::string name)
{
	smesh::Mesh out = this->convert2smesh();
	out.writeObj(name.c_str());
}

void pmc::Mesh::writeply(std::string name)
{
	smesh::Mesh out = this->convert2smesh();
	out.writeInPly(name.c_str());
}

///////////////////////////////////////////////
/////////////getZbuffer class//////////////////
///////////////////////////////////////////////


////////////コンストラクタ/////////////////////

pmc::getZbuffer::getZbuffer(smesh::Mesh in_mesh)
{
	mesh.convert2pmcMesh(in_mesh);
	
}


pmc::getZbuffer::getZbuffer(smesh::Mesh tenboMesh,smesh::Mesh KinectMesh,int in_width,int in_height)
{
	mesh.convert2pmcMesh(KinectMesh);
	TenboMesh.convert2pmcMesh(tenboMesh);

	kwidth = in_width;
	kheight = in_height;
}


//read all file mesh1(Tenbo) mesh2(kinect)
pmc::getZbuffer::getZbuffer(int meshNum,std::string meshListName1,std::string meshListName2,int in_width,int in_height)
{
	kwidth = in_width;
	kheight = in_height;

	meshCount = 0;

	char str[100];
	for(int s=0;s<meshNum;s++)
	{
		smesh::Mesh stemp;
		Mesh ptemp;

		sprintf(str,meshListName1.c_str(),s);
		stemp.readObj(str);
		ptemp.convert2pmcMesh(stemp);
		meshList1.push_back(ptemp);
		std::cout << str<<std::endl;
		stemp.clearAll();

		sprintf(str,meshListName2.c_str(),s);
		std::cout << str<<std::endl;
		stemp.readObj(str);
		ptemp.convert2pmcMesh(stemp);
		meshList2.push_back(ptemp);
	}
}


void pmc::getZbuffer::Display(void)
{

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0,0.0,0.0,  mesh.center.elem[0],mesh.center.elem[1],mesh.center.elem[2]  ,0.0,1.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	DrawScene(mesh);

	glFlush();
	//　ダブルバッファ
	glutSwapBuffers();

}


void pmc::getZbuffer::Display_all(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0,0.0,0.0,  meshList2[0].center.elem[0],meshList2[0].center.elem[1],meshList2[0].center.elem[2]  ,0.0,1.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	DrawScene(meshList2[meshCount]);
	visibleCheckAndDistances(meshCount,meshList1[meshCount],meshList2[meshCount]);

	glFlush();
	runAnimation();
	
	//　ダブルバッファ
	glutSwapBuffers();
	
}


void pmc::getZbuffer::runAnimation()
{
	std::cout << meshList2.size() << std::endl;
	if(meshCount >= meshList2.size()-1){ 
		//meshCount=0; 
		return;
	}

	meshCount++;
	//std::cout << meshCount << std::endl;
}


void pmc::getZbuffer::DrawScene(Mesh this_mesh)
{
	unsigned int colIdx = 0;
	glBegin(GL_TRIANGLES);
	for(unsigned int i=0; i<this_mesh.face_list.size(); i++){
		
		glColor3ubv((GLubyte*)(&colIdx));
		pvm::Vector3D v1 = this_mesh.vertex_list[this_mesh.face_list[i].vertex[0]];
		pvm::Vector3D v2 = this_mesh.vertex_list[this_mesh.face_list[i].vertex[1]];
		pvm::Vector3D v3 = this_mesh.vertex_list[this_mesh.face_list[i].vertex[2]];
		
		glVertex3d(v1.elem[0], v1.elem[1], v1.elem[2]);
		glVertex3d(v2.elem[0], v2.elem[1], v2.elem[2]);
		glVertex3d(v3.elem[0], v3.elem[1], v3.elem[2]);
		colIdx += 1;
	}
	glEnd();
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
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_NORMALIZE);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}


void pmc::getZbuffer::keyboard(unsigned char key, int x, int y)
{
	switch(key) {
	case 'd':
		saveDepthImage2(&mesh);
		getDistanceTenbo2Kinect();
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


void pmc::getZbuffer::outputVisibilityPoint(Mesh this_mesh,std::string name)
{
	if(this_mesh.getdepthImage == false)
	{
		std::cout <<"可視判定が終わっていません"<<std::endl;
		return;
	}
	
	Mesh test;
	smesh::Mesh tea;

	smesh::RGBuchar white,red;
	for(int b=0;b<4;b++)
	{
		white.rgba[b] = 255;
	}

	red.rgba[0] = 255;
	red.rgba[1] = 0;
	red.rgba[2] = 0;
	red.rgba[3] = 255;

	for(int k=0;k<this_mesh.vertex_list.size();k++)
	{
		
		

		tea.addVertex(this_mesh.vertex_list[k]);
		if(this_mesh.visibility_check[k] == true)
		{
			tea.addColor(red);
			test.vertex_list.push_back(this_mesh.vertex_list[k]);
		}else{
			tea.addColor(white);
		}

		//tea.addColor(red);
	}
	
	smesh::Mesh tt = test.convert2smesh();
	//tt.writeVertex(name.c_str());
	tea.flagPointColored();
	tea.writeInPly(name.c_str());
	
	std::cout << "save bisible data ok" << std::endl;
}


void pmc::getZbuffer::saveDepthImage2(Mesh *this_mesh)//mesh2(kienctfitting)の可視判定
{
	//from NAIST 
	std::cout << "show depth image" << std::endl;
	cv::Mat idxImg = cv::Mat(kheight,kwidth, CV_8UC3);
	glReadPixels(0,0,idxImg.cols,idxImg.rows,GL_RGB,  GL_UNSIGNED_BYTE, idxImg.data);
	cv::flip(idxImg, idxImg, 0);
	//cv::imshow("index",idxImg);
	//cv::waitKey(1);

	std::vector<int> frontVertices(this_mesh->vertex_list.size(), 0);
	std::vector<int> frontFaces(this_mesh->face_list.size(), 0);


	for(int i=0; i<idxImg.rows; i++){
		for(int j=0; j<idxImg.cols; j++){
			unsigned int sufIdx = *(unsigned int*)(idxImg.data + (i * idxImg.cols + j) * 3);
			sufIdx &= 0x00ffffff;
			if(sufIdx == 0x00ffffff)
				continue;
			if(frontFaces[sufIdx] == 0){
				frontFaces[sufIdx] = 1;
				Face f = this_mesh->face_list[sufIdx];
				
				for(int a=0;a<3;a++)
				{
					this_mesh->visibility_check[f.vertex[a]] = true;
				}
			}
		}
	}
	this_mesh->getdepthImage = true;
	//outputVisibilityPoint(*this_mesh); //dubeg 
	std::cout << "get depth buffer ok" << std::endl;
}


float pmc::getZbuffer::getNorm(pvm::Vector3D a,pvm::Vector3D b) //b-a norm
{
	float distance;
	float temp;
	
	temp = pow(b.elem[0] - a.elem[0],2) + pow(b.elem[1] - a.elem[1],2) + pow(b.elem[2] - a.elem[2],2);
	distance = sqrt(temp);

	return distance;
}



void pmc::getZbuffer::moveMesh2Mesh(Mesh *mesh1,Mesh *mesh2)//move mesh1(tenbo) -> mesh2(kinect)
{
	pvm::Vector3D dist = mesh1->center - mesh2->center;
	
	for(int a=0;a<mesh1->vertex_list.size();a++)
	{
		mesh1->vertex_list[a] = mesh1->vertex_list[a] - dist;
	}
}


void pmc::getZbuffer::getDistanceMesh2Mesh(Mesh mesh1,Mesh mesh2, std::vector<pvm::Vector3D> *distanceList)//mesh2(visible) - mesh1 mesh2:kinectFitting mesh1:tenboOutput
{
	std::cout <<"calc distance ";
	if(mesh2.getdepthImage == false)
	{
		std::cout<<"false 可視判定がおわっていません\n";
		return;
	}

	moveMesh2Mesh(&mesh1,&mesh2);
	
	for(int a=0;a<mesh2.vertex_list.size();a++)
	{
		
		pvm::Vector3D temp;
		if(mesh2.visibility_check[a] == true)
		{
			temp = mesh2.vertex_list[a] - mesh1.vertex_list[a];
		}else{
			temp.setElement(-1000,-1000,-1000);
		}

		distanceList->push_back(temp);
	}
	std::cout <<"ok\n";
	//printDistance(*distanceList);
}


void pmc::getZbuffer::getDistanceTenbo2Kinect()
{
	getDistanceMesh2Mesh(TenboMesh,mesh,&distanceL);
}



void pmc::getZbuffer::visibleCheckAndDistances(int num, Mesh mesh1, Mesh mesh2)
{
	if(mesh1.getdepthImage == true)
	{
		return;
	}

	std::vector<pvm::Vector3D> distanceListTemp;
	char str[100];
	saveDepthImage2(&mesh2);
	getDistanceMesh2Mesh(mesh1,mesh2,&distanceListTemp);
	sprintf(str,"distance_%03d.csv",num);
	printDistance(distanceListTemp,str);
	sprintf(str,"visible_%03d.ply",num);
	outputVisibilityPoint(mesh2,str);
	
}


void pmc::getZbuffer::printDistance(std::vector<pvm::Vector3D> distanceList,std::string fileName)
{
	std::ofstream ofs(fileName);
	std::cout <<"distance list "<< fileName << " out put" << std::endl;
	for(int a=0;a<distanceList.size();a++)
	{
		for(int b=0;b<3;b++)
			ofs << distanceList[a].elem[b]<<",";
		ofs<<std::endl;
	}
}