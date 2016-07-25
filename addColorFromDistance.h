#include "MyUtils.h"
#include <fstream>

#pragma once;
namespace pmc{

	class addColorFromDistance{
	public:

		std::vector<float> distanceList;
		pmc::Mesh mesh;

		addColorFromDistance()
		{
			distanceList.resize(0);
		}

		int getDistanceList(std::string distanceListName);
		void readMesh(std::string name);
		Mesh convertDistanceToColors(void);
		smesh::RGBuchar convertDistanceToColor(float distance,float max,float min,bool minus);
	};
}