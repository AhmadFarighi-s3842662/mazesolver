#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

#include "Types.h"
#include "Node.h"
#include "NodeList.h"
#include "PathSolver.h"
/*
 * My approach to this assignment was to dissect the provided pseudocode into
 * smaller methods relevant to the Pathsolver class. Then I implemented all
 * methods in Node, NodeList and PathSolver classes. I spent most of my time on
 * the forward search algorithm, and at first had difficulty working with parameter
 * passes and ideal return types for the helper functions I developed. I experienced
 * difficulty with finding the shortest estimated distance and the filtered openlist
 * was the solution to those issues (infinite loops). Initially, my getPath found
 * several nodes with the same distance travelled which would cause odd paths to
 * be printed by printStdout. To combat this, I wrote a conditional statement to
 * ensure only the first node with a certain distance travelled was considered.
 * This resulted in further odd behaviours which were 'somewhat' solved with more
 * conditions which decided which node to add when the prior condition was not 
 * satisfied. This resolved an infinite loop issue in getPath but the resulting
 * algorithm is not 100% correct in its implementation, as evidenced by the odd
 * result of test3.
 * 
 * Unfortunately, I have not been able to implement milestone 4 due to poor time
 * management. :(
 */

// Helper test functions
void testNode();
void testNodeList();

// Read a environment from standard input.
void readEnvStdin(Env env);

// Print out a Environment to standard output with path.
// To be implemented for Milestone 3
void printEnvStdout(Env env, NodeList* solution);


int main(int argc, char** argv){
    // THESE ARE SOME EXAMPLE FUNCTIONS TO HELP TEST YOUR CODE
    // AS YOU WORK ON MILESTONE 2. YOU CAN UPDATE THEM YOURSELF
    // AS YOU GO ALONG.
    // COMMENT THESE OUT BEFORE YOU SUBMIT!!!
    // std::cout << "TESTING - COMMENT THE OUT TESTING BEFORE YOU SUBMIT!!!" << std::endl;
    // testNode();
    // testNodeList();
    // std::cout << "DONE TESTING" << std::endl << std::endl;

    // Load Environment 
    Env env;
    readEnvStdin(env);
    
    // Solve using forwardSearch
    // THIS WILL ONLY WORK IF YOU'VE FINISHED MILESTONE 2
    PathSolver* pathSolver = new PathSolver();
    pathSolver->forwardSearch(env);

    NodeList* exploredPositions = nullptr;
    exploredPositions = pathSolver->getNodesExplored();

    // Get the path
    // THIS WILL ONLY WORK IF YOU'VE FINISHED MILESTONE 3
    NodeList* solution = pathSolver->getPath(env);

    printEnvStdout(env, solution);

    delete pathSolver;
    delete exploredPositions;
    delete solution;

}

void readEnvStdin(Env env) {
    char p1[ENV_DIM];
    int row = 0;
    while (!std::cin.eof()) {
        std::cin >> p1;
        for (int col = 0; col < ENV_DIM; ++col) {
            //assign environment element to char at matching index position of char string
            env[row][col] = p1[col];
        }
        ++row;
    }
}

void printEnvStdout(Env env, NodeList* solution) {
    for (int i = 0; i < solution->getLength() - 1; ++i) {
        if (solution->getNode(i)->getDistanceTraveled() != 0) {
            if (solution->getNode(i)->getRow() 
                == solution->getNode(i + 1)->getRow() + 1) {
                    env[solution->getNode(i)->getRow()][solution->getNode(i)->getCol()] 
                    = SYMBOL_UP;
            } else if (solution->getNode(i)->getRow() 
                == solution->getNode(i + 1)->getRow() - 1) {
                    env[solution->getNode(i)->getRow()][solution->getNode(i)->getCol()] 
                    = SYMBOL_DOWN;
            } else if (solution->getNode(i)->getCol() 
                == solution->getNode(i + 1)->getCol() - 1) {
                    env[solution->getNode(i)->getRow()][solution->getNode(i)->getCol()] 
                    = SYMBOL_RIGHT;
            } else if (solution->getNode(i)->getCol() 
                == solution->getNode(i + 1)->getCol() + 1) {
                    env[solution->getNode(i)->getRow()][solution->getNode(i)->getCol()] 
                    = SYMBOL_LEFT;
            }
        }       
    }
    for (int row = 0; row < ENV_DIM; ++row) {
        for (int col = 0; col < ENV_DIM; ++col) {
            std::cout << env[row][col];
        }
    std::cout << std::endl;
    } 
}

void testNode() {
    std::cout << "TESTING Node" << std::endl;

    // Make a Node and print out the contents
    Node* node = new Node(1, 1, 2);
    std::cout << node->getRow() << ",";
    std::cout << node->getCol() << ",";
    std::cout << node->getDistanceTraveled() << std::endl;
    delete node;

    // hange Node and print again
    node = new Node(4, 2, 3);
    std::cout << node->getRow() << ",";
    std::cout << node->getCol() << ",";
    std::cout << node->getDistanceTraveled() << std::endl;
    delete node;
}

void testNodeList() {
    std::cout << "TESTING NodeList" << std::endl;

    // Make a simple NodeList, should be empty size
    NodeList* nodeList = new NodeList();
    std::cout << "NodeList size: " << nodeList->getLength() << std::endl;

    // Add a Node to the NodeList, print size
    Node* b1 = new Node(1, 1, 1);
    nodeList->addElement(b1);
    std::cout << "NodeList size: " << nodeList->getLength() << std::endl;

    // Add second Nodetest
    Node* b2 = new Node(0, 0, 1);
    nodeList->addElement(b2);
    std::cout << "NodeList size: " << nodeList->getLength() << std::endl;

    // Test Get-ith - should be 0,0,1
    Node* getB = nodeList->getNode(1);
    std::cout << getB->getRow() << ",";
    std::cout << getB->getCol() << ",";
    std::cout << getB->getDistanceTraveled() << std::endl;

    // Print out the NodeList
    for (int i = 0; i < nodeList->getLength(); ++i) {
        Node* nodePtr = nodeList->getNode(i);
        std::cout << nodePtr->getRow() << ",";
        std::cout << nodePtr->getCol() << ",";
        std::cout << nodePtr->getDistanceTraveled() << std::endl;
    }

    delete b2;
    delete b1;
    delete nodeList;
}