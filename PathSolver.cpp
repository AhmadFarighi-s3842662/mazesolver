#include "PathSolver.h"
#include <iostream>

PathSolver::PathSolver() {
    this->nodesExplored = new NodeList();
}

PathSolver::~PathSolver() {
    delete nodesExplored;
}

void PathSolver::forwardSearch(Env env) {
    // initialise start and finish nodes
    NodeList* openList = new NodeList();
    Node* startNodePtr = new Node(getSymbolRow(SYMBOL_START, env),
                             getSymbolCol(SYMBOL_START, env), 0);
    Node* goalNodePtr = new Node(getSymbolRow(SYMBOL_GOAL, env),
                             getSymbolCol(SYMBOL_GOAL, env), 0);
    openList->addElement(startNodePtr);
    Node* node = startNodePtr;
    int count = 0;

    do {
        NodeList* filteredOpenList = new NodeList();
        for (int i = 0; i < openList->getLength(); ++i) {
            // fill list with nodes contained in openlist that are not present in closedlist
            if (!checkIfInNodeList(openList->getNode(i), nodesExplored)) {
                filteredOpenList->addElement(openList->getNode(i));
            }
        }
        // reassign current node to closest node
        node = getClosestNodeToGoal(goalNodePtr, filteredOpenList);
        NodeList* reachableNodes = new NodeList();
        // add reachable positions to reachableNodes nodelist
        availablePositions(node, reachableNodes, env); 
        for (int i = 0; i < reachableNodes->getLength(); ++i) {
            if (!checkIfInNodeList(reachableNodes->getNode(i), openList)) {
                // only add to openlist if not already present in openlist
                openList->addElement(reachableNodes->getNode(i));
            }
        }
        nodesExplored->addElement(node);
        ++count;
        delete reachableNodes;
        delete filteredOpenList;
    } while((node->getRow() != goalNodePtr->getRow()) || 
            (node->getCol() != goalNodePtr->getCol()));
    delete openList;
    delete startNodePtr;
    delete goalNodePtr;
}

NodeList *PathSolver::getNodesExplored()
{
    NodeList *nodesExploredCopy = new NodeList(*nodesExplored);
    return nodesExploredCopy;
}

NodeList* PathSolver::getPath(Env env) {
    NodeList* backTracedPath = new NodeList();
    // assign goalnode to final element in closedlist, assumes forward search is successful
    Node goalNode = Node(*nodesExplored->getNode(nodesExplored->getLength()-1));
    Node* node = &goalNode;
    backTracedPath->addElement(node);
    int count = 0;
    do {
        // iterates backwards
        for (int i = nodesExplored->getLength() - 1; i >= 0; --i) {
            // check if previous node in nodesexplored is neighbour and has 1 less distance travelled
            if (((nodesExplored->getNode(i)->getRow() == node->getRow() - 1 && 
            nodesExplored->getNode(i)->getCol() == node->getCol()) ||
                (nodesExplored->getNode(i)->getRow() == node->getRow() + 1 && 
                nodesExplored->getNode(i)->getCol() == node->getCol()) ||
                (nodesExplored->getNode(i)->getCol() == node->getCol() - 1 && 
                nodesExplored->getNode(i)->getRow() == node->getRow()) ||
                (nodesExplored->getNode(i)->getCol() == node->getCol() + 1 &&
                 nodesExplored->getNode(i)->getRow() == node->getRow()) ) 
                && (nodesExplored->getNode(i)->getDistanceTraveled() == 
                node->getDistanceTraveled() - 1)) {
                    //check if startnode or if node does not have the same distance travelled as 'next' node
                    if (i == 0 || nodesExplored->getNode(i)->getDistanceTraveled() != nodesExplored->getNode(i-1)->getDistanceTraveled()) {
                        backTracedPath->addElement(nodesExplored->getNode(i));

                    /*
                    * this is necessary to prevent infinite looping, compares loop count and if it exceeds number of nodes
                    * explored, take the current node regardless of its distance travelled
                    */
                    } else if (count >= nodesExplored->getLength())
                        backTracedPath->addElement(nodesExplored->getNode(i));
                }
        } count++;
        // reassign node to the last node in the nodeList for next iteration
        node = backTracedPath->getNode(backTracedPath->getLength() - 1);
    } while (node->getDistanceTraveled() != 0);
    NodeList* shortestPath = new NodeList();
    // reverse fill shortestPath to return path from start to end
    for (int i = backTracedPath->getLength() - 1; i >= 0; --i) {
        shortestPath->addElement(backTracedPath->getNode(i));
    }
    return shortestPath;
}

int PathSolver::getSymbolRow(char symbol, Env env) {
    int row;
    for (int i = 0; i < ENV_DIM; ++i) {
        for (int j = 0; j < ENV_DIM; ++j) {
            if (env[i][j] == symbol) {
                row = i;
            }
        }
    }
    return row;
}

int PathSolver::getSymbolCol(char symbol, Env env) {
    int col;
    for (int i = 0; i < ENV_DIM; ++i) {
        for (int j = 0; j < ENV_DIM; ++j) {
            if (env[i][j] == symbol) {
                col = j;
            }
        }
    }
    return col;
}

bool PathSolver::checkIfInNodeList(Node *node, NodeList *nodeList) {
    bool checkResult = false;
    if (nodeList->getLength() != 0) {
        for (int i = 0; i < nodeList->getLength(); ++i) {
            if ((node->getRow() == nodeList->getNode(i)->getRow()) &&
             (node->getCol() == nodeList->getNode(i)->getCol())) {
                checkResult = true;
            }
        }
    } 
    return checkResult;
}

void PathSolver::availablePositions(Node *node, NodeList *reachableNodes, Env env) {
    // checks reachable positions from any position other than edge positions
        if (env[node->getRow() - 1][node->getCol()] == SYMBOL_EMPTY ||
         env[node->getRow() - 1][node->getCol()] == SYMBOL_GOAL)
        {
            Node n1 = Node(node->getRow() - 1, node->getCol(), 
                        node->getDistanceTraveled() + 1);
            reachableNodes->addElement(&n1);
        }
        if (env[node->getRow() + 1][node->getCol()] == SYMBOL_EMPTY ||
         env[node->getRow() + 1][node->getCol()] == SYMBOL_GOAL)
        {
            Node n2 = Node(node->getRow() + 1, node->getCol(),
                         node->getDistanceTraveled() + 1);
            reachableNodes->addElement(&n2);
        }
        if (env[node->getRow()][node->getCol() - 1] == SYMBOL_EMPTY ||
         env[node->getRow()][node->getCol() - 1] == SYMBOL_GOAL)
        {
            Node n3 = Node(node->getRow(), node->getCol() - 1,
                         node->getDistanceTraveled() + 1);
            reachableNodes->addElement(&n3);
        }
        if (env[node->getRow()][node->getCol() + 1] == SYMBOL_EMPTY ||
         env[node->getRow()][node->getCol() + 1] == SYMBOL_GOAL)
        {
            Node n4 = Node(node->getRow(), node->getCol() + 1,
                         node->getDistanceTraveled() + 1);
            reachableNodes->addElement(&n4);
        }
}

Node* PathSolver::getClosestNodeToGoal(Node* goal, NodeList* nodeList) {
    // initialise as startnode
    Node* closestNode = nodeList->getNode(0);
    for (int i = 0; i < nodeList->getLength(); ++i) {
        if (nodeList->getNode(i)->getEstimatedDist2Goal(goal) < closestNode->getEstimatedDist2Goal(goal)) {
            closestNode = nodeList->getNode(i);
        }
    }
    return closestNode;
}
