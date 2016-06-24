#pragma once

#include <vector>

#include "../NavMesh/NavMeshNode.hpp"

class PNBASharedData
{
	private:
		bool finished;
		float L;

	public:
		std::vector<NavMeshNode*> M;

		inline std::vector<NavMeshNode*> *getM() { return &M; }

		inline bool getFinished() const { return finished; }
		void setFinished(bool finished);

		inline float getL() const { return L; }
		void setL(float L);
};