#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <string.h>
#define MAX 11234
#define INF 2147483647 // Integer "ceil"
#define lli long long int

using namespace std;

int G[MAX][MAX], queue_aux[MAX], v, e, n, matrix_type, qt_digits; // n == number_of_vertix, if matrix_type == 0 is adjacency, else is incidence
vector<pair<long long int, pair<int, int>>> edge_list; // (weight, two vertices) of the edge
class Union_Find{
    private:
        vector<int> parent, rank;
    public:
        Union_Find(int v){
            rank.assign(v+1, 0);
            parent.assign(v+1, 0);
            for(int i = 0; i <= v; i++) parent[i] = i;
        }
        int find(int i){
            while(i != parent[i]) i = parent[i];
            return i;
        }
        bool is_same_set(int i, int j){
            return find(i) == find(j);
        }
        void union_set(int i, int j){
            if(is_same_set(i, j)) return;
            int x = find(i), y = find(j);
            if(rank[x] > rank[y]) parent[y] = x;
            else{
                parent[x] = y;
                if(rank[x] == rank[y]) rank[y]++;
            }
        }
};

// =============== <Function Prototype Area> ============= //
// #                   Arguments: N/A                    # //
// #                    Returns: N/A                     # //
// #      Description: This function clear console       # //
void clear();

// #            Arguments: An integer value              # //
// #    Returns: The number of digits of the argument    # //
// #    Description: This function count the number of   # //
// #                         digits of a integer value   # //
int digits_of_integer(int x);

void kruskal();

// #                   Arguments: N/A                    # //
// #                    Returns: N/A                     # //
// #     Description: This function show the matrix      # //
void show_matrix();

void prim(vector<pair<int, int>> adj[], int start);

void print_graph(vector<pair<int, int>> adj[]);
void print_vertix_connection(vector<pair<int, int>> adj[], int vertix);

void add_edge(vector<pair<int, int>> adj[], int w, int u, int v); // Add a edge (origin Adjacency Matrix)

// ============== </Function Prototype Area> ============= //

int main(){
    qt_digits = 0;
    do{
        cout << "1) Adjacency matrix." << endl << "2) Incidence matrix." << endl << "0) Finish execution." << endl << "> ";
        cin >> matrix_type;
        if(matrix_type == 0){
            clear();
            cout << "Exiting." << endl;
            return 0;
        }else if(matrix_type == 1 || matrix_type == 2){
            matrix_type = matrix_type == 1 ? 0 : 1;break;
        }else{
            clear();
            cout << "Invalid option." << endl;
        }
    }while(matrix_type != 0);

    cout << "Type the number of vertix: ";
    cin >> v;
    if(matrix_type == 1){ // If is incidence matrix lines is vertix and rows is edges
        cout << "Type the number of edges: ";
        cin >> e;
    }else{      // If is adjacency matrix lines and row have the same len
        e = v;
    }
    int incidence[e];
    for(int i=0; i<e; i++) incidence[i]= -1;
    vector<pair<int, int>> adj[v];

    // Read pos by pos
    for(int i=0; i<v; i++){ // Line(vertix) pos iteration if is incidence
        for(int j=0; j < e; j++){ // Row(edges) pos iteration if is incidence
            cout << "  G[" << i << "][" << j << "]: ";
            cin >> G[i][j];
            if(G[i][j] == 0)   // Needed to implementations work
                G[i][j] = INF; // Means is unreachable
        }
    }
    // Transform matrix (incidence || adjacency) to list of adjacency
   for(int i=0; i<v; i++) // Line(vertix) pos iteration if is incidence
        for(int j=0; j < e; j++) // Row(edges) pos iteration if is incidence
            if(G[i][j] != INF){ // If had connection
                if(matrix_type == 0){ // Adjacency matrix
                    add_edge(adj, G[i][j], i, j);
                    G[j][i] = INF;
                }else if(matrix_type == 1){ // Incidence Matrix
                    if(incidence[j] == -1){
                        incidence[j]=i; // Edge j know first vertix(and save the vertix)
                    }else{
                        add_edge(adj, G[i][j], incidence[j], i);
                    }
                }
            }
    clear();
    int start, op;
    do{
        cout << "1. Print all graph.\n2. Prim.\n3. Kruskal.\n4. Print all edges.\n5. Print a x-vertix and adjacency.\n0. Exit\n->";
        cin >> op;
        switch (op){
        case 0:
            clear();
            cout << "Exiting.\n";
            return 0;
            break;
        case 1:
            print_graph(adj);
            break;
        case 2:
            cout << "Type the initial vertix.\n-> ";
            cin >> start;
            if(!(start < 0) && start < v)
                prim(adj, start);
            else
                cout << "Inválid vertix value.\n";
            break;
        case 3:
            kruskal();
            break;
        case 4:
            cout << " Edge   Weight\n";
            for (auto x : edge_list){
                cout << "[" << x.second.first << "-" << x.second.second << "] (" << x.first << ")\n";
            }
            break;
        case 5:
            clear();
            cout << "Type the vertix number.\n->";
            cin >> start;
            if(!(start < 0) && start < v)
                print_vertix_connection(adj, start);
            else
                cout << "Inválid vertix value.\n";
        default:
            clear();
            cout << "Inválid option.\n";
            break;
        }
    }while(op != 0);

    return 0;
}


// Undirected graph
void add_edge(vector<pair<int, int>> adj[], int w, int u, int v){ // Add a edge (origin Adjacency Matrix)
    adj[u].push_back(make_pair(v, w));
    adj[v].push_back(make_pair(u, w));
    edge_list.push_back(make_pair((long long int)w, make_pair(u, v)));
}

// #                   Arguments: N/A                    # //
// #                    Returns: N/A                     # //
// #      Description: This function clear console       # //
void clear() {
    // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
    std::cout << "\x1B[2J\x1B[H";
}

void kruskal(){
    long long int cost = 0;
    Union_Find UF(v); // vertix
    pair<long long int, pair<int, int>> edge;
    sort(edge_list.begin(), edge_list.end());

    cout << "  Edge     Weight      \n";
    for(int i = 0; i < e; i++){//edges
        edge = edge_list[i];
        if(!UF.is_same_set(edge.second.first, edge.second.second)){
            cost += edge.first;
            cout << edge.second.first << " - " << edge.second.second << "    " << edge.first << '\n';
            UF.union_set(edge.second.first, edge.second.second);
        }
    }
    cout << "Cost = " << cost << "\n";

}

void print_graph(vector<pair<int, int>> adj[]){
    for (int i = 0; i < v; ++i) {
        //cout << "i= " << adj[i].size() << "\n";
        cout << "[" << i << "] - { ";
        for (auto x : adj[i]){
            cout << "[" << x.first << "](" <<  x.second << ") ";
        }
        printf("}\n");
    }
}

void print_vertix_connection(vector<pair<int, int>> adj[], int vertix){
    cout << "Vertix: [" << vertix << "] - {";
    for(auto x: adj[vertix])
        cout << x.first << "(" << x.second << "), ";
    printf("}\n");
}

void prim(vector<pair<int, int>> adj[], int start){
    vector<bool> in_mst(v, false); // Set all values of vector to false
    in_mst[start] = true; // Change the first value of in_mst to true (in this way including in mst)

    priority_queue<pair<int, pair<int, int>>> pq; //weight, vertix, parent
    pair<int, pair<int, int>> vertix, front;
    int u, w, o;

    long long int cost = 0;
    // Put in PQ the adjacency vertix
    for (auto x : adj[start]){
        pq.push(make_pair(-x.second, make_pair(-x.first, start))); // w first to order pq by weight, u is destination, and last is origin
    }
    cout << "  Edge     Weight      \n";
    while(!pq.empty()){
        front = pq.top(); pq.pop();
        u = -front.second.first; w = -front.first; o = front.second.second;

        if(!in_mst[u] && in_mst[o]){
            cost += (long long int)w; in_mst[u]=true;
            cout << o << " - " << u << "    " << w << '\n';
        for (auto x : adj[u]){
            if(!in_mst[x.first])
                pq.push(make_pair(-x.second, make_pair(-x.first, u)));
        }
        }
    }
    cout << "Cost: " << cost << "\n";
}
