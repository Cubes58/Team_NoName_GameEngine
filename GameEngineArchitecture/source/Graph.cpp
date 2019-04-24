#include "Graph.h"
#include "GraphNode.h"

Graph::Graph(glm::vec3 p_Pos, float p_distance, glm::vec2 p_numberOfNodes)
{
	for (int i = 0; i < (int)p_numberOfNodes.x; i++)
	{
		int x, y; x = p_Pos.x + (i * p_distance); y = p_Pos.y;
		std::vector < GraphNode*> temp;
		
		for (float j = 0; j < (int)p_numberOfNodes.y; j++)
		{
			if (i == 3 and j == 3)
			{
				int z; z = p_Pos.z + (j * p_distance);
				temp.push_back(new InvalidNode(glm::vec3(x, y, z), i, j));
			}
			else {
				int z; z = p_Pos.z + (j * p_distance);
				temp.push_back(new NormalNode(glm::vec3(x, y, z), i, j));
			}
		}
		m_Graph.push_back(temp);

	}
	m_numberOfNodes = p_numberOfNodes;
	m_distance = p_distance;
}

std::list<GraphNode*> Graph::GetThePath(glm::vec3 p_start, glm::vec3 p_goal)
{

	float GValueTemp;
	GraphNode* tempNode;
	GraphNode* tempGoalNode;
	GraphNode* tempStartNode;

	m_openList.clear();
	m_closedList.clear();

    //tempNode = FindNode(p_start);
	m_openList.push_back(FindNode(p_start));
	tempGoalNode = FindNode(p_goal);
	tempStartNode = FindNode(p_start);
	m_openList.front()->setTheG(0.0f); m_openList.front()->setTheF(0.0f);

	for (int i = 0; i < m_numberOfNodes.x; i++)
	{
		for (int j = 0; j < m_numberOfNodes.y; j++)
		{
			m_Graph.at(i).at(j)->setThePerant(nullptr);
		}
	}

	while (!m_openList.empty())
	{
		tempNode = m_openList.front();

		for (auto* i : m_openList)
		{
			if (i->getTheF() < tempNode->getTheF())	{ tempNode = i; }
		}

		m_openList.remove(tempNode);
		m_closedList.push_back(tempNode);

		if (tempNode->getID().first == tempGoalNode->getID().first and
		tempNode->getID().second == tempGoalNode->getID().second)
		{
			std::cout << "Goal Reached!!!" << std::endl;
			return ConstructThePath(tempStartNode, tempGoalNode);
		}

		for (auto * j : GatherTheChildren(tempNode))
		{
			if (j->checkNodeType() == "InvalidNode")
				continue;

			m_isInClosedList = false;
			for (auto* k : m_closedList) {
				if (k == j) { m_isInClosedList = true;}
			}

			if (m_isInClosedList)
				continue;

			if (j->getTheDiagonal()) { GValueTemp = tempNode->getTheG() + 1.4f; }
			else if (!j->getTheDiagonal()) { GValueTemp = tempNode->getTheG() + 1; };

			m_isInOpenList = false;
			for (auto* l : m_openList)
			{
				if (l == j) {
					m_isInOpenList = true; 
				}
			}

			if (GValueTemp < j->getTheG() or !m_isInOpenList) {

				j->setTheG(GValueTemp);
			    j->setTheH(CalculateH(tempNode->getNodesPos(), p_goal));
				j->setTheF(j->getTheG() + j->getTheH());

				j->setThePerant(tempNode);
				m_storeLast = j;

				if (!m_isInOpenList)
					m_openList.push_back(j);

			}
			
		}

	}


	return ConstructThePath(tempStartNode, m_storeLast);
}

void Graph::setNodeInvalid(GraphNode * p_Node)
{
	m_Graph.at(p_Node->getID().first).at(p_Node->getID().second) = new InvalidNode(p_Node->getNodesPos(), p_Node->getID().first, p_Node->getID().second);
}

GraphNode * Graph::FindNode(glm::vec3 p_nodePos)
{
	GraphNode* tempNode; tempNode = new NormalNode(glm::vec3(10000.f, 10000.f, 10000.f), 1000, 1000);

	for (int i = 0; i < m_numberOfNodes.x; i++)
	{
		for (int j = 0; j < m_numberOfNodes.y; j++)
		{
			if (m_Graph.at(i).at(j)->checkNodeType() != "InvalidNode")
			{
				if (CalculateH(tempNode->getNodesPos(), p_nodePos) > (CalculateH(m_Graph.at(i).at(j)->getNodesPos(), p_nodePos)))
				{
					tempNode = m_Graph.at(i).at(j);
				}
			}
		}
	}


	return tempNode;
}

GraphNode * Graph::GetNode(int p_idx, int p_idy)
{
	for (int i = 0; i < m_numberOfNodes.x; i++)
	{
		for (int j = 0; j < m_numberOfNodes.y; j++)
		{
			if (m_Graph.at(i).at(j)->getID().first == p_idx and 
			m_Graph.at(i).at(j)->getID().second == p_idy)
			{
				return m_Graph.at(i).at(j);
			}
		}
	}


	//return nullptr;
}

std::list<GraphNode*> Graph::GatherTheChildren(GraphNode * p_node)
{
	m_childList.clear();

	auto getCloseNode = [&](std::list<GraphNode*> p_TempList, int x = 0, int y = 0) {
		m_childList.push_back(m_Graph.at(p_node->getID().first + x).at(p_node->getID().second + y));
		m_Graph.at(p_node->getID().first + x).at(p_node->getID().second + y)->setTheDiagonal(false);
		if (x != 0 and y != 0) { m_Graph.at(p_node->getID().first + x).at(p_node->getID().second + y)->setTheDiagonal(true); }
	};

	if (p_node->getID().first != 0) { getCloseNode(m_childList, -1, 0); }
	if (p_node->getID().first != m_numberOfNodes.x - 1) { getCloseNode(m_childList, 1, 0); }
	if (p_node->getID().second != m_numberOfNodes.y - 1) { getCloseNode(m_childList, 0, 1); }
	if (p_node->getID().second != 0) { getCloseNode(m_childList, 0, -1); }

	if (p_node->getID().first != m_numberOfNodes.x - 1 and p_node->getID().second != m_numberOfNodes.y - 1) { getCloseNode(m_childList, 1, 1); }
	if (p_node->getID().first != 0 and p_node->getID().second != m_numberOfNodes.y - 1) { getCloseNode(m_childList, -1, 1); }
	if (p_node->getID().first != 0 and p_node->getID().second != 0) { getCloseNode(m_childList, -1, -1); }
	if (p_node->getID().first != m_numberOfNodes.x - 1 and p_node->getID().second != 0) { getCloseNode(m_childList, 1, -1); }

	return m_childList;
}

std::list<GraphNode*> Graph::ConstructThePath(GraphNode * p_startNode, GraphNode * p_endNode)
{

	m_pathList.clear();
	GraphNode* currentNode = p_endNode;

	while (currentNode != p_startNode)
	{
		m_pathList.push_front(currentNode);
		currentNode = currentNode->getThePerant();

		//So each node stores where it came from, and it will work backwards to create a the path
	}

	return m_pathList;

	//return std::list<GraphNode*>();
}

float Graph::CalculateH(glm::vec3 p_start, glm::vec3 p_end)
{
	float tempX; tempX = p_start.x - p_end.x;
	float tempY; tempY = p_start.z - p_end.z;	

	return sqrt((tempX * tempX) + (tempY * tempY));
}
