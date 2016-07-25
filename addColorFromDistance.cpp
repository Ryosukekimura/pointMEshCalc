#include "addColorFromDistance.h"

int pmc::addColorFromDistance::getDistanceList(std::string name)
{
	std::fstream ifs(name);
	std::string str;

	if (ifs.fail())
    {
        std::cerr << "é∏îs" << std::endl;
        return -1;
	}

	while (getline(ifs, str))
    {
		std::stringstream stm;
		stm << str;
		float temp;
		stm >> temp;
		distanceList.push_back(temp);
	}
	return 0;
}

smesh::RGBuchar pmc::addColorFromDistance::convertDistanceToColor(float distance,float max,float min,bool minus = false)
{
	smesh::RGBuchar col,red,blue;

	col.rgba[0] = 127;
	col.rgba[1] = 255;
	col.rgba[2] = 127;
	col.rgba[3] = 255;

	float begin = 0;
	float end = 127;
	float colorNorm;

	//0Å`255ÇÃîÕàÕÇ≈ê≥ãKâª
	colorNorm = ( (distance-min)*(end - begin) ) / ( (max - min) + begin );
	
	if(minus)
	{
		col.rgba[1] -= colorNorm;
		col.rgba[2] += colorNorm;
	}else{
		col.rgba[1] -= colorNorm;
		col.rgba[0] += colorNorm;
	}

	return col;
}


pmc::Mesh pmc::addColorFromDistance::convertDistanceToColors()
{
	float max = *std::max_element(distanceList.begin(),distanceList.end());
	float zero = 0.0;
	float min = *std::min_element(distanceList.begin(),distanceList.end());

	std::cout << "max "<<max<<" min "<<min<<std::endl;

	mesh.color_list.resize(mesh.vertex_list.size());
	for(int a=0;a<mesh.vertex_list.size();a++)
	{
		mesh.color_list[a].rgba[0] = 127;
		mesh.color_list[a].rgba[1] = 127;
		mesh.color_list[a].rgba[2] = 127;
		mesh.color_list[a].rgba[3] = 255;
	}

	for(int s=0;s<mesh.vertex_list.size();s++)
	{
		smesh::RGBuchar col;
		if(mesh.visibility_check[s] == false)
		{
			col.rgba[0] = 200;
			col.rgba[1] = 200;
			col.rgba[2] = 200;
			col.rgba[3] = 255;
			mesh.color_list[s] = col;

			continue;
		}
		if(distanceList[s] <0)
		{
			col = convertDistanceToColor(-distanceList[s],-min,0,true);
		}else{
			col = convertDistanceToColor(distanceList[s],max,0);
		}
		mesh.color_list[s] = col;
		
	}
	return mesh;
}


void pmc::addColorFromDistance::readMesh(std::string name)
{
	mesh.readobj(name);
}
