//Justin Ritchey - CIS 350 - Program 3 - 12/4/2020 - 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <list>
#include <cmath>
#include <iterator>


using namespace std;

list<int> nodes[60];                                                 //*global variable* a list of vectors, where each vector will be a node which holds the edges
int* minHeap;
vector<int> verts;                   //list of nodes added to the MST graph

void datCheck(string&);                                              //Funciton to check for .dat
bool fileCheck(ifstream&);                                           //function to check if the file exisits
bool fileEmpty(ifstream&);                                           //function to check if the file is empty
bool firstLine(int&, int&, ifstream&, ofstream&, ofstream&, ifstream&);       //function to read the first line of the input file and update the values
bool edgeNodeCompare(int, int);                                      //fucntion to check if there are two many edges for the amount of nodes declared
bool missingEdgeCheck(ifstream&, ofstream&, int, int);               //fucntion to check if there are any missing edges from the amount declared
bool addEdge(int, int, int, ofstream&, int);                         //fucntion to add an edge
bool isAction(string);                                               //fucntion to check if a char is an action char
void printList(int, ofstream&);                                      //prints an adjecney list
bool MST(int, int, ofstream&);                                                  //Makes an MST graph and adjency list of the graph
void heap();                                                         //function makes a heap array with the lowest value at the top
void removeMin(int);                                                 //removes the lowest value from heap
bool check1(int);
bool check2(int);
void actions(ifstream&, ofstream&, int, int);                              //calculates the actions from the file
void userInput(ofstream&, int, int);                                            //asks for user input for extra actions

int main()
{
	string fileName = "";                                            //holds the name of the input data file to check for .dat or not
	ifstream inFile;                                                 //will hold the input file
	ofstream outFile;                                                //will hold the output file
	ofstream updated_out_InFile;
	updated_out_InFile.open("input.dat");                            //new input file after changes
	ifstream updatedInFile;
	int numNodes = 0;                                                //will hold the amount of nodes from first line of input file
	int numEdges = 0;                                                //will hold the amount of edges from first line of input file
	ifstream actionFile;

	cout << "Welcome to the Minimum Spanning Tree program!\n\n";     //welcome message
    
	cout << "Please enter a name of your input file: ";              //gathering name of input name and opening it
	cin >> fileName;
	datCheck(fileName);
	inFile.open(fileName);
	actionFile.open(fileName);

	if (fileCheck(inFile) == true)                                    //ends the program if the program cannot find the file
	{
		"Cannot find the file name you entered, program will now end";
		return 0;
	} 
	if (fileEmpty(inFile) == true)                                    //ends the program if the file is empty
	{
		cout << "The file is empty, the program will be terminated";
		return 0;
	}

	cout << "Please enter a name for your output file: ";             //gathering name of the output file and opening  
	cin >> fileName;
	datCheck(fileName);
	outFile.open(fileName);

	if (firstLine(numNodes, numEdges, inFile, outFile, updated_out_InFile, updatedInFile) == false)      //assigning the first line to values for amount of nodes and edges and checking if the amount of edges is lower than the max possible
	{
		cout << "There are too many listed edges for the declared amount of nodes, program will now terminate\n\n";
		outFile << "There are too many listed edges for the declared amount of nodes, program will now terminate\n\n";
		return 0;
	}

	if (missingEdgeCheck(updatedInFile, outFile, numEdges, numNodes) == false)                  //error checking and making the adjecney list if false, will end the program
	{
		return 0;
	}

	printList(numNodes, outFile);                                 //printing the adjeceny list
	if (MST(numNodes, numEdges, outFile) == false)
	{
		cout << "The graph is not complete, program will now terminate\n\n";
		outFile << "The graph is not complete, program will now terminate\n\n";
		return 0;
	}

	actions(actionFile, outFile, numNodes, numEdges);
	userInput(outFile, numNodes, numEdges);


	return 0;
}

void datCheck(string& fileName)                                      //function to check if the .dat was enetered by the user
{                                                                    //if not this will add the .dat to the name entered 
	string dat = ".dat";                                             //this does not account for any "dat" in the name of the file
	int find;

	find = fileName.find(dat);                                       //searchs the string for the "dat" phrase

	if (find > 0)                                                    //if any "dat"'s were found it will return
	{
		return;
	}
	else                                                             //else it will add a .dat to the end of the string
	{
		fileName = fileName + ".dat";
	}
}

bool fileCheck(ifstream& file)                                        //Checks if the file opened susscufuly 
{
	bool result = false;                                              //boolean variable to be returned

	if (file.fail())                                                  //if the program did not find the file name it will terminidate and print out why
	{
		result = true;
		cout << "The input file does not exist. The program will be terminated\n";
		return result;
	}

	return result;
}

bool fileEmpty(ifstream& file)
{
	bool result = false;

	file.seekg(0, file.end);                                         //checks if the file is empty
	if (file.tellg() == 0)
	{
		result = true;
		return result;
	}
	file.seekg(0);

	return result;
}

bool firstLine(int& node, int& edge, ifstream& in, ofstream& out, ofstream& inputOut, ifstream& newInput)
{
	string line;                             //holds the lines fo the file
	string line1;
	string line2;
	int i = 0;

	while (i == 0)                           //grabbing the first line of the file (will be two ints)
	{
		in >> line1 >> line2;                
		i++;
	}

	stringstream nod(line1);                 //changing the strings into their int values
	nod >> node;
	stringstream nod1(line2);
	nod1 >> edge;

	in.seekg(2, ios::cur);                   //setting the file pointer to the second line of the file

	while (getline(in, line))                //outputting the rest of the lines into a new file without the first line
	{
		inputOut << line << endl;
	}

	newInput.open("input.dat");             //opening the new file

	if (edgeNodeCompare(node, edge) == false)          //sending to function to compare if there are too many edges for the amount of nodes listed, will term if there are too many edges
	{
		return false;
	}
	else                                   //printing out the amount of nodes and edges
	{
		cout << "\nThere are " << node << " nodes and " << edge << " edges\n\n";
		out <<  "\nThere are " << node << " nodes and " << edge << " edges\n\n";
		return true;
	}
}

bool edgeNodeCompare(int node, int edge)                   //fuction to check if there are too many edges for the amount of nodes
{
	int maxEdges = 0;

	maxEdges = node * (node - 1) / 2;                     //calcuation for the max amount of edges for the given nodes

	if (maxEdges < edge)                                  //if there are more edges than the max return false
	{
		return false;
	}
	else                                                  //else return true
	{
		return true;
	}
		
}

bool missingEdgeCheck(ifstream& in, ofstream& out, int edge, int node)
{
	string line = "";                                        //will hold the each line while checking
	string a = "";
	string b = "";
	string c = "";
	string d = "";
	int w, x, y, z;
	int num_of_lines = 0;                                    //starts at second line of the file to count the amount of lines read in before any actions
	int missingEdges = 0;
	ofstream update;

	cout << "Edges: \n";

	while (getline(in, line))                                //counting the non action lines to see if there are any missing edges from what was declared
	{
		stringstream ch(line);                            
		ch >> a >> b >> c >> d;                          

		if (isAction(a) == true)
		{
			break;
		}

		num_of_lines++;
	}

	if (num_of_lines > edge)                                //if there is more edges listed than what was declared from the first line
	{
		cout << "To many edges have been listed in the file, program will now terminate\n\n";
		out << "To many edges have been listed in the file, program will now terminate\n\n";
		return false;
	}

	if (num_of_lines < edge)                                 //if any lines are missing they will be added here and then put into the file
	{
		missingEdges = edge - num_of_lines;                  //holds the amount of missing edges to be entered in
		cout << missingEdges << " edges are missing. Please add the missing edges: \n";
		out << missingEdges << " edges are missing. Please add the missing edges: \n";

		in.clear();                                          //setting the file pointer to the start and opening new file to enter the missing lines in too
		in.seekg(0, ios::beg); 
		update.open("input1.dat");

		while (getline(in, line))                            //adding in all the exisiting non action lines to the file
		{
			update << line << endl;

			if (isAction(line) == true)
			{
				break;
			}
		}

		in.close();                                          //clsoing the current input file from the user

		while (num_of_lines < edge)                          //gathering the missing lines from the user and entering them into the new file
		{
			cin >> w >> x >> y;
			update << w << " " << x <<" " << y << endl;
			num_of_lines++;
		}

		in.open("input1.dat");                               //opening the new file with the input name from the user

		out << "\n\n";
		cout << "\n\n";

	}

	in.clear();                                            //setting file pointer back to the start
	in.seekg(0, ios::beg);

	while (getline(in, line))
	{
		d = "";
		stringstream ch(line);                             //printing out each line while checking for errors (if errors occur will print out the error)
		ch >> a >> b >> c >> d;                            //if no errors will send to add edge function

		if (isAction(a) == true)                           //if the first letter of any non action line is for an action this will break the loop
		{
			break;
		}

		cout << a << " " << b << " " << c << " " << d;     //echo printng each line
		out << a << " " << b << " " << c << " " << d;

		if (d != "")                                       //checking to make sure there isnt a 4th digit (any more will not be checked or shown)     
		{
			cout << " - Too many digits in this line\n";
			out << " - Too many digits in this line\n";
			d = "";
			continue;                                      //move to next line of the file
		}

		stringstream che(line);
		che >> w >> x >> y;

		if (y == 0)                                       //checking to make sure weight is above 0
		{
			cout << " - Weight cannot be < 1\n";
			out << " - Weight cannot be < 1\n";
			continue;
		}

		if (!(isdigit(a[0]) && isdigit(b[0]) && isdigit(c[0])))  //checking for any value not a digit
		{
			cout << " - invalid command\n";
			out << " - invalid command\n";
			continue;
		}

		if (w == x)                                             //if both nodes entered are the same
		{
			cout << " - Cannot create edge on the same node\n";
			out << " - Cannot create edge on the same node\n";
			continue;
		}

		if (w > node - 1 || x > node - 1)                      //if a node is entered that is larger than the amount of nodes entered. 
		{
			cout << " - a node entered is not in scope of the graph\n";
			out << " - a node entered is not in scope of the graph\n";
			continue;
		}

		if (addEdge(w, x, y, out, node) == false)               //if the line passes the tests, this will add the line
		{
			cout << " - Duplicate edge\n";
			out << " - Duplicate edge\n";
		}
		cout << " \n";
		out << " \n";
	}

	return true;
}

bool addEdge(int a, int c, int w, ofstream& out, int node)
{
	for (int i = 0; i < node; i++)                    //checking for a dublicate in the list
	{ 
		list<int> ::iterator it;
		for (it = nodes[i].begin(); it != nodes[i].end(); ++it) 
		{
			if (*it == a && i == c)
			{
				return false;
			}
			else if (*it == c && i == a)
			{
				return false;
			}
			++it;
		}
	}
	nodes[a].push_back(c);                //adding the node and weight into the list
	nodes[a].push_back(w);

	nodes[c].push_back(a);                //adding the node and weight onto the list for the reverse 
	nodes[c].push_back(w);

	return true;
}

void printList(int node, ofstream& out)
{
	int counter = 0;                                     //used to add the formatting to the array

	cout << "\nOverall Adjecency list for the graph\n";
	out << "\nOverall Adjecency list for the graph\n";

	for (int i = 0; i < node; i++)                       //prints out the adjeceny list
	{
		cout <<"Node[" <<  i << "] -->";
		list<int> ::iterator it;
		for (it = nodes[i].begin(); it != nodes[i].end(); ++it) 
		{
			if (counter == 0)
			{
				cout << "(";
				out << "(";
				counter++;
			}
			cout << *it;
			out << *it;
			if (counter == 1)
			{
				cout << ", ";
				out << ", ";
			}
			if (counter == 2)
			{
				cout << ") ";
				out << ") ";
				counter = 0;
				continue;
			}
			counter++;
		}
		cout << endl;
		out << endl;
	}
	
}

bool isAction(string s)                      //fucntion to see if the char is a action character
{
	switch (s[0])
	{
	case 'd':
	case 'D':
	case 's':
	case 'S':
	case 'e':
	case 'E':
	case 'i':
	case 'I':
		return true;
		break;
	}

	return false;
}

bool MST(int node, int edges, ofstream& out)
{
	int co = 0;
	int temp = 0;

	minHeap = new int[edges*2];                //min heap array that will hold the edges to be choosen

    verts.push_back(0);                        //starting with node 0 and adding to the MST graph

	           
	list<int> ::iterator it;                   //adds the edges of the first node to the minheap array
	for (it = nodes[0].begin(); it != nodes[0].end(); it++)
	{
		it++;
		minHeap[co] = *it;
		co++;
	}
	co = 0;
	heap();                                    //moves the lowest edge to root of the minheap array
	list<int> ::iterator it1;
	list<int> ::iterator it2;
	for (it2 = nodes[0].begin(); it2 != nodes[0].end(); it2++)
	{
		it2++;
		if (minHeap[0] == *it2)
		{
			verts.push_back(*--it2);
			break;
		}
	}

	for (it1 = nodes[*it2].begin(); it1 != nodes[*it2].end(); it1++)
	{
		it1++;
		for (int i = 0; i < (edges*2); i++)
		{
			if (minHeap[i] < 0)
			{
				minHeap[i] = *it1;
				i++;
				break;
			}
		}
	}

	removeMin(edges);
	heap();
	/*
	while (verts.size() < node)
	{
		check1(node);
	}
	*/

	cout << "\nMST graph\n";
	out << "\nMST graph\n";
	for (int i = 0; i < edges; i++)
	{
		if (minHeap[i] < 0)
		{
			continue;
		}
		cout << minHeap[i] << " ";
		out << minHeap[i] << " ";

	}
	cout << "\n";
	out << "\n";
	return true;
	
}

void heap()                             //function to put the min number at the top of the heap array
{
	int temp = 0;

	for (int i = sizeof(minHeap); i > 0; i--)
	{
		if (minHeap[i] < 0)
		{
			continue;
		}
		if (minHeap[i] < minHeap[(i - 1) / 2])
		{
			temp = minHeap[i];
			minHeap[i] = minHeap[(i - 1) / 2];
			minHeap[(i - 1) / 2] = temp;
		}
	}
}

void removeMin(int edges)
{
	int root = minHeap[0];
	for (int i = (edges * 2); i > 0; i--)                     //assigning the last value added to the root
	{
		if (minHeap[i] < 0)
		{
			continue;
		}
		for (int j = i - 1; j > 0; j--)
		{
			if (minHeap[j] == root)
			{
				minHeap[j] = minHeap[i];
				minHeap[i] = -1;
			}
		}
		break;
	}

	for (int i = (edges * 2); i > 0; i--)
	{
		if (minHeap[i] < 0)
		{
			continue;
		}
		minHeap[0] = minHeap[i];
		minHeap[i] = -1;
		break;
	}
}

bool check1(int node)
{
	int w, n;
	list<int> ::iterator it;
	for (int i = 0; i < node; i++)
	{
		for (it = nodes[i].begin(); it != nodes[i].end(); it++)
		{
			n = *it;            //node i and node n with w being the weight
			it++;
			w = *it;
			if (minHeap[0] == *it)
			{
				for (int l = 0; l < verts.size(); l++)
				{
					if (verts.at(l) != n)
					{
						continue;
					}
					else
					{
						return false;
					}
				}
				return true;
			}
		}
	}
	if (check2(node) == true)
	{
		verts.push_back(*--it);
	}
	return true;
}

bool check2(int node)
{
	int w, n;
	list<int> ::iterator it;
	list<int> ::iterator it2;
	for (int i = 0; i < node; i++)
	{
		for (it = nodes[i].begin(); it != nodes[i].end(); it++)
		{
			n = *it;            //node i and node n with w being the weight
			it++;
			w = *it;
			for (it2 = nodes[n].begin(); it2 != nodes[n].end(); it2++)
			{
				for (int h = 0; i < verts.size(); h++)
				{
					if (*it2 == verts.at(h))
					{
						return true;
					}
				}
				it2++;
			}
		}
	}
	return false;
}

void actions(ifstream& in, ofstream& out, int node, int edges)
{
	string line = "";                                        //will hold the each line while checking
	string a = "";
	string b = "";
	string c = "";
	string d = "";
	string e = "";
	char z;
	int w, x, y;

	cout << "Actions: \n";

	while (getline(in, line))                                //counting the non action lines to see if there are any missing edges from what was declared
	{
		stringstream ch(line);
		ch >> a >> b >> c >> d;

		if (e != "")                                       //checking to make sure there isnt a 4th digit (any more will not be checked or shown)     
		{
			cout << "-----------------------------\n";
			out << "-----------------------------\n";
			cout << " - Too many digits in this line\n";
			out << " - Too many digits in this line\n";
			cout << "\n-----------------------------\n";
			out << "\n-----------------------------\n";
			d = "";
			continue;                                      //move to next line of the file
		}

		stringstream che(line);
		che >> z >> w >> x >> y;

		if (a == "e" || a == "E")
		{
			cout << "-----------------------------\n";
			out << "-----------------------------\n";
			cout << a << " " << b << " " << c << " " << d <<" " << e;     //echo printng each line
			out << a << " " << b << " " << c << " " << d <<" " << e;

			if (y == 0)                                       //checking to make sure weight is above 0
			{
				cout << " - Weight cannot be < 1\n";
				out << " - Weight cannot be < 1\n";
				cout << "-----------------------------\n";
				out << "-----------------------------\n";
				continue;
			}

			if (!(isdigit(b[0]) && isdigit(c[0]) && isdigit(d[0])))  //checking for any value not a digit
			{
				cout << " - invalid command\n";
				out << " - invalid command\n";
				cout << "-----------------------------\n";
				out << "-----------------------------\n";
				continue;
			}

			if (w == x)                                             //if both nodes entered are the same
			{
				cout << " - Cannot create edge on the same node\n";
				out << " - Cannot create edge on the same node\n";
				cout << "-----------------------------\n";
				out << "-----------------------------\n";
				continue;
			}

			if (w > node - 1 || x > node - 1)                      //if a node is entered that is larger than the amount of nodes entered. 
			{
				cout << " - a node entered is not in scope of the graph\n";
				out << " - a node entered is not in scope of the graph\n";
				cout << "-----------------------------\n";
				out << "-----------------------------\n";
				continue;
			}
			if (addEdge(w, x, y, out, node) == false)               //if the line passes the tests, this will add the line
			{
				cout << " - Duplicate edge\n";
				out << " - Duplicate edge\n";
				cout << "-----------------------------\n";
				out << "-----------------------------\n";
				continue;
			}

			printList(node, out);
			MST(node, edges, out);
			cout << "-----------------------------\n";
			out << "-----------------------------\n";

		}
		else if (a == "d" || a == "D")                                     //decreses weight of an edge
		{
			cout << a << " " << b << " " << c << " " << d << " " << e;     //echo printng each line
			out << a << " " << b << " " << c << " " << d << " " << e;

			list<int> ::iterator it;
			for (it = nodes[w].begin(); it != nodes[w].end(); it++)
			{
				if (*it == x)
				{
					++it;
					if ((*it - y) < 1)
					{
						cout << " - Weight cannot be less than 1 - edge not updated\n";
						out << " - Weight cannot be less than 1 - edge not updated\n";
						break;
					}
					else
					{
						*it = *it - y;
						break;
					}
				}
				it++;
			}
			list<int> ::iterator it1;
			for (it1 = nodes[x].begin(); it1 != nodes[x].end(); it1++)
			{
				if (*it1 == w)
				{
					++it1;
					if ((*it1 - y) < 1)
					{
						break;
					}
					else
					{
						*it1 = *it1 - y;
						break;
					}
				}
				it1++;
			}

			cout << "\n";
			printList(node, out);
			MST(node, edges, out);
			cout << "-----------------------------\n";
			out << "-----------------------------\n";
		}
		else if (a == "i" || a == "I")                                     //increases weight of an edge
		{
			cout << a << " " << b << " " << c << " " << d << " " << e;     //echo printng each line
			out << a << " " << b << " " << c << " " << d << " " << e;
			list<int> ::iterator it;
			for (it = nodes[w].begin(); it != nodes[w].end(); it++)
			{
				if (*it == x)
				{
					++it;
					*it = *it + y;
					break;
				}
				it++;
			}
			list<int> ::iterator it1;
			for (it1 = nodes[x].begin(); it1 != nodes[x].end(); it1++)
			{
				if (*it1 == w)
				{
					++it1;
					*it1 = *it1 + y;
					break;
				}
				it1++;
			}

			cout << "\n";
			out << "\n";
			printList(node, out);
			MST(node, edges, out);
			cout << "-----------------------------\n";
			out << "-----------------------------\n";
		}
		else if (a == "s" || a == "S")                                     //shortest path from one node to another
		{
		    cout << "\n-----------------------------\n";
			out << "\n-----------------------------\n";
			cout << a << " " << b << " " << c << " " << d << " " << e;     //echo printng each line
			out << a << " " << b << " " << c << " " << d << " " << e;
			cout << " not ready yet\n";
			out << " not ready yet\n";
			cout << "-----------------------------\n";
			out << "-----------------------------\n";
		}
	}	
}

void userInput(ofstream& out, int node, int edges)
{
	char a = '\n';
	int x, y, z;

	while (true)
	{
		cout << "\nPLease enter any additonal actions(press q/Q to quit): ";
		cin >> a;

		if (a == 'e' || a == 'E')
		{
			cin >> x >> y >> z;
			cout <<"\nYou entered: " << a << " " << x << " " << y << " " << z << "\n";     //echo printng each line
			out << "\nYou entered: " << a << " " << x << " " << y << " " << z << "\n";

			if (z < 0)                                             //checking to make sure weight is above 0
			{
				cout << " - Weight cannot be < 1\n";
				out << " - Weight cannot be < 1\n";
				continue;
			}

			if (y == x)                                             //if both nodes entered are the same
			{
				cout << " - Cannot create edge on the same node\n";
				out << " - Cannot create edge on the same node\n";
				continue;
			}

			if (y > node - 1 || x > node - 1)                      //if a node is entered that is larger than the amount of nodes entered. 
			{
				cout << " - a node entered is not in scope of the graph\n";
				out << " - a node entered is not in scope of the graph\n";
				continue;
			}
			if (addEdge(z, x, y, out, node) == false)               //if the line passes the tests, this will add the line
			{
				cout << " - Duplicate edge\n";
				out << " - Duplicate edge\n";
				continue;
			}
			
			cout << "\n";
			out << "\n";

			printList(node, out);
			MST(node, edges, out);
			out << "\n";
			cout << "\n";

		}
		else if (a == 'd' || a == 'D')
		{
			cin >> x >> y >> z;
			cout << "\nYou entered: " << a << " " << x << " " << y << " " << z << "\n";     //echo printng each line
			out << "\nYou entered: " << a << " " << x << " " << y << " " << z << "\n";
			list<int> ::iterator it;
			for (it = nodes[x].begin(); it != nodes[x].end(); it++)
			{
				if (*it == y)
				{
					++it;
					if ((*it - z) < 1)
					{
						cout << " - Weight cannot be less than 1 - edge not updated\n";
						out << " - Weight cannot be less than 1 - edge not updated\n";
						break;
					}
					else
					{
						*it = *it - z;
						--it;
						break;
					}
				}
				it++;
			}
			list<int> ::iterator it1;
			for (it1 = nodes[y].begin(); it1 != nodes[y].end(); it1++)
			{
				if (*it1 == x)
				{
					++it1;
					if ((*it1 - z) < 1)
					{
						break;
					}
					else
					{
						*it1 = *it1 - z;
						--it1;
						break;
					}
				}
				it1++;
			}
			printList(node, out);
			MST(node, edges, out);
			cout << "\n";
		}
		else if (a == 'i' || a == 'I')
		{
			cin >> x >> y >> z;
			cout << "\nYou entered: " << a << " " << x << " " << y << " " << z << "\n";     //echo printng each line
			out << "\nYou entered: " << a << " " << x << " " << y << " " << z << "\n";
			list<int> ::iterator it;
			for (it = nodes[x].begin(); it != nodes[x].end(); it++)
			{
				if (*it == y)
				{
					++it;
					*it = *it + z;
					--it;
					break;
				}
				it++;
			}
			list<int> ::iterator it1;
			for (it1 = nodes[y].begin(); it1 != nodes[y].end(); it1++)
			{
				if (*it1 == x)
				{
					++it1;
					*it1 = *it1 + z;
					--it1;
					break;
				}
				it1++;
			}
			printList(node, out);
			MST(node, edges, out);
			cout << "\n";
			out << "\n";
		}
		else if (a == 's' || a == 'S')
		{
			cin >> x >> y >> z;
			cout << "\nYou entered: " << a << " " << x << " " << y << " " << z << "\n";     //echo printng each line
			out << "\nYou entered: " << a << " " << x << " " << y << " " << z << "\n";
			cout << " not ready yet\n";
			out << " not ready yet\n";
		}
		else if (a == 'q' || a == 'Q')
		{
			return;
		}
		else
		{
			cout << "invalid command\n";
			out << "invalid command\n";
			continue;
		}
	}
}