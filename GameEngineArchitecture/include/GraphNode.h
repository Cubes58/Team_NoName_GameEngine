/**
@file GrahNode.h
@brief Data each node in the graph should contain.
*/

#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <string>

/*! \class GraphNode
	\brief The data that must be in each node that is used for the graph
*/



class GraphNode {

public:

	/*!
	\brief Constructs Node
	\param p_NodesPosition sets Pos
	*/

	virtual void SetNodePos(glm::vec3 p_NodesPosition) { m_Pos = p_NodesPosition; }

	/*!
	\brief Gets the position of the Node
	\return node pos
	*/

	virtual glm::vec3 GetNodesPos() { return m_Pos; }

	/*!
	\brief Checks if the node is Invalid
	\return the type of a node as a string
	*/


	virtual std::string CheckNodeType() = 0;

	/*!
	\brief Set Method
	\param p_SetG the new G value
	*/

	virtual void SetTheG(float p_SetG) { m_G = p_SetG; };

	/*!
	\brief Set Method
	\param p_SetF the new F value
	*/

	virtual void SetTheF(float p_SetF) { m_F = p_SetF; };

	/*!
	\brief Set Method
	\param p_SetH the new H value
	*/

	virtual void SetTheH(float p_SetH) { m_H = p_SetH; };

	/*!
	\brief Set Method
	\param p_B set Diagonal
	*/

	virtual void SetTheDiagonal(bool p_B) { m_IsDiagonal = p_B; };

	/*!
	\brief Get Method
	\return The perant node
	*/

	virtual GraphNode* GetThePerant() { return m_PerantNode; };

	/*!
	\brief Set Method
	\param p_Node set the perant node
	*/


	virtual void SetThePerant(GraphNode* p_Node) { m_PerantNode = p_Node; };

	/*!
	\brief Set Method
	\param p_X sets X
	\param p_Y sets Y
	*/


	virtual void SetID(int p_X, int p_Y) { m_IdX = p_X; m_IdY = p_Y; }

	/*!
	\brief Get Method
	\return the ID as a pair
	*/

	virtual std::pair<int, int> GetID() { return std::make_pair(m_IdX, m_IdY); };

	/*!
	\brief Get Method
	\return G
	*/
	virtual float GetTheG() { return m_G; };
	/*!
	\brief Get Method
	\return F
	*/
	virtual float GetTheF() { return m_F; };
	/*!
	\brief Get Method
	\return H
	*/
	virtual float GetTheH() { return m_H; };
	/*!
	\brief Get Method
	\return Diagonal
	*/
	virtual bool GetTheDiagonal() { return m_IsDiagonal; };


protected:

	int m_IdX; //!< x id
	int m_IdY; //!< y id
	GraphNode * m_PerantNode; //!< The Prevoius Node
	glm::vec3 m_Pos; //!< Its Position 
	float m_F; //!< F value
	float m_G; //!< G value
	float m_H; //!< H value
	bool m_IsDiagonal; //!< Is it a Diagonal 

};

/*! \class InvalidNode
	\brief A node that cannot be explored
*/

class InvalidNode : public GraphNode {

public:

	/*!
	\brief Constructor
	\param p_Pos its position
	\param p_IdX id for x
	\param p_IdY id for y
	*/

	InvalidNode(glm::vec3 p_Pos, int p_IdX, int p_IdY) { SetNodePos(p_Pos); SetID(p_IdX, p_IdY); }

	/*!
	\brief Get Method
	\return String to find out type
	*/

	std::string CheckNodeType() override { return "InvalidNode"; } //!< Return String

private:

};

/*! \class NormalNode
	\brief A node that cannot be explored
*/

class NormalNode : public GraphNode {

public:

	/*!
	\brief Constructor
	\param p_Pos its position
	\param p_IdX id for x
	\param p_IdY id for y
	*/

	NormalNode(glm::vec3 p_Pos, int p_IdX, int p_IdY) { SetNodePos(p_Pos); SetID(p_IdX, p_IdY); }

	/*!
	\brief Get Method
	\return String to find out type
	*/

	std::string CheckNodeType() override { return "NormalNode"; }

private:


};

