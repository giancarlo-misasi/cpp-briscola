//
// Created by Gianc on 2022-12-24.
//

#ifndef BRISCOLA_GRAPHVIZ_H
#define BRISCOLA_GRAPHVIZ_H

#include "mcts.h"
#include <fstream>
#include <vector>

using namespace std;

/**
 * Function writing out a Graphviz .dot file of an MCTS tree. Useful for debugging.
 * @param root Root of the MCTS tree
 * @param filename Filename to write the .dot file to
 */
template <class T, class A, class G>
void writeDotFile(Node<T, A, G>* root, const char* filename)
{
    ofstream dot;
    dot.open(filename);

    // write header
    dot << "digraph MCTS {" << endl;

    vector<Node<T, A, G>*> fringe;
    fringe.push_back(root);

    // Do a breadth first search through the nodes and write the Nodes and their Actions one by one.
    while (!fringe.empty()) {
        Node<T, A, G>* current = fringe[0];
        fringe.erase(fringe.begin());

        // Write out Node
        dot << current->getID() << " [label=\"" << current->getData() << "\\nVisits: " << current->getNumVisits()
            << "\\nScore: " << current->getAvgScore() << "\"];" << endl;

        // Write out Action as edge
        if (current->getID() != root->getID()) {
            dot << current->getParent()->getID() << " -> " << current->getID() << "[label=\"" << current->getAction()
                << "\"];" << endl;
        }

        for (auto &child : current->getChildren()) {
            fringe.emplace_back(child.get());
        }
    }

    dot << "}" << endl;

    dot.close();
}

#endif //BRISCOLA_GRAPHVIZ_H
