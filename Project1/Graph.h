/*
This Header file  defines a namespace "graph" that will 
include utility functions related to graphs 

***History***
*
Date        Description
------------------------
02/13/23       Initial

*/
#pragma once


#include <fstream>
#include <iostream> 
#include <memory>
#include <vector>
#include <sstream>

using namespace std;

namespace graph
{
    template<class idType>
    struct Vertex;

    using VertexPtr = std::unique_ptr<Vertex<int>>;
    using Graph     = std::vector<std::vector<VertexPtr>>;

    void print(const Graph& graph);
    Graph createGraphFromFile(const char* fileName, char delim = ',');


    // Struct Vertex
    //   Represents a vertex 
    //   in a weighed graph
    template<class idType>
    struct Vertex
    {
        Vertex() : id(-1), weight(0) {}

        Vertex(idType id_, int weight_)
            : id(id_), weight(weight_) {}

        idType id;
        int weight;
    };

 

    //-----------------------------------------------------------------
    // Function: createGraphFromFile
    // Input:
    //     stringType fileName
    // Output   
    //     GraphType adjacency list 
    // Description:
    //     This function reads a text file that contains data in the specified format and generates an adjacency list 
    //     representation of a graph. It reads each line of the file and creates a new vertex in the graph with the 
    //     vertex's name specified in the line's first element. The remaining elements in the line are interpreted as 
    //     neighbors of the vertex, along with the corresponding edge weight. The generated Graph is returned. If the 
    //     file cannot be opened, an empty Graph is returned. If the format of the data is invalid, the function may 
    //     print an error message and return an incomplete Graph.
    // Assumptions: 
    //     1. Each line in the given text file lists data in the following format:
    //        VertexName neighbor_1,edge_1  neighbor_2,edge_2  {...}  neighbor_N,edge_N
    //        of vertex_x's neighbors {1,2,...,i}$
    //     2. The data is valid 
    //    #TO-DO Handle the cases when this assumption is not true 
    //    #TO-DO Add error handling codes
    Graph createGraphFromFile(const char* fileName, char delim)
    {
        Graph graph;

        std::ifstream inData;
        inData.open(fileName, std::fstream::in);

        if (!inData.is_open())
        {
            cout << "error opening file" << endl;
            return graph;
        }
        string line;
        while (getline(inData, line))
        {
            std::istringstream neighborParser(line);

            int vertexIdThrowaway;
            neighborParser >> vertexIdThrowaway;

            graph.emplace_back(vector<VertexPtr>());
            while (neighborParser)
            {
                VertexPtr neighbor = make_unique<Vertex<int>>();
                neighborParser >> neighbor->id;
                if (neighborParser && neighborParser.peek() != delim)
                {
                    cout << "Error: unexpected character:" << neighborParser.peek() << endl;
                    return graph;
                }
                else
                {
                    neighborParser.ignore();
                }
                neighborParser >> neighbor->weight;

                graph.back().emplace_back(std::move(neighbor));
            }
        }
        print(graph);
        inData.close();
        return graph;
    }

    // --------------------------------------------------------------------------------
    // Function: Print
    //     Prints the adjacency list representation of the given Graph object
    void print(const Graph& graph)
    {
        cout << "Print Adjacency List\n\n";
        cout << "Vertex ID\n" << "Edges[NeighborID,Weight]\n\n";

        int vertexNum = 0;
        for (auto vertexItr = graph.cbegin(); vertexItr != graph.cend(); vertexItr++)
        {
            cout << "V:" << vertexNum++ << endl;
            for (auto neighbor = vertexItr->cbegin(); neighbor != vertexItr->cend(); neighbor++)
            {
                cout << '[' << neighbor->get()->id << "," << neighbor->get()->weight << ']';
            }
            cout << endl << endl;
        }
    }
}

