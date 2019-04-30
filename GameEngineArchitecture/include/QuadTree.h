#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <glm/glm.hpp>
#include "GameObject.h"
#include "TransformComponent.h"
#include "AABBComponent.h"


class BoundingBox {
public:
	BoundingBox() {};
	BoundingBox(float p_x, float p_y, float p_width, float p_length) {
		x = p_x;
		y = p_y;
		width = p_width;
		length = p_length;
	}
	float x;
	float y;
	float width;
	float length;
};

class QuadTree {
private:
	const int m_MaxObjs = 10;
	const int m_MaxLevels = 5;
	int m_Level;
	std::vector<std::shared_ptr<GameObject>> m_Objects;
	BoundingBox m_Box;
	std::vector<QuadTree*> m_Nodes;
public:
	QuadTree(int p_Level, BoundingBox p_Box);
	//Setup
	void Clear();
	void Split();
	int GetIndex(float x, float y);
	void Insert(std::shared_ptr<GameObject>);
	//Retrieval
	std::vector<std::shared_ptr<GameObject>> Retrieve(float x, float y, std::vector<std::shared_ptr<GameObject>>& p_Objects);

};