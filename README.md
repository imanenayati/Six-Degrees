# Six-Degrees


Compile/run:

    - compile using
        make
    - run executable using
        ./SixDegrees dataFile [commandsFile] [outputFile]
               - dataFile is an input file containing information about 
                 artists
               - commandsFile (optional) is a second input file containing
                 commands to execute. If this parameter is not given, 
                 the program reads input from std::cin.
               - outputFile (optional) is an output file. If provided,
                 SixDegrees will send search results to this file.
                 If this parameter is not given, then you must send output 
                 to the standard output stream (std::cout).


Program Purpose:

        The purpose of the program in the context of your "Six Degrees 
        of Collaboration" project is to explore and visualize the 
        relationships between musical artists based on their collaborative 
        works (songs).

Acknowledgements:
	Thank you Weiwei for your support in troubleshooting this
	program.


How much time the assignment took you in hours: 
	10 hours

Files:

main.cpp

    Initializes an object named six_degrees from the SixDegrees class 
    and then invokes the populate_collabGraph and command_loop methods. 
    Both methods belong to the SixDegrees class. The command_loop 
    method processes inputs and carries out commands as needed.


CollabGraph.cpp:

    Defines the functionality of the CollabGraph class, setting up 
    a network where artists are nodes and collaborations, depicted 
    as edges, represent songs created together by artist pairs.
     
CollabGraph.h

    Declares the interface for the CollabGraph, outlining a structure 
    where artists form nodes and their joint songs create links 
    (edges) between these nodes.

SixDegrees.cpp:

    Facilitates the SixDegrees class interface by populating the 
    CollabGraph, managing a series of commands, and generating the 
    respective outputs. It also operates the command loop.

SixDegrees.h:

    Defines the SixDegrees class interface, which includes a CollabGraph 
    instance. It features methods designed to fill and traverse the 
    CollabGraph efficiently.
     
artists.txt
 
     sample list of artists and their songs provided
     
Makefile:
     For compilation purposes
          
Data Structures and Algorithm:
     
    The program leverages data structures such as vectors, queues, 
    stacks, and unordered maps to construct and navigate a graph. 
    The populate_graph function accepts a vector of artists to fill 
    the graph. The dynamic size of a vector is particularly beneficial 
    here because it allows for an unspecified number of artists, which 
    is crucial for scalability. This flexibility is also advantageous 
    in the get_vertex_neighbors function, where the potentially 
    unlimited number of neighbors an artist can have is accommodated 
    without size constraints. Furthermore, vectors provide convenient 
    access to all their elements, facilitating quick iteration.

    The report_path function utilizes a stack to record the route 
    between two artists, placing the starting artist on top. This 
    arrangement enables the program to access and modify the top 
    element before moving to the next, similar to a queue where only 
    the front element is accessible. The correct order must be maintained 
    in the stack to ensure the destination artist is at the bottom and 
    the starting artist is at the top.

    The application employs both breadth-first and depth-first traversal 
    strategies to assign predecessors and navigate through the graph. 
    The breadth-first search (BFS) and a modified BFS, termed 'not', 
    which excludes specific artists from the path, use both a queue for 
    pending artists and a vector for their neighbors. As each artist is 
    explored, their neighbors are set as predecessors, added to the queue, 
    and the process repeats until all artists are visited. In the 'not' 
    function, artists to be excluded are marked prior to starting the 
    traversal.

    For depth-first search (DFS), recursion is employed, allowing the 
    traversal to extend as deep into the graph as possible without 
    necessarily finding the shortest path between two artists.

    Once set up, the graph structure proves to be highly efficient for:

    Iterating through nodes (vertices) and their connections (edges).
    Quickly identifying one of the shortest paths between artists 
    using the BFS function, enhancing user experience by providing 
    rapid results.

Testing:
I start by conducting unit testing. I test each individual function 
and method in isolation to verify their correctness. I create test 
cases for each function, covering various scenarios and edge cases. 
I use assertion statements to check if the expected output or 
behavior matches the actual results. Additionally, I test the 
constructors, destructors, and assignment operators of the classes 
to ensure they work correctly. I also verify that the accessors and 
mutators of the classes function as expected and test the error 
handling and input validation logic within the functions.
Next, I focus on testing the graph construction process. I provide 
different datasets to the program and ensure that vertices are correctly 
added to the graph based on the input data. I also verify that edges are 
properly established between the vertices according to the collaboration 
information. To assess the graph construction performance, I test with 
datasets of varying sizes and complexity.
Once the graph is constructed, I move on to testing the graph 
traversal algorithms, specifically the BFS and DFS algorithms that 
I've implemented. I test these algorithms with different starting 
and destination vertices to ensure they find the correct paths between 
the vertices. I also test with both connected and disconnected graphs 
to ensure proper handling of unreachable vertices. Additionally, I validate 
that the algorithms terminate correctly and handle cycles in the graph 
appropriately.
After testing the traversal algorithms, I focus on testing the path 
reporting functionality. I check the generated paths between vertices 
to verify that they are correctly formatted and include the necessary 
information. I test with different pairs of vertices, including adjacent 
vertices, vertices with multiple paths, and unreachable vertices. I also 
ensure that the program handles cases where no path exists between the 
vertices gracefully.
The exclusion functionality is another important aspect that I need to 
test. I provide different sets of vertices to exclude from the path finding 
and verify that the program correctly excludes the specified vertices and 
finds alternative paths when possible. I test with various combinations 
of excluded vertices, including vertices that are part of the shortest 
path and vertices that are not in the graph.
Input handling is crucial for the program's robustness, so I test the 
program with different types of inputs, including valid and invalid 
inputs. I verify that the program handles incorrect command formats, 
missing arguments, and invalid artist names gracefully. I also test 
the program with edge cases, such as empty input files or input files 
with incorrect formats.
To ensure the correctness of the program's output, I compare the program's 
output with the expected output for different test cases. I verify 
that the program produces the correct paths, path lengths, and 
collaboration information. I also check the formatting and readability 
of the output to make sure it's clear and understandable.
 
The difference between BFS and DFS? What are their
pros/cons?

Breadth-First Search (BFS):
BFS systematically explores the graph level by level, making it optimal 
for finding the shortest path between two points. It implements a queue 
(first-in, first-out) to manage the nodes as they are visited, ensuring 
that each node's neighbors are queued for subsequent exploration. While 
this method can be less speedy due to its systematic nature, its strength 
lies in reliably determining the shortest path, which is particularly 
valuable in many practical applications.

Depth-First Search (DFS):
DFS explores as deeply as possible into the graph before backtracking, 
making it useful for scenarios where discovering a complete path is 
necessary, not necessarily the shortest. It can be executed using 
recursion or a stack (last-in, first-out), where visited nodes are pushed 
onto the stack, and backtracking occurs by popping the stack when no unvisited 
neighbors are left. Although DFS is typically faster than BFS, it may traverse 
more nodes than necessary, as it does not guarantee the shortest path.


Implementing the "incl" and "not" Commands
Inclusive Search ("incl"):

Purpose: The "incl" command aims to find a path from artist A to artist B 
that must pass through a specified artist C. This presents unique challenges, 
as it requires ensuring that any discovered path includes artist C.
Implementation Strategy: This was approached by conducting two separate BFS 
traversals. The first traversal starts from artist A and stops once artist 
C is reached, marking the nodes visited along the way. The second traversal 
then begins from artist C and continues to artist B. The challenge is ensuring 
the path from A to C does not overlap with C to B, except at artist C.
Complexity and Comparison: The complexity is similar to a standard BFS in terms 
of time, but the need to manage two separate lists of visited nodes and to 
coordinate the paths adds a layer of complexity. Compared to a simple BFS, 
this method is more computationally intensive because it essentially performs 
BFS twice and must handle additional checks for path validity.
Exclusive Search ("not"):

Purpose: The "not" command is designed to find a path from artist A to artist 
B while explicitly avoiding certain specified artists. This command is crucial 
for scenarios where the user wants to explore alternative connections that do 
not involve mainstream or frequently occurring collaborators.
Implementation Strategy: This command modifies the BFS algorithm by first 
marking the artists to be excluded as already visited before the search begins. 
This preemptive marking prevents these nodes from being added to the queue 
during the BFS traversal.
Complexity and Comparison: The modification does not significantly change the 
overall time complexity from O(V + E), typical of BFS, where V is the number of 
vertices and E is the number of edges. However, the spatial complexity may 
increase slightly due to the need to maintain a larger list of marked nodes. 
The logic is simpler than that of DFS, as it does not require backtracking.
Comparison to BFS and DFS
BFS and "not": The "not" command is essentially a BFS with a pre-filtering step 
to exclude certain nodes. Its performance will generally mirror that of BFS except 
for the exclusion logic, which marginally increases the preprocessing time.
BFS and "incl": The "incl" command is more complex than a standard BFS because it 
must ensure that the path includes a specific node. This requirement to control 
the path's composition is a significant divergence from standard BFS, which 
simply finds the shortest path without any such constraints.
DFS: DFS inherently explores as deep as possible into the graph, which can be 
inefficient for the "not" and "incl" commands. DFS is not typically used for 
these commands because controlling the traversal depth dynamically based on 
runtime conditions (like excluding or including specific nodes) is more complex 
and less efficient in a DFS context.
Overall, the "incl" and "not" commands add layers of strategic decision-making 
to the basic traversal algorithms, tailored to provide users with more controlled 
and meaningful exploration of the artist collaboration graph.


Runtime for each of the commands
Breadth-First Search (BFS):
The time complexity for BFS is often expressed as O(V^2), especially in graphs 
represented by adjacency matrices. This complexity arises because each node and 
its neighbors are systematically explored, requiring potentially exhaustive 
checks when all nodes are connected.

Depth-First Search (DFS):
DFS also tends to have a time complexity of O(V^2) under similar conditions as 
BFS, particularly in dense graphs. The complexity is due to the depth-first 
nature of the traversal, where each node’s connections are explored deeply 
before moving to the next.

Exclusion Search (not command):
The 'not' command, which functions similarly to BFS but excludes specified 
nodes, also operates with a time complexity of O(V^2). This complexity 
considers the additional steps of marking excluded nodes and potentially 
increased traversal depth, as some paths are deliberately avoided.
