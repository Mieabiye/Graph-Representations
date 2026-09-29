#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        cerr << "Error: wrong number of arguments.\n";
        cerr << "Use: \"Graph Representations.exe\" In.txt Matrix.txt List.txt\n";
        return 1;
    }

    string inputFileName = argv[1];
    string matrixFileName = argv[2];
    string listFileName = argv[3];

    ifstream inputFile(inputFileName);

    if (!inputFile.is_open())
    {
        cerr << "Error: cannot open input file: " << inputFileName << '\n';
        return 1;
    }

    string line;

    if (!getline(inputFile, line))
    {
        cerr << "Error: input file is empty.\n";
        return 1;
    }

    vector<string> vertices;
    istringstream firstLine(line);
    string name;

    while (firstLine >> name)
    {
        vertices.push_back(name);
    }

    if (vertices.empty())
    {
        cerr << "Error: no vertices found.\n";
        return 1;
    }

    int n = static_cast<int>(vertices.size());

    unordered_map<string, int> vertexIndex;

    for (int i = 0; i < n; ++i)
    {
        vertexIndex[vertices[i]] = i;
    }

    vector<vector<int>> matrix(n, vector<int>(n, 0));

    // This stores neighbors in the order that edges are read.
    vector<vector<int>> adjacencyList(n);

    string firstVertex;
    string secondVertex;

    while (getline(inputFile, line))
    {
        if (line.empty())
        {
            continue;
        }

        istringstream edgeLine(line);

        if (!(edgeLine >> firstVertex >> secondVertex))
        {
            cerr << "Error: invalid edge line: " << line << '\n';
            return 1;
        }

        if (vertexIndex.find(firstVertex) == vertexIndex.end())
        {
            cerr << "Error: unknown vertex: " << firstVertex << '\n';
            return 1;
        }

        if (vertexIndex.find(secondVertex) == vertexIndex.end())
        {
            cerr << "Error: unknown vertex: " << secondVertex << '\n';
            return 1;
        }

        int firstIndex = vertexIndex[firstVertex];
        int secondIndex = vertexIndex[secondVertex];

        // The graph is undirected.
        matrix[firstIndex][secondIndex] = 1;
        matrix[secondIndex][firstIndex] = 1;

        // Store edge order for the adjacency list.
        adjacencyList[firstIndex].push_back(secondIndex);
        adjacencyList[secondIndex].push_back(firstIndex);
    }

    inputFile.close();

    ofstream matrixFile(matrixFileName);

    if (!matrixFile.is_open())
    {
        cerr << "Error: cannot create: " << matrixFileName << '\n';
        return 1;
    }

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (j > 0)
            {
                matrixFile << ' ';
            }

            matrixFile << matrix[i][j];
        }

        matrixFile << '\n';
    }

    matrixFile.close();

    ofstream listFile(listFileName);

    if (!listFile.is_open())
    {
        cerr << "Error: cannot create: " << listFileName << '\n';
        return 1;
    }

    // Print neighbors in input-edge order.
    for (int i = 0; i < n; ++i)
    {
        listFile << vertices[i] << ":";

        for (int neighborIndex : adjacencyList[i])
        {
            listFile << " " << vertices[neighborIndex];
        }

        listFile << '\n';
    }

    listFile.close();

    cout << "Done successfully.\n";
    cout << "Created " << matrixFileName << '\n';
    cout << "Created " << listFileName << '\n';

    return 0;
}