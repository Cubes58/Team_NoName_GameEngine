#pragma once
#include <vector>
#include <list>
#include <glm/glm.hpp>
#include <math.h>
//#include "GraphNode.h"

class GraphNode;

class Graph {


public:

	Graph(glm::vec3 p_Pos, float p_distance, glm::vec2 p_numberOfNodes);
	std::list<GraphNode*> GetThePath(glm::vec3 p_start, glm::vec3 p_goal);

	std::vector<std::vector<GraphNode*>> getGraph() { return m_Graph; };
	void setNodeInvalid(GraphNode* p_Node);

	GraphNode * FindNode(glm::vec3 p_nodePos);
	GraphNode * GetNode(int p_idx, int p_idy);
	
protected:

	bool m_isInClosedList;
	bool m_isInOpenList;
	float m_distance;
	glm::vec2 m_numberOfNodes;
	GraphNode * m_storeLast;

    std::vector<std::vector<GraphNode*>> m_Graph;

	std::list<GraphNode*> GatherTheChildren(GraphNode* p_node);
	std::list<GraphNode*> ConstructThePath(GraphNode* p_startNode, GraphNode* p_endNode);
	float CalculateH(glm::vec3 p_start, glm::vec3 p_end);

	std::list<GraphNode*> m_closedList; 
	std::list<GraphNode*> m_openList;
	std::list<GraphNode*> m_pathList; 
	std::list<GraphNode*> m_childList;

};