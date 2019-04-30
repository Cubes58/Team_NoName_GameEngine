#define USE_GLFW 1
#ifdef USE_GLFW
#include "GLFW_EngineCore.h"
#endif

#include <memory>
#include <vector>
#include <algorithm>

#include "Game.h"
#include "DefaultGame.h"

#include "MemoryManager.h"

#include "DoublyLinkedList.h"
#include "Vector.h"
#include "HashMap.h"
#include "MyTestClass.h"

#include <iostream>
#include <string>


int main(int p_Argc, char* p_Argv[]) {
	MemoryManagerInstance;

	// Hash Map Test 1:
	unsigned int hasMapSize = 100;
	HashMap<std::string, MyTestClass> myHashMap(hasMapSize);










	// Vector Test 1:
	unsigned int vectorSize = 10u;
	CC::Vector<unsigned int> myVector(vectorSize);

	vectorSize *= 2u;
	for(unsigned int i = 0u; i <= vectorSize; ++i) {
		myVector.PushBack(i);
	}

	myVector.Remove((unsigned int)(vectorSize / 2u));

	myVector.PushBack(255u);

	for(unsigned int i = 0u; i <= vectorSize; ++i) {
		if(myVector.At(i) != nullptr)
			std::cout << myVector[i] << "  \t";
	}

	myVector.PushBack(255u);
	std::cout << myVector[myVector.NumberOfElements() - 1] << std::endl;

	std::cout << std::endl << std::endl;
	if(true) {
		// Vector Test 2:
		unsigned int newVectorSize = 10u;
		CC::Vector<MyTestClass> myNewVector(newVectorSize);

		MyTestClass instance;
		for(unsigned int i = 0u; i < newVectorSize; ++i) {
			myNewVector.PushBack(instance);
		}

		myNewVector.Remove((unsigned int)(newVectorSize / 2u));

		myNewVector.PushBack(instance);

		for(unsigned int i = 0u; i < newVectorSize; ++i) {
			if(myNewVector.At(i) != nullptr)
				std::cout << myNewVector[i];
		}
	}
	// Vector Test 3:
	unsigned int vectorSize2 = 10u;
	CC::Vector<unsigned int> myVector2(vectorSize2);
	for(unsigned int i = 0u; i < vectorSize2; ++i) {
		myVector2.PushBack(i);
	}

	for(unsigned int i = 0u; i < vectorSize2; ++i) {
		if(myVector2.At(i) != nullptr)
			std::cout << myVector2[i] << "  \t";
	}
	std::cout << std::endl << std::endl;

	/*
	std::shared_ptr<IEngineCore> engineCore;

#ifdef USE_GLFW
	 engineCore = std::make_shared<GLFW_EngineCore>();
#endif

	if (!engineCore->InitWindow(800, 600, "GameEngine"))
		return -1;

	std::shared_ptr<Game> game = std::make_shared<DefaultGame>();
	engineCore->RunEngine(game);
	*/

	std::cout << std::endl << std::endl << std::endl << std::endl;
	system("pause");
	return 0;
}