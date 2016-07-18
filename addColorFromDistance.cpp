#include "addColorFromDistance.h"

int pmc::addColorFromDistance::getIndexList(std::string name)
{
	std::fstream ifs(name);
	std::string str;

	if (ifs.fail())
    {
        std::cerr << "Ž¸”s" << std::endl;
        return -1;
	}

	while (getline(ifs, str))
    {
		std::stringstream stm;
		stm << str;
		int temp;
		stm >> temp;
		indexList.push_back(temp);
	}
	return 0;
}

int pmc::addColorFromDistance::getDistanceList(std::string name)
{
	std::fstream ifs(name);
	std::string str;

	if (ifs.fail())
    {
        std::cerr << "Ž¸”s" << std::endl;
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
	smesh::RGBuchar col;

	for(int a=0;a<3;a++)
	{
		col.rgba[a] = 0;
	}
	col.rgba[4] = 255;

	float begin = 0;
	float end = 255;
	float colorNorm;

	//0`255‚Ì”ÍˆÍ‚Å³‹K‰»
	colorNorm = ( (distance-min)*(end - begin) ) / ( (max - min) + begin );
	
	if(minus)
	{
		col.rgba[2] = colorNorm;
	}else{
		col.rgba[1] = colorNorm;
	}

	return col;
}


void pmc::addColorFromDistance::convertDistanceToColors()
{
	float max = *std::max_element(distanceList.begin(),distanceList.end());
	float zero = 0.0;
	float min = *std::min_element(distanceList.begin(),distanceList.end());

	std::cout << "max "<<max<<" min "<<min<<std::endl;

	for(int s=0;s<distanceList.size();s++)
	{
		smesh::RGBuchar col;
		if(distanceList[s] <0)
		{
			col = convertDistanceToColor(-distanceList[s],-min,0,true);
		}else{
			col = convertDistanceToColor(distanceList[s],max,0);
		}

		mesh.color_list.push_back(col);
	}
	mesh.writeply("distanceColorTest.ply");
}


void pmc::addColorFromDistance::readMesh(std::string name)
{
	mesh.readobj(name);
}
