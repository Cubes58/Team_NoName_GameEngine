/**
@file Grah.h
@brief Data of the graph of nodes for pathfinding aswell as helper methods to navigate it
*/

#pragma once
#include <vector>
#include <list>
#include <glm/glm.hpp>
#include <math.h>
//#include "GraphNode.h"

class GraphNode;

/*! \class Graph
	\brief builds the 2d vector of nodes, it also contains methods that handles this graph
*/

class Graph {


public:

	/*!
	\brief Constructs Graph
	\param p_Pos start pos of graph
	\param p_Distance distance between nodes
	\param p_NumberOfNodes how many nodes in the 2d Vector
	*/

	Graph(glm::vec3 p_Pos, float p_Distance, glm::vec2 p_NumberOfNodes);

	/*!
	\brief Gets a path
	\param p_Start start of path
	\param p_Goal the final node of the path
	\return list of nodes that will become the path
	*/

	std::list<GraphNode*> GetThePath(glm::vec3 p_Start, glm::vec3 p_Goal);

	/*!
	\brief Gets the graph
	\return a 2d vector
	*/

	std::vector<std::vector<GraphNode*>> GetGraph() { return m_Graph; };

	/*!
	\brief Sets a node invalid
	\param p_Node the node that will be made invalid
	*/

	void SetNodeInvalid(GraphNode* p_Node);

	/*!
	\brief Finds the node
	\param p_NodePos find the node nearest to this
	\return Node
	*/

	GraphNode * FindNode(glm::vec3 p_NodePos);

	/*!
	\brief used to get a node
	\param p_Idx id used to find node
	\param p_Idy id used to find node
	\return Node
	*/

	GraphNode * GetNode(int p_Idx, int p_Idy);

	/*!
    \brief alculates the distance between nodes
    \param p_Start start position
    \param p_End end position
    \return distance
    */

	float CalculateH(glm::vec3 p_Start, glm::vec3 p_End);


protected:

	bool m_IsInClosedList; //!< bool used to tell if something is in the closed list
	bool m_IsInOpenList; //!< bool used to tell if something is in the open list
	float m_Distance; //!< float used to determine the distance
	glm::vec2 m_NumberOfNodes; //!< Stores the total number of nodes
	GraphNode * m_StoreLast; //!< Stores the last node

	std::vector<std::vector<GraphNode*>> m_Graph; //!< The Graph with all the nodes contained

	/*!
	\brief Gathers the 8 nodes around the current node
	\param p_Node the current node
	\return 8 child nodes
	*/

	std::list<GraphNode*> GatherTheChildren(GraphNode* p_Node);

	/*!
	\brief used to create the path of nodes
	\param p_StartNode the start of the path
	\param p_EndNode the end of the path
	\return path
	*/

	std::list<GraphNode*> ConstructThePath(GraphNode* p_StartNode, GraphNode* p_EndNode);


	std::list<GraphNode*> m_ClosedList;  //!< A list containing the nodes that have been looked at
	std::list<GraphNode*> m_OpenList;   //!< A list containing the nodes that are not done with yet
	std::list<GraphNode*> m_PathList;   //!< A list of nodews to generate the path
	std::list<GraphNode*> m_ChildList;  //!< Stores the 8 nodes got from the gather method

};

