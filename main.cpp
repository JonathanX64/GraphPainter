//
//  main.cpp
//  course_work
//
//  Created by Jonathan Ostrovsky on 10/28/17.
//  Copyright © 2017 Jonathan Ostrovsky. All rights reserved.
//

//Реализовать переборы и жадный алгоритмы решения задачи о раскраске графа. Программа должна  определять хромотическое число и записывать раскраску файл. Вход --- матрица смежности, выход --- граф в формате graphviz.

//Литература:
//a. Ахо, Ульман, Хопкрофт, “Структуры данных и алгоритмы”

#include <iostream>
#include <sstream>
#include <string>
#include "Graph.hpp"

int main(int argc, const char* argv[])
{
    if (argc == 1) {
        Graph g1(5, "graph.gv");
        g1.addEdge(0, 1);
        g1.addEdge(0, 2);
        g1.addEdge(1, 2);
        g1.addEdge(1, 3);
        g1.addEdge(2, 3);
        g1.addEdge(3, 4);
        try {
            g1.addEdge(3, 3);
        } catch (const char* err) {
            std::cerr << err << std::endl;
        }

        Graph g2(6, "graph2.gv");
        g2.addEdge(0, 1);
        g2.addEdge(0, 2);
        g2.addEdge(1, 2);
        g2.addEdge(1, 4);
        g2.addEdge(2, 4);
        g2.addEdge(4, 3);
        g2.addEdge(4, 5);
        g2.addEdge(3, 5);
        g2.addEdge(2, 5);

        int size = 11;
        Graph g3(size, "graph3.gv");
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (i != j) {
                    g3.addEdge(i, j);
                }
            }
        }

        g1.TwoTasksOneFunc("graph 1");
        g2.TwoTasksOneFunc("graph 2");
        g3.TwoTasksOneFunc("graph 3");
    } else {
        // open file
        std::ifstream matrix;
        matrix.open(argv[1]);

        // learn amount of vertexes
        int v, j = 0, k;
        std::string line;
        std::getline(matrix, line);
        line.erase(0, 2);
        v = std::stoi(line);

        // and create the graphs
        Graph g1(v, "greedy_colored.gv");
        Graph g2(v, "brute_forced.gv");

        while (std::getline(matrix, line)) {
            k = 0;
            for (int i = 1; i < line.length(); i += 2) {
                if (line[i] == '1') {
                    try {
                        g1.addEdge(j, k);
                        g2.addEdge(j, k);
                    } catch (const char* err) {
                        std::cerr << err << std::endl;
                    }
                    k++;
                } else if (line[i] == '0') {
                    k++;
                }
            }
            j++;
        }

        std::cout << "Greedy coloring of your graph:" << std::endl;
        clock_t start_s = clock();
        std::cout << g1.greedyColoring() << " colors used" << std::endl;
        clock_t stop_s = clock();
        std::cout << "time spent: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) << "\n\n";
        std::cout << "Slow coloring of your graph:" << std::endl;
        start_s = clock();
        std::cout << g2.bruteColoring() << " colors used" << std::endl;
        stop_s = clock();
        std::cout << "time spent: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) << std::endl;
    }
    
    return 0;
}
