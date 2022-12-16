// Nick Franklin

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

// structure to hold course information
struct Course {
	string courseId; // unique id for comparison and placement in bst
	string courseTitle;
	vector <string> coursePrerequisite;

	Course() {
	}
};

// struture for tree node
struct Node {
	Course course;
	Node* left;
	Node* right;

	// default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// initialize with a course
	Node(Course aCourse) : Node() {
		this->course = aCourse;
	}
};

class BinarySearchTree {

private:
	Node* root;
	void addNode(Node* node, Course course);
	void inOrder(Node* node);

public:
	BinarySearchTree();
	virtual ~BinarySearchTree();
	void DeleteBST(Node* node);
	void InOrder();
	void Insert(Course course);
	Course Search(string courseId);
};

// Default constructor
BinarySearchTree::BinarySearchTree() {
	root = nullptr;
}

// Destructor
BinarySearchTree::~BinarySearchTree() {
	DeleteBST(root);
}

void BinarySearchTree::DeleteBST(Node* node) {
	if (node) {
		DeleteBST(node->left);
		DeleteBST(node->right);
		delete node;
	}
}

void BinarySearchTree::Insert(Course course) {
	// if root is empty
	if (root == nullptr) {
		// new node is root
		root = new Node(course);
	}
	// else
	else {
		// send to addnode 
		this->addNode(root, course);
	}
}

void BinarySearchTree::addNode(Node* node, Course course) {
	// if node is larger then go left
	if (node->course.courseId.compare(course.courseId) > 0) {
		// if no left node
		if (node->left == nullptr) {
			// this node becomes left
			node->left = new Node(course);
		}
		// else recurse down the left node
		else {
			this->addNode(node->left, course);
		}
	}
	// else
	else {
		// if no right node
		if (node->right == nullptr) {
			// this node becomes right
			node->right = new Node(course);
		}
		//else
		else {
			// recurse down the right node
			this->addNode(node->right, course);
		}
	}
}

void BinarySearchTree::InOrder() {
	// call inOrder fuction and pass root 
	this->inOrder(root);
}

void BinarySearchTree::inOrder(Node* node) {
	//if node is not equal to null ptr
	if (node != nullptr) {
		//InOrder left
		inOrder(node->left);
		//output bidID, title, amount, fund
		cout << node->course.courseId << ", " << node->course.courseTitle << endl;
		//InOder right
		inOrder(node->right);
	}
}

Course BinarySearchTree::Search(string courseId) {
	// current node start with root
	Node* current = root;

	// loop down until course found or reach end
	while (current != nullptr) {
		// if found then return current course
		if (current->course.courseId.compare(courseId) == 0) {
			return current->course;
		}
		// if course is smaller than current node then traverse left
		else if (courseId.compare(current->course.courseId) < 0) {
			current = current->left;
		}
		// else larger so traverse right
		else {
			current = current->right;
		}
	}
	Course course;
	return course;
}

void displayCourse(Course course) {
	// display course data
	cout << course.courseId << ", " << course.courseTitle << endl;
	cout << "Prerequisite(s): ";
	unsigned int i;

	 // in case of prereqs to display
	if (course.coursePrerequisite.size() != 0) {
		// loop for all prereq's
		for (i = 0; i < course.coursePrerequisite.size(); ++i) {
			cout << course.coursePrerequisite.at(i);
			// if more than one prereq, add comma
			if (i != course.coursePrerequisite.size() - 1) {
				cout << ", ";
			}
		}
		cout << endl;
	}
	
	else {
		cout << "No Prerequisites" << endl;
	}

	return;
}

void readFile(string filePath, BinarySearchTree* bst) {
	cout << "Opening course file..." << endl;
	// open course file
	ifstream courseFile(filePath);
	// variable for each line of file
	string line;
	// variables for each part of each line of the file
	string courseId;
	string courseName;
	string prereqOne;
	string prereqTwo;

	// make sure file opened successfully
	if (!courseFile.is_open()) {
		cout << "File failed to open." << endl;
		return;
	}

	// loop to parse through each line of the file
	while (getline(courseFile, line)) {
		// call for parsing through each line to each comma
		stringstream ss(line);
		// get each part of the line and save it to its respective variable
		getline(ss, courseId, ',');
		getline(ss, courseName, ',');
		getline(ss, prereqOne, ',');
		getline(ss, prereqTwo, ',');

		// create new course object
		Course course;
		// retrieve the new course object's variables
		course.courseId = courseId;
		course.courseTitle = courseName;

		// in case of prereq's
		if (prereqOne.length() > 3) {
			course.coursePrerequisite.push_back(prereqOne);
		}
		if (prereqTwo.length() > 3) {
			course.coursePrerequisite.push_back(prereqTwo);
		}
		// insert new course object in the bst
		bst->Insert(course);
	}
	
	// close file
	courseFile.close();
	cout << "Course file successfully loaded." << endl;
	
	return;
}

int main(int argc, char* argv[]) {

	// process command line arguments
	string filePath, courseId;
	switch (argc) {
	case 2:
		filePath = argv[1];
		break;
	case 3:
		filePath = argv[1];
		courseId = argv[2];
		break;
	default:
		filePath = "course_list.txt";
	}

	// Define a binary search tree to hold all bids
	BinarySearchTree* bst;
	bst = new BinarySearchTree();
	Course course;

	int choice = 0;
	while (choice != 9) {
		cout << "Menu:" << endl;
		cout << "  1. Load course data into BST." << endl;
		cout << "  2. Print Course List." << endl;
		cout << "  3. Print Course." << endl;
		cout << "  9. Exit" << endl;
		cout << "Enter choice:";
		cin >> choice;

		switch (choice) {

		case 1:
			// Complete the method call to load the bids
			readFile(filePath, bst);
			break;

		case 2:
			
			bst->InOrder();

			break;

		case 3:
			cout << "What course do you want to know about? ";
			cin >> courseId;
			course = bst->Search(courseId);

			if (!course.courseId.empty()) {
				displayCourse(course);
			}
			else {
				cout << "Course Id " << courseId << " not found." << endl;
			}

			break;
		}
	}

	cout << "Good bye." << endl;

	return 0;
}


