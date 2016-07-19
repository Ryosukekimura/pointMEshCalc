#include "MyUtils.h"
#include <fstream>

#pragma once;
namespace pmc{

	class addColorFromDistance{
	public:

		std::vector<float> distanceList;
		std::vector<int> indexList;
		pmc::Mesh mesh;

		addColorFromDistance()
		{
			distanceList.resize(0);
			indexList.resize(0);
		}

		int getDistanceList(std::string distanceListName);
		int getIndexList(std::string indexListName);
		void readMesh(std::string name);
		void convertDistanceToColors(std::string name);
		smesh::RGBuchar convertDistanceToColor(float distance,float max,float min,bool minus);
	};
}