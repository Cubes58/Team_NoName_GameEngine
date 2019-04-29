#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <string>

/*! The data that must be in a node that is used for the graph */

class GraphNode {

public:

	virtual void SetNodePos(glm::vec3 p_NodesPosition) { m_Pos = p_NodesPosition; }//!< Constructs Node
																				   /*!
																				   \param p_NodesPosition sets Pos
																				   */
	virtual glm::vec3 GetNodesPos() { return m_Pos; }//!< Gets the position of the Node
	virtual std::string CheckNodeType() = 0; //!< Checks if the node is Invalid

	virtual void SetTheG(float p_SetG) { m_G = p_SetG; };//!< Set Method
													     /*!
													     \param p_SetG the new G value
														 */
	virtual void SetTheF(float p_SetF) { m_F = p_SetF; };//!< Set Method
														 /*!
														 \param p_SetF the new F value
														 */
	virtual void SetTheH(float p_SetH) { m_H = p_SetH; };//!< Set Method
														 /*!
														 \param p_SetH the new H value
														 */
	virtual void GetTheDiagonal(bool p_B) { m_IsDiagonal = p_B; };//!< Set Method
																  /*!
																  \param p_B set Diagonal 
																  */

	virtual GraphNode* GetThePerant() { return m_PerantNode; }; //!< Gets the perant Node
	virtual void SetThePerant(GraphNode* p_Node) { m_PerantNode = p_Node; }; //!< Set Method
																			 /*!
																			 \param p_Node sets perant node
																			 */
	virtual void SetID(int p_X, int p_Y) { m_IdX = p_X; m_IdY = p_Y; }//!< Set Method
																	  /*!
																	  \param p_X sets X
																	  \param p_Y sets Y
																	  */
	virtual std::pair<int, int> GetID() { return std::make_pair(m_IdX, m_IdY); }; //!< Get Method

	virtual float GetTheG() { return m_G; }; //!< Set G
	virtual float GetTheF() { return m_F; }; //!< Set F
	virtual float GetTheH() { return m_H; }; //!< Set H
	virtual bool GetTheDiagonal() { return m_IsDiagonal; }; //!< Set the Diagonal


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

/*! A node that cannot be explored */

class InvalidNode : public GraphNode {

public:

	InvalidNode(glm::vec3 p_Pos, int p_IdX, int p_IdY) { SetNodePos(p_Pos); SetID(p_IdX, p_IdY); } //!< Constructer
																								   /*!
																								   \param p_Pos its position
																								   \param p_IdX id for x
																								   \param p_IdY id for y
																								   */
	std::string CheckNodeType() override { return "InvalidNode"; } //!< Return String

private:

};

/*! A node that cannot be explored */

class NormalNode : public GraphNode {

public:

	NormalNode(glm::vec3 p_Pos, int p_IdX, int p_IdY) { SetNodePos(p_Pos); SetID(p_IdX, p_IdY); }  //!< Constructer
																								   /*!
																								   \param p_Pos its position
																								   \param p_IdX id for x
																								   \param p_IdY id for y
																								   */
	std::string CheckNodeType() override { return "NormalNode"; } //!< Return String

private:


};
