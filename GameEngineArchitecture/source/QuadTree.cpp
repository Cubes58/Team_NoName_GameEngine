#include "..\include\QuadTree.h"

QuadTree::QuadTree(int p_Level, BoundingBox p_Box)
{
	m_Level = p_Level;
	m_Box = p_Box;
	m_Nodes.reserve(4);
}

void QuadTree::Clear()
{
	m_Objects.clear();
	for (QuadTree* l_Node : m_Nodes) {
		if (l_Node != nullptr) {
			l_Node->Clear();
		}
	}
}

void QuadTree::Split()
{
	int subWidth = m_Box.length / 2.0f;
	int subHeight = m_Box.width / 2.0f;
	int x = m_Box.x;
	int y = m_Box.y;

	//(0,0) in top left
	m_Nodes.push_back(new QuadTree(m_Level + 1, BoundingBox(x, y, subWidth, subHeight)));//top left quad
	m_Nodes.push_back(new QuadTree(m_Level + 1, BoundingBox(x + subWidth, y, subWidth, subHeight)));//top right quad
	m_Nodes.push_back(new QuadTree(m_Level + 1, BoundingBox(x + subWidth, y + subHeight, subWidth, subHeight)));//bot right quad
	m_Nodes.push_back(new QuadTree(m_Level + 1, BoundingBox(x, y + subHeight, subWidth, subHeight)));//bot left quad
}

int QuadTree::GetIndex(float x, float y)
{
	int index = -1;
	double verticalMidpoint = m_Box.x + (m_Box.width / 2);
	double horizontalMidpoint = m_Box.y + (m_Box.length / 2);

	bool topQuadrant = (y < horizontalMidpoint);

	bool bottomQuadrant = (y > horizontalMidpoint);

	if (x < verticalMidpoint) {
		if (topQuadrant) {
			index = 1;
		}
		else if (bottomQuadrant) {
			index = 2;
		}
	}

	else if (x > verticalMidpoint) {
		if (topQuadrant) {
			index = 0;
		}
		else if (bottomQuadrant) {
			index = 3;
		}
	}

	return index;
}

void QuadTree::Insert(std::shared_ptr<GameObject> p_GameObject)
{
	if (m_Nodes.size() > 0) {
		glm::vec3 l_Pos = p_GameObject->GetComponent<TransformComponent>()->Position();
		int index = GetIndex(l_Pos.x, l_Pos.y);

		if (index != -1) {
			m_Nodes[index]->Insert(p_GameObject);
			return;
		}
	}

	m_Objects.push_back(p_GameObject);

	if (m_Objects.size() > m_MaxObjs && m_Level < m_MaxLevels) {
		if (m_Nodes.size() == 0) {
			Split();
		}

		int i = 0;
		while (i < m_Objects.size()) {
			glm::vec3 l_Pos = m_Objects[i]->GetComponent<TransformComponent>()->Position();
			int index = GetIndex(l_Pos.x, l_Pos.y);
			if (index != -1) {
				m_Nodes[index]->Insert(m_Objects[i]);
				m_Objects.erase(m_Objects.begin() + i);
			}
			else {
				i++;
			}
		}
	}
}

std::vector<std::shared_ptr<GameObject>> QuadTree::Retrieve(float x, float y, std::vector<std::shared_ptr<GameObject>>& p_Objects)
{
	int l_Index = GetIndex(x, y);
	if (l_Index != -1 && m_Nodes[0] != nullptr) {
		m_Nodes[l_Index]->Retrieve(x, y, p_Objects);
	}

	for (std::shared_ptr<GameObject> l_Object : m_Objects) {
		p_Objects.push_back(l_Object);
	}

	return p_Objects;
}