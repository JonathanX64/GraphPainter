//
//  Graph.cpp
//  course_work
//
//  Created by Jonathan Ostrovsky on 12/10/17.
//  Copyright © 2017 Jonathan Ostrovsky. All rights reserved.
//

#include "Graph.hpp"
#include <iostream>
#include <exception>
#include <ctime>
#include <algorithm>

/**
 * Constructor
 * @param V amount of graph' vertexes
 * @param fname name of graphviz file to be created
 */
Graph::Graph(int V, const std::string& fname)
{
    this->V = V;

    adj.reserve(V);
    for (int i = 0; i < V; i++) {
        std::set<int> temp;
        adj.push_back(temp);

        result.push_back(0);
    }

    out_graph.open(fname, std::ios::binary | std::ios::out);
    out_graph << "graph graphname { \n";
}

/**
 * Destructor\n
 * Nobody calls them manually anyway\n
 * Writes last pieces of graph to file and closes stream
 */
Graph::~Graph()
{
    out_graph << "}" << std::endl;
    out_graph.close();
}

/**
 Adds edge to a graph\n
 Ignores already added edges and loops

 @param v node to link
 @param w node to link
 */
void Graph::addEdge(int v, int w)
{
    if (v == w) {
        throw "No loops are allowed; continuing execution...";
        return;
    }

    if (v >= V || w >= V) {
        throw "Trying to access non-existent memory...";
        return;
    }

    std::set<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i) {
        if (*i == w) {
            //std::cout << "they're already connected" << std::endl;
            return;
        }
    }

    adj[v].insert(w);
    adj[w].insert(v); // Note: the graph is undirected
    out_graph << '\t' << v << "--" << w << ';' << std::endl;
}

/**
 Runs brute force graph coloring algorithm within current graph

 @return chromatic number
 */
int Graph::bruteColoring()
{
    int i = 1;
    while (colorer(i)) {
        //std::cout << i << " colors — not passed\n";
        i++;
    }
    output();
    return countColors();
}

/**
 Function that's being used within brute force coloring algoritm\n
 Checks if this graph can be colored with just X colors
 
 @param chromatic chromatic number to check
 @return 0 if solution is found, 1 if it's not
 */
int Graph::colorer(int chromatic)
{
    std::vector<int> res;
    for (int i = 0; i < V; i++) {
        res.push_back(i % chromatic);
    }
    do {
        if (checkColors(res) > 0) {
            result = res;
            return 0;
        }
    } while (std::next_permutation(res.begin(), res.end()));
    return 1;
}

/**
 Function that's being used within "colorer"

 @param colors array of graph colors to check; could be internal Graph's result or sometthing else.
 @return 0 if these colors are not a solution, 1 if they are
 */
int Graph::checkColors(std::vector<int> colors)
{
    std::set<int>::iterator i;
    for (int u = 0; u < V; u++) {
        for (i = adj[u].begin(); i != adj[u].end(); ++i) {
            if (colors[u] == colors[*i]) {
                return 0;
            }
        }
    }

    return 1;
}

/**
 Runs brute force graph coloring algorithm within current graph

 @return amount of colors used, which is usually close to the chromatic number
 */
int Graph::greedyColoring()
{
    // Assign the first color to first vertex
    result[0] = 0;

    // Initialize remaining V-1 vertices as unassigned
    for (int u = 1; u < V; u++)
        result[u] = -1; // no color is assigned to u

    // A temporary array to store the available colors. True
    // value of available[cr] would mean that the color cr is
    // assigned to one of its adjacent vertices
    std::vector<bool> available;
    available.reserve(V);
    for (int cr = 0; cr < V; cr++)
        available.push_back(false);

    // Assign colors to remaining V-1 vertices
    for (int u = 1; u < V; ++u) {
        // Process all adjacent vertices and flag their colors
        // as unavailable
        std::set<int>::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
            if (result[*i] != -1)
                available[result[*i]] = true;

        // Find the first available color
        int cr;
        for (cr = 0; cr < V; cr++)
            if (available[cr] == false)
                break;

        result[u] = cr; // Assign the found color

        // Reset the values back to false for the next iteration
        for (i = adj[u].begin(); i != adj[u].end(); i++)
            if (result[*i] != -1)
                available[result[*i]] = false;
    }

    output();
    return countColors();
}

/**
 Simple function to check how much colors are used in graph currently

 @return amount of colors, whereas 1 = 1 color used, 2 = 2, etc.
 */
int Graph::countColors()
{
    int ret = 0;
    for (int i = 0; i < V; i++) {
        if (result[i] > ret)
            ret = result[i];
    }
    return ret + 1;
}

/**
 Primitive function that puts colors of vertexes on screen\n
 Also puts color into our graphviz file
 */
void Graph::output()
{
    for (int u = 0; u < V; u++) {
        std::cout << "Vertex " << u << " --->  Color " << result[u] << std::endl;
        out_graph << '\t' << u << ' ' << "[color=" << colorGenerator(result[u]) << "];\n";
    }
}

/**
 Function that runs greedy algorithm, then brute force algorithm, then shows us how much time did they take to complete

 @param name name of graph that features within this output. Does not actually affects graph
 */
void Graph::TwoTasksOneFunc(const std::string& name)
{
    std::cout << "Greedy coloring of " << name << std::endl;
    clock_t start_s = clock();
    std::cout << this->greedyColoring() << " colors used" << std::endl;
    clock_t stop_s = clock();
    std::cout << "time spent: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) << std::endl;
    std::cout << "Slow coloring of " << name << std::endl;
    start_s = clock();
    std::cout << this->bruteColoring() << " colors used" << std::endl;
    stop_s = clock();
    std::cout << "time spent: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) << std::endl;
    std::cout << std::endl;
}

/**
 Primitive function to convert color data of vertexes into distinguishable colors

 @param number color data of some vertex
 @return string with color that corresponds to this number
 */
std::string Graph::colorGenerator(int number)
{
    int amount = 8;
    std::string colors[amount];
    colors[0] = "blue";
    colors[1] = "magenta";
    colors[2] = "red";
    colors[3] = "green";
    colors[4] = "yellow";
    colors[5] = "violet";
    colors[6] = "black";
    colors[7] = "grey";

    return colors[number % amount];
}
