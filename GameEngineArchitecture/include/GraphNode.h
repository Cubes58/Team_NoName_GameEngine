#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <string>

class GraphNode{

public:

	virtual void setNodePos(glm::vec3 p_nodesPosition) { m_pos = p_nodesPosition; }
	virtual glm::vec3 getNodesPos() { return m_pos; }
	virtual std::string checkNodeType() = 0;

	virtual void setTheG(float p_setG) { m_G = p_setG; };
	virtual void setTheF(float p_setF) { m_F = p_setF; };
	virtual void setTheH(float p_setH) { m_H = p_setH; };
	virtual void setTheDiagonal(bool p_b) { m_isDiagonal = p_b; };

	virtual GraphNode* getThePerant() { return m_perantNode; }; 
	virtual void setThePerant(GraphNode* p_Node) { m_perantNode = p_Node; }; 
	virtual void setID(int p_x, int p_y) { m_idX = p_x; m_idY = p_y; }
	virtual std::pair<int, int> getID() { return std::make_pair(m_idX, m_idY); };

	virtual float getTheG() { return m_G; };
	virtual float getTheF() { return m_F; };
	virtual float getTheH() { return m_H; };
	virtual float getTheDiagonal() { return m_isDiagonal; };


protected:

	int m_idX; int m_idY;
	GraphNode * m_perantNode;
	glm::vec3 m_pos;
	float m_F;
	float m_G;
	float m_H;
	bool m_isDiagonal;

};

class InvalidNode : public GraphNode {

public:

	InvalidNode(glm::vec3 p_pos, int p_idX, int p_idY) { setNodePos(p_pos); setID(p_idX, p_idY); }
	std::string checkNodeType() override { return "InvalidNode"; }

private:

};

class NormalNode : public GraphNode {

public:

	NormalNode(glm::vec3 p_pos, int p_idX, int p_idY) { setNodePos(p_pos); setID(p_idX, p_idY); }
	std::string checkNodeType() override { return "NormalNode"; }

private:


};
