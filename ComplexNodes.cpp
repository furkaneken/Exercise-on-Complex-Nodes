#include<iostream>
#include<string>
#include<vector>
#define SIZE 4

using namespace std;

struct ComplexNode 
{
	int num, inlist; //num is the value stored, inlist shows which list this node is in, we will use this while merging lists
	string type; // Can be "S"(standard) or "M"(merged)
	ComplexNode* nexts[SIZE];

	ComplexNode(int n = 0, int l = 0) : num(n), inlist(l)
	{
			for(int i = 0; i < SIZE; i++)
				nexts[i] = nullptr;
			type = "S";
	}
};




void Connect(ComplexNode* heads[], ComplexNode* & original, ComplexNode* & copy, bool first) //copy is actually the node before the repeated node in its list, head pointer for the first element
{
	int list = copy->inlist;
	if (!first)
	{
		for (int i = 0; i < SIZE; i++) //delete all connections to the nexts of the copy and connect them to the original
			if (copy->nexts[copy->inlist]->nexts[i] != nullptr)
			{
				original->nexts[i] = copy->nexts[copy->inlist]->nexts[i];
				copy->nexts[copy->inlist]->nexts[i] = nullptr;
			}
	}
	else
	{
		for (int i = 0; i < SIZE; i++) //delete all connections to the nexts of the copy and connect them to the original
			if (copy->nexts[i] != nullptr)
			{
				original->nexts[i] = heads[copy->inlist]->nexts[i];
				heads[copy->inlist]->nexts[i] = nullptr;
			}
	}
	
	if (!first)
	{
		delete copy->nexts[copy->inlist];
		copy->nexts[list] = new ComplexNode;
	//	copy->nexts[copy->inlist] = nullptr;
	}
	else 
	{
		delete heads[copy->inlist];
		heads[list] = new ComplexNode;
	//	heads[copy->inlist] = nullptr;
	}
	original->type = "M";

	if (!first)
		copy->nexts[list] = original;
	else
		heads[list] = original;

	//ComplexNode* temp = heads[list]; //place the merged node into the copy node's location
	//if (temp == nullptr)
	//	heads[list] = original;
	//else
	//{
	//	while (temp->nexts[list] != nullptr)
	//		temp = temp->nexts[list];
	//	temp->nexts[list] = original;
	//}
}

void InsertNode(ComplexNode* heads[], ComplexNode* tails[])
{
	int list, num;
	cout << "Please enter which list you want to add to: ";
	cin >> list;
	if (list > SIZE)
	{
		cout << "Selection out of range";
		return;
	}
	cout << "Please enter the element you want to add: ";
	cin >> num;
	if (heads[list] == nullptr) //if the list is empty
	{
		heads[list] = new ComplexNode(num, list);
		tails[list] = heads[list];
	}
	else //if the list has at least one element
	{
		tails[list]->nexts[list] = new ComplexNode(num, list);
		tails[list] = tails[list]->nexts[list];
	}
}

void Merge(ComplexNode* heads[])
{
	vector<int> checked; //checked values will be put in this vector for optimization
	ComplexNode* original; //the node that will be turned into type "M" when a match is found
	ComplexNode* search; //the node that will be deleted when a match is found
	for (int i = 0; i < SIZE - 1; i++)
	{
		original = heads[i]; //nexts lerden yuru, baglamayi oyle yaparsin, ilk iki case i ise

		while (original != nullptr)
		{

			for (int j = i + 1; j < SIZE; j++)
			{
				search = heads[j];
				if (search != nullptr)
				{
					if (original->num == search->num && original->type == "S") //check first element seperately
						Connect(heads, original, search, true);
					else
						while (search->nexts[j] != nullptr)
						{
							if (original->num == search->nexts[j]->num && original->type == "S")
							{
								Connect(heads, original, search, false);
							}
							search = search->nexts[j];
						}
				}

			}
			original = original->nexts[i];
		}
	}
}
	
	//two loops, one for elements of the list, other for other lists, no need to check the same and the last lists
	//check elements starting from list 0, compare it with all others, put in a vector so don't check again, in first match found make the original an "M", in next matches connect to it


void Print(ComplexNode* heads[], int list)
{
	if (list >= SIZE)
	{
		cout << "Selection out of range" << endl;
		return;
	}
		ComplexNode* temp = heads[list];
	if (temp == nullptr)
		cout << "The list is empty";
	while (temp != nullptr)
	{
		if (temp->type == "S") //if its a standard node
			cout << temp->type << "-" << temp->num << " ";
		else //if its a merged node
			cout << temp->type << temp->inlist << "-" << temp->num << " "; //we need to know in which list the node originated from if the node is merged
		temp = temp->nexts[list];
	}
	cout << endl;
}

void Delete(ComplexNode* heads[])
{
	int list, number;
	cout << "Please enter which list you want to delete from: ";
	cin >> list;
	cout << "Please enter the number to be deleted: ";
	cin >> number;
	ComplexNode* temp = heads[list];
	if (temp == nullptr)
	{
		cout << "The list is empty" << endl;
		return;
	}
	if (temp->num == number && temp->nexts[list] == nullptr) //execute first case seperately
	{
	/*dhelper(heads, num, list, temp*/
		ComplexNode* temp2 = temp->nexts[list];
		temp->nexts[list] = nullptr;
		int links = 0;
		for (int i = 0; i < SIZE; i++)
		{
			if (temp->nexts[i] != nullptr)
				links++;
		}
		if (links == 0) //if all is null delete the node
		{
			delete heads[list];
			heads[list] = new ComplexNode;
			heads[list] = temp2;
		}
		else if (links == 1) //if one of them is full make it "S"
		{
			temp->type = "S";
			heads[list] = temp2;
		}
		else //if more than one is full its still "M"
		{
		heads[list] = temp2;
		}
	}
	else //other cases
	{
		while (temp->nexts[list]->num == number && temp->nexts[list] != nullptr)
		{
			ComplexNode* temp2 = temp->nexts[list]->nexts[list];
			temp->nexts[list]->nexts[list] = nullptr;
			int links = 0;
			for (int i = 0; i < SIZE; i++)
			{
				if (temp->nexts[list]->nexts[i] != nullptr)
					links++;
			}
			if (links == 0) //if all is null delete the node
			{
				delete heads[list];
				heads[list] = new ComplexNode;
				heads[list] = temp2;
			}
			else if (links == 1) //if one of them is full make it "S"
			{
				temp->type = "S";
				heads[list] = temp2;
			}
			else //if more than one is full its still "M"
			{
			heads[list] = temp2;
			}
		}
	}
}
//when deleting a merged node, check all nexts,, , 

int main()
{
	int choice;

	int* p = new int;
	*p = 5;
	delete p;
	p = new int;
	*p = 4;
	ComplexNode* heads[SIZE];
	for(int i = 0; i < SIZE; i++)
		heads[i] = nullptr;
	ComplexNode* tails[SIZE];
	for(int i = 0; i < SIZE; i++)
		tails[i] = nullptr;
	do
	{
		cout << "1 - Insert a new node" << endl << //done
		"2 - Merge lists" << endl << //done
		"3 - Print list" << endl << //done
		"4 - Print all lists" << endl << //done
		"5 - Delete node" << endl <<
		"6 - Exit" << endl << endl << //done

		"Enter your choice: ";

		cin >> choice;
		if (choice == 1)
			InsertNode(heads, tails);
		else if (choice == 2)
			Merge(heads);
		else if (choice == 3)
		{
			int list;
			cout << "Please enter which list you want to print: ";
			cin >> list;
			Print(heads, list);
		}
		else if (choice == 4)
			for (int i = 0; i < SIZE; i++)
			{
				cout << "**** LIST " << i << " ****" << endl;
				Print(heads, i);
			}
		else if (choice == 5)
			Delete(heads);
		cout << endl;
	} while (choice !=6);

	return 0;
}            