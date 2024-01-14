#include<bits/stdc++.h>
using namespace std;

int generateRandomNumber(int min, int max) 
{
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 gen(static_cast<unsigned>(seed));
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(gen);
}


void drawBipartiteGraph(vector<vector<int>> adjacencyMatrix,vector<int> partitionX) 
{    
    ofstream out("bipartite_graph.dot");
    // Generate DOT file header
    out << "graph bipartite_graph {\n";
    out << "  rankdir=LR;\n";

    //DOT file
    // Create nodes
    for (int i = 0; i < adjacencyMatrix.size(); ++i) {
        out << "  " << i << " [label=\"" << i << "\", color=" << (partitionX[i] == 1 ? "red" : "blue") << "];\n";
    }
    // Create edges
    for (int i = 0; i < adjacencyMatrix.size(); ++i) 
    {
        for (int j = i + 1; j < adjacencyMatrix[i].size(); ++j) 
        {
            if (adjacencyMatrix[i][j] == 1) 
            {
                out << "  " << i << " -- " << j << ";\n";
            }
        }
    }
    out << "}\n";
    out.close();

    // Display the graph
    system("dot -Tpng bipartite_graph.dot -o bipartite_graph.png");
    system("start bipartite_graph.png");  // Open the generated image file
}

void dfs(vector<vector<int>>& adjMatrix, vector<bool>& visited, int vertex) {
    visited[vertex] = true;
    for (int i = 0; i < adjMatrix.size(); ++i) {
        if (adjMatrix[vertex][i] == 1 && !visited[i]) {
            dfs(adjMatrix, visited, i);
        }
    }
}

bool connected(vector<vector<int>>& adjMatrix) {
    int vertices = adjMatrix.size();
    vector<bool> visited(vertices, false);

    int startVertex = -1;
    for (int i = 0; i < vertices; ++i) {
        if (count(adjMatrix[i].begin(), adjMatrix[i].end(), 1) > 0) {
            startVertex = i;
            break;
        }
    }

    if (startVertex == -1) 
    {
        return true;
    }

    dfs(adjMatrix, visited, startVertex);
    return all_of(visited.begin(), visited.end(), [](bool v) { return v; });
}

vector<vector<int>> create_adj(vector<vector<int>> mat)
{
    // create a adj matrix
    int n = mat.size() + mat[0].size();
    vector<vector<int>>Adjmat( n , vector<int>(n , 0));

    int mat_row = mat.size();
    int mat_col = mat[0].size();
    for(int i = 0 ; i < mat_row ; i++)
    {
        for(int j = mat_row ; j < n ; j++)
        {
            Adjmat[i][j] = mat[i][j - mat_row];
        }
    }
    for(int i = 0 ; i < mat_row ; i++ )
    {
        for(int j = mat_row ;  j < n ; j++)
        {
            Adjmat[j][i] = Adjmat[i][j]; 
        }
    }


    for(int i = 0 ; i < n ; i++)
    {
        for(int j = 0 ; j < n ; j++)
        {
            cout<<Adjmat[i][j]<<" ";
        }
        cout<<"\n";
    }
    return Adjmat;
}


pair<int , int> call_create_edges(int b)
{
        int lower_limit = generateRandomNumber(0 , b-1);
        int upper_limit = generateRandomNumber(0 , b-1);

        if( upper_limit == 0 || lower_limit > upper_limit )  return call_create_edges(b);
    
        return {lower_limit , upper_limit};
}


vector<vector<int>> create_graph(int X , int Y)
{
    // x == row // y == col
    vector<vector<int>>mat( X, vector<int> ( Y , 0 ));
    int row = X ;
    int col = Y ;
    
    //X have alone OR Y have alone vertex 
    if(Y == 1 || X == 1)
    {      
        for(int i = 0 ; i < row ; i++)
        {
            for(int j = 0 ; j < col ; j++)
            {
                mat[i][j] = 1;
            }   
        }
    }
    //normal 
    else
    {
        for(int i = 0 ; i < row ; i++)
        {
            pair<int , int> edges = call_create_edges(col);
            //cout<<edges.first << edges.second <<endl; 
            for(int j = 0 ;  j < col ; j++)
            {
                if( j >= edges.first and j <= edges.second)
                {
                    mat[i][j] = 1;
                }
            }  
        }
    }

    vector<vector<int>>ADJ = create_adj(mat);

    // check it is connected or not
    if(!connected(ADJ))
    {
        mat = create_graph(X,Y);
        return mat;
    }

    //DRAW
    int mat_row = mat.size();
    int mat_col = mat[0].size();
    int n = mat_col + mat_row;
    // 1 for set Y(RED)
    // 0 for set X(BLUE)
    vector<int>parts(n , 1);
    for(int i = 0  ; i < mat_row ; i++)
    {
        if(i < mat_row) parts[i] = 0;
    }
    drawBipartiteGraph(ADJ,parts);
    return mat;
}

int main()
{   
    int n_o_v;
    cout<<"Enter number of vertices \n";
    cin>>n_o_v;
    for( int i = 0 ; i < 10 ; i++)
    {
    int size_of_X = generateRandomNumber(1 , n_o_v-1 ) ;
    int size_of_Y = n_o_v - size_of_X ;
    cout<<"Size of set X "<<size_of_X <<" Size of set Y "<<size_of_Y;
    //will are making graph according to set X
    
    //create graph
    vector<vector<int>> mat = create_graph(size_of_X , size_of_Y );
    
    // cout<<"\nMATRIX\n";
    // for(int i=  0 ; i< mat.size() ; i++)
    // {
    //     for(int j = 0 ; j < mat[0].size() ; j++)
    //     {
    //         cout<<mat[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }
    }
    return 0;
}