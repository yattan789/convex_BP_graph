import random
import subprocess 
import os

graphviz_bin_path = 'C:\\Program Files\\Graphviz\\bin'
os.environ['PATH'] += os.pathsep + graphviz_bin_path

def generate_random_number(min_val, max_val):
    return random.randint(min_val, max_val)

def dfs(adj_matrix, visited, vertex):
    visited[vertex] = True
    for i in range(len(adj_matrix)):
        if adj_matrix[vertex][i] == 1 and not visited[i]:
            dfs(adj_matrix, visited, i)

def is_connected(adj_matrix):
    vertices = len(adj_matrix)
    visited = [False] * vertices

    start_vertex = -1
    for i in range(vertices):
        if sum(adj_matrix[i]) > 0:
            start_vertex = i
            break

    if start_vertex == -1:
        return True

    dfs(adj_matrix, visited, start_vertex)
    return all(visited)

def draw_bipartite_graph(adjacency_matrix, partition_x):
    
    with open("C:\\Users\\Sins\\Desktop\\sem4\\research\\coding\\bipartite_graph.dot", "w") as out:
        out.write("graph bipartite_graph {\n")
        out.write("  rankdir=LR;\n")

        # Create nodes
        for i in range(len(adjacency_matrix)):
            color = "red" if partition_x[i] == 1 else "blue"
            out.write(f'  {i} [label="{i}", color={color}];\n')

        # Create edges
        for i in range(len(adjacency_matrix[i])):
            for j in range(i, len(adjacency_matrix)):
                if adjacency_matrix[i][j] == 1:
                    out.write(f'  {i} -- {j};\n')

        out.write("}\n")
        return

def create_adj(mat):
    n = len(mat) + len(mat[0])
    adj_mat = [[0] * n for _ in range(n)]

    mat_row = len(mat)
    mat_col = len(mat[0])

    for i in range(mat_row):
        for j in range(mat_row, n):
            adj_mat[i][j] = mat[i][j - mat_row]

    for i in range(mat_row):
        for j in range(mat_row, n):
            adj_mat[j][i] = adj_mat[i][j]

    for row in mat:
        print(row)
    print("\n")
    for row in adj_mat:
        print(row)
    return adj_mat

def call_create_edges(b):
    lower_limit = generate_random_number(0, b - 1)
    upper_limit = generate_random_number(0, b - 1)

    if upper_limit == 0 or lower_limit > upper_limit:
        return call_create_edges(b)

    return lower_limit, upper_limit

def create_graph(X, Y):
    mat = [[0] * Y for _ in range(X)]
    row = X
    col = Y

    if Y == 1 or X == 1:
        for i in range(row):
            for j in range(col):
                mat[i][j] = 1
    else:
        for i in range(row):
            edges = call_create_edges(col)
            for j in range(col):
                if edges[0] <= j <= edges[1]:
                    mat[i][j] = 1

    adj = create_adj(mat)

    if not is_connected(adj):
        mat = create_graph(X, Y)
        return

    n = col + row
    partition = [1] * n
    for i in range(row):
        if i < row:
            partition[i] = 0

    draw_bipartite_graph(adj, partition)
    return adj

if __name__ == "__main__":

    size_of_X = int(input("Enter size set I:\n"))
    size_of_Y = int(input("Enter size set J:\n"))
    for i in range(1):

        print(f"Size of set X: {size_of_X} | Size of set Y: {size_of_Y}")
        
        adj = create_graph(size_of_X, size_of_Y)
        
        cpp_executable = 'C:\\Users\\Sins\\Desktop\\sem4\\research\\coding\\apply.exe'
        try:
            subprocess.run(cpp_executable, check=True)
        except subprocess.CalledProcessError as e:
            print(f"Error running given subprogram program: {e}")
        
        partition = [1] * (size_of_X + size_of_Y)
        for i in range(size_of_X):
            if i < size_of_X:
                partition[i] = 0
        want_to_delete = input("Y/N \n").lower()

        while( want_to_delete == 'y'):
            v = int(input("Enter Vertex number : \n"))
            if adj is not None:
                for i in range(len(adj)):
                    adj[i][v] = 0
                    adj[v][i] = 0
                draw_bipartite_graph(adj, partition)
                cpp_executable = 'C:\\Users\\Sins\\Desktop\\sem4\\research\\coding\\apply.exe'
                try:
                    subprocess.run(cpp_executable, check=True)
                except subprocess.CalledProcessError as e:
                    print(f"Error running given subprogram program: {e}")
                want_to_delete = input("Press Y for Resume N to end : \n").lower()
