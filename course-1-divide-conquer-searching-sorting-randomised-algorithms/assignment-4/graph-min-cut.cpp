/*This version takes into consideration any existing self loops in the input graph,
unlike the assignment input which did not have any self loops in the input graph*/
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <random>
#include <cmath>

using namespace std;

int pickRandomVertex(const int range_from, const int range_to)
{
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int>  distr(range_from, range_to);
    return distr(generator);
}

void pickEdgeRandomly(std::vector<vector<int>> &input, std::vector<int> &verticesToMerge)
{
    /*choosing an edge at random; first a vertex is chosen randomly &
     then its adjacent vertex is chosen randomly;
     returns actual number of the edge vertices and not their index value in the vector array;
     returns vertices in ascending order (edge[0] < edge[1])*/
    bool isVertexMergedAlready = true;
    while (isVertexMergedAlready)
    {
        const int range_from  = 0;
        const int range_to = (const int) (input.size()-1);
        int randomEdgeHead = pickRandomVertex(range_from,range_to);

        if(input[randomEdgeHead].size() == 1 )
        {
            //keep going in while loop
        }
        else
        {
            const int range_from  = 1;
            const int range_to = (const int)((input[randomEdgeHead]).size()-1);
            int randomEdgesource = pickRandomVertex(range_from,range_to);
            
            if ((input[randomEdgeHead])[randomEdgesource] > (input[randomEdgeHead])[0])
            {
                verticesToMerge.push_back((input[randomEdgeHead])[0]);
                verticesToMerge.push_back((input[randomEdgeHead])[randomEdgesource]);
                isVertexMergedAlready = false;
            }
            else if((input[randomEdgeHead])[randomEdgesource] < (input[randomEdgeHead])[0])
            {
                verticesToMerge.push_back((input[randomEdgeHead])[randomEdgesource]);
                verticesToMerge.push_back((input[randomEdgeHead])[0]);
                isVertexMergedAlready = false;
            }
            else
            {
                //self loop; ignore
            }
        }
    }
}


void contractVertices(std::vector<vector<int>> &input,long vertexRemainingCount)
{
    
    if(vertexRemainingCount <= 2)
    {
        return;
    }
    std::vector<int> verticesToMerge;
    pickEdgeRandomly(input,verticesToMerge);
    
    
    //remove existing self loops and self loops that can be formed when merging this vertex
    for(std::vector<int>::iterator it = (input[verticesToMerge[1]-1]).begin()+1; it < (input[verticesToMerge[1]-1]).end();)
    {
        
        if(verticesToMerge[0] == (*it) || verticesToMerge[1] == (*it))
        {
            it = (input[verticesToMerge[1]-1]).erase(it);
        }
        else
        {
            ++it;
        }
    }
    
    //update the new head list to remove all the self-loops, existing and from vertices merge
    for(std::vector<int>::iterator it = (input[verticesToMerge[0]-1]).begin() + 1; it < (input[verticesToMerge[0]-1]).end();)
    {
        if(verticesToMerge[1] == (*it) || verticesToMerge[0] == (*it))
        {
            it = (input[verticesToMerge[0]-1]).erase(it);
        }
        else
        {
            ++it;
        }
    }
    
    //List of adjacent vertices of the vertex to be merged; point them to new vertex
    std::vector<int> verticesToUpdate = input[verticesToMerge[1]-1];
    
    //copy the merging vertex's list to the head
    if((input[verticesToMerge[1]-1]).size()>1)
    {
        input[verticesToMerge[0]-1].insert(input[verticesToMerge[0]-1].end(), input[verticesToMerge[1]-1].begin()+1,input[verticesToMerge[1]-1].end());
    }
    
    //Remove all adjacent vertices of the merging vertex
    input[verticesToMerge[1]-1].erase((input[verticesToMerge[1]-1]).begin() + 1, (input[verticesToMerge[1]-1]).end());
    
      
    //now traverse the merged vertex's original list to update the adjacency list
    for (std::vector<int>::iterator it = verticesToUpdate.begin()+1; it < verticesToUpdate.end();++it)
    {
        for (std::vector<int>::iterator updateMergedVertexValue = (input[(*it)-1]).begin(); updateMergedVertexValue < (input[(*it)-1]).end(); ++updateMergedVertexValue)
        {
            if(verticesToMerge[1] == (*updateMergedVertexValue))
            {
                (*updateMergedVertexValue) = verticesToMerge[0];
            }
        }
    }
    
    vertexRemainingCount = vertexRemainingCount - 1;  
    contractVertices(input,vertexRemainingCount);
    
}

int main(int argc, const char * argv[]) {
    //read input file
    std::string fileName = "/Users/RohiniPangrikar 1/Desktop/kragerMinCut.txt";
    std::ifstream in(fileName);
    if(!in)
    {
        std::cout << "cannot open input file:" << fileName << endl;
        return 0;
    }
    std::vector<std::vector<int>> inputAdjacentList;
    std::string input;
    while (std::getline(in,input))
    {
        std::istringstream is(input);
        inputAdjacentList.push_back(std::vector<int> (std::istream_iterator<int>(is),std::istream_iterator<int>()));
    }
    long totalIterationCounts = inputAdjacentList.size();
    long lowestMinCut = totalIterationCounts*totalIterationCounts;
    
    // Choose the iterations to be a large number; I tested with totalIterationCounts & totalIterationCounts^2
    for(int i = 0; i < totalIterationCounts*totalIterationCounts; i++)
    {
        std::vector<std::vector<int>> currentIterationAdjacencyList(inputAdjacentList);
        contractVertices(currentIterationAdjacencyList, currentIterationAdjacencyList.size());
        long minCut = 0;
        for(std::vector<std::vector<int>>::iterator it = currentIterationAdjacencyList.begin(); it < currentIterationAdjacencyList.end();++it)
        {
            if((*it).size() > 1)
            {
                minCut = (*it).size()-1;
                break;
            }
        }
        if(lowestMinCut > minCut)
        {
            lowestMinCut = minCut;
        }    
    }
    
    cout << "MinCut is: " << lowestMinCut << endl;
    
    return 0;
}
