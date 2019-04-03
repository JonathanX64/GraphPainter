//
//  Graph.hpp
//  course_work
//
//  Created by Jonathan Ostrovsky on 12/10/17.
//  Copyright © 2017 Jonathan Ostrovsky. All rights reserved.
//

#ifndef Graph_hpp
#define Graph_hpp

#include <fstream>
#include <vector>
#include <set>

/**
 * Contains undirected graph
 */
class Graph {
    int V;
    std::vector<std::set<int> > adj;
    std::ofstream out_graph;
    std::vector<int> result;

public:
    Graph(int V, const std::string& fname);
    ~Graph();

    void addEdge(int v, int w);
    std::string colorGenerator(int number);
    int greedyColoring();
    int bruteColoring();
    int checkColors(std::vector<int> colors);
    int countColors();
    int colorer(int chromatic);

    void output();
    void TwoTasksOneFunc(const std::string& name);
};

#endif /* Graph_hpp */

