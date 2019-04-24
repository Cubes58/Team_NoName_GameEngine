#pragma once
#include <iostream>
#include <list>

class BehaviourNode {

public:
	virtual bool run() = 0;
};

class CompositeNode : public BehaviourNode {
//This class is used to store all the children of a node
private:
	std::list<BehaviourNode*> m_childrenList; 
public:
	const std::list<BehaviourNode*>& getTheChildren() const { return m_childrenList; }
	void addTheChildren(BehaviourNode* p_child) { m_childrenList.emplace_back(p_child);	}
};

class Selector : public CompositeNode {

public:
	bool run() override {

		for (BehaviourNode* BN : getTheChildren()) 
		{
			if (BN->run()) { return true; } //Run will work as long as one child is working
		}
		return false;
	}

};

class Sequence : public CompositeNode {

public:
	bool run() override {
		for (BehaviourNode* BN : getTheChildren())
		{
			if (!BN->run()) { return false; } //every child needs to work for this one
		}
		return true;
	}
};

class LeafNodeRageExample : public BehaviourNode
{
private : 

	int m_rage; //Just rage for this example

public:

	void giveMeInt(int p_rage) { m_rage = p_rage; } //Stores new Value

	bool run() override {
		if (m_rage <= 100){ //This part will be the condition of the node
			return true;  
			std::cout << "Enemy Has Embrace the Void and it drove him Mad" << std::endl;
		}
		else return false;
		std::cout << "Enemy is calm..... for now" << std::endl;
	}
};

class LeafNodeActionExample : public BehaviourNode
{
private:

	//int m_rage; //Just rage for this example

public:

	//void giveMeInt(int p_rage) { m_rage = p_rage; } //Stores new Value

	bool run() override {
		
		std::cout << "AUGHHHHH!!!!!!" << std::endl;
		return true;

	}
};


//Inside a NPC 
//Sequance will be the root
//Example Sequance* root = new Sequance;
//Example Selector* selectorOne = new Selector;
//Selectors will be added to this Sequance each containing condition and action leaf nodes
//root->addTheChildren(selectorOne);

//LeafNodeRageExample * m_RageLeaf;
//in each update loop you can pass it a new Rage value
//m_rageLeaf->giveMeInt(m_currentNPCRage);
//next time run is called, it could return a diffrent value then before

//Last thing to do is to add it to the selector 
//selectorOne->addTheChildren(m_RageLeaf);
//The next action could be another condition or action they will only be hit if m_RageLeaf is true..... i think
//if one is false it would move onto the next selector in the sequance..... i think
//Each time a node is created in here it can re used, so when creating a new enemy with simlair
//properties you wont have to re use this code

//I hope this helps this can be confusing (i keep mixing up selector and sequance)
	


