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
	m_NumberOfNodes = p_numberOfNodes;
	m_Distance = p_distance;
}

std::list<GraphNode*> Graph::GetThePath(glm::vec3 p_start, glm::vec3 p_goal)
{

	float GValueTemp;
	GraphNode* tempNode;
	GraphNode* tempGoalNode;
	GraphNode* tempStartNode;

	m_OpenList.clear();
	m_ClosedList.clear();

    //tempNode = FindNode(p_start);
	m_OpenList.push_back(FindNode(p_start));
	tempGoalNode = FindNode(p_goal);
	tempStartNode = FindNode(p_start);
	m_OpenList.front()->SetTheG(0.0f); m_OpenList.front()->SetTheF(0.0f);

	for (int i = 0; i < m_NumberOfNodes.x; i++)
	{
		for (int j = 0; j < m_NumberOfNodes.y; j++)
		{
			m_Graph.at(i).at(j)->SetThePerant(nullptr);
		}
	}

	while (!m_OpenList.empty())
	{
		tempNode = m_OpenList.front();

		for (auto* i : m_OpenList)
		{
			if (i->GetTheF() < tempNode->GetTheF())	{ tempNode = i; }
		}

		m_OpenList.remove(tempNode);
		m_ClosedList.push_back(tempNode);

		if (tempNode->GetID().first == tempGoalNode->GetID().first and
		tempNode->GetID().second == tempGoalNode->GetID().second)
		{
			std::cout << "Goal Reached!!!" << std::endl;
			return ConstructThePath(tempStartNode, tempGoalNode);
		}

		for (auto * j : GatherTheChildren(tempNode))
		{
			if (j->CheckNodeType() == "InvalidNode")
				continue;

			m_IsInClosedList = false;
			for (auto* k : m_ClosedList) {
				if (k == j) { m_IsInClosedList = true;}
			}

			if (m_IsInClosedList)
				continue;

			if (j->GetTheDiagonal()) { GValueTemp = tempNode->GetTheG() + 1.4f; }
			else if (!j->GetTheDiagonal()) { GValueTemp = tempNode->GetTheG() + 1; };

			m_IsInOpenList = false;
			for (auto* l : m_OpenList)
			{
				if (l == j) {
					m_IsInOpenList = true; 
				}
			}

			if (GValueTemp < j->GetTheG() or !m_IsInOpenList) {

				j->SetTheG(GValueTemp);
			    j->SetTheH(CalculateH(tempNode->GetNodesPos(), p_goal));
				j->SetTheF(j->GetTheG() + j->GetTheH());

				j->SetThePerant(tempNode);
				m_StoreLast = j;

				if (!m_IsInOpenList)
					m_OpenList.push_back(j);

			}
			
		}

	}


	return ConstructThePath(tempStartNode, m_StoreLast);
}

void Graph::SetNodeInvalid(GraphNode * p_Node)
{
	m_Graph.at(p_Node->GetID().first).at(p_Node->GetID().second) = new InvalidNode(p_Node->GetNodesPos(), p_Node->GetID().first, p_Node->GetID().second);
}

GraphNode * Graph::FindNode(glm::vec3 p_nodePos)
{
	GraphNode* tempNode; tempNode = new NormalNode(glm::vec3(10000.f, 10000.f, 10000.f), 1000, 1000);

	for (int i = 0; i < m_NumberOfNodes.x; i++)
	{
		for (int j = 0; j < m_NumberOfNodes.y; j++)
		{
			if (m_Graph.at(i).at(j)->CheckNodeType() != "InvalidNode")
			{
				if (CalculateH(tempNode->GetNodesPos(), p_nodePos) > (CalculateH(m_Graph.at(i).at(j)->GetNodesPos(), p_nodePos)))
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
	for (int i = 0; i < m_NumberOfNodes.x; i++)
	{
		for (int j = 0; j < m_NumberOfNodes.y; j++)
		{
			if (m_Graph.at(i).at(j)->GetID().first == p_idx and 
			m_Graph.at(i).at(j)->GetID().second == p_idy)
			{
				return m_Graph.at(i).at(j);
			}
		}
	}


	//return nullptr;
}

std::list<GraphNode*> Graph::GatherTheChildren(GraphNode * p_node)
{
	m_ChildList.clear();

	auto getCloseNode = [&](std::list<GraphNode*> p_TempList, int x = 0, int y = 0) {
		m_ChildList.push_back(m_Graph.at(p_node->GetID().first + x).at(p_node->GetID().second + y));
		m_Graph.at(p_node->GetID().first + x).at(p_node->GetID().second + y)->SetTheDiagonal(false);
		if (x != 0 and y != 0) { m_Graph.at(p_node->GetID().first + x).at(p_node->GetID().second + y)->SetTheDiagonal(true); }
	};

	if (p_node->GetID().first != 0) { getCloseNode(m_ChildList, -1, 0); }
	if (p_node->GetID().first != m_NumberOfNodes.x - 1) { getCloseNode(m_ChildList, 1, 0); }
	if (p_node->GetID().second != m_NumberOfNodes.y - 1) { getCloseNode(m_ChildList, 0, 1); }
	if (p_node->GetID().second != 0) { getCloseNode(m_ChildList, 0, -1); }

	if (p_node->GetID().first != m_NumberOfNodes.x - 1 and p_node->GetID().second != m_NumberOfNodes.y - 1) { getCloseNode(m_ChildList, 1, 1); }
	if (p_node->GetID().first != 0 and p_node->GetID().second != m_NumberOfNodes.y - 1) { getCloseNode(m_ChildList, -1, 1); }
	if (p_node->GetID().first != 0 and p_node->GetID().second != 0) { getCloseNode(m_ChildList, -1, -1); }
	if (p_node->GetID().first != m_NumberOfNodes.x - 1 and p_node->GetID().second != 0) { getCloseNode(m_ChildList, 1, -1); }

	return m_ChildList;
}

std::list<GraphNode*> Graph::ConstructThePath(GraphNode * p_startNode, GraphNode * p_endNode)
{

	m_PathList.clear();
	GraphNode* currentNode = p_endNode;

	while (currentNode != p_startNode)
	{
		m_PathList.push_front(currentNode);
		currentNode = currentNode->GetThePerant();

		//So each node stores where it came from, and it will work backwards to create a the path
	}

	return m_PathList;

	//return std::list<GraphNode*>();
}

float Graph::CalculateH(glm::vec3 p_start, glm::vec3 p_end)
{
	float tempX; tempX = p_start.x - p_end.x;
	float tempY; tempY = p_start.z - p_end.z;	

	return sqrt((tempX * tempX) + (tempY * tempY));
}
