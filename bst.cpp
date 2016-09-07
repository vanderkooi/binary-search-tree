#include <iostream>
#include <vector>
#include <cstdlib>	// provides atoi()
#include <iomanip>	// provides std::setw()
#include <climits>
#include <sstream>
#include "unit.h"


/**
 * Defines the tree in a type agnostic way. Usually this would be done with
 * templates, but let's ignore those for today.
 */
typedef int KType;

/**
 * A tree node.
 */
struct Node {
	KType key;
	Node * left;
	Node * right;
};

/**
 * A visitor that can be used to traverse the tree. This is an abstract class;
 * you can't instantiate it directly.
 */
class Visitor {
public:
	virtual ~Visitor() { }
	virtual void visit(Node* node, int level) = 0;
};

/**
 * Creates a new Node with key=key, left=l, and right=r.
 */
Node* createNode( KType key, Node* l = NULL, Node* r = NULL ) {
	Node* result = new Node;
	result->key = key;
	result->left = l;
	result->right = r;
	return result;
}

/**
 * Deletes all nodes in the tree rooted at root and sets root to NULL.
 */
void deleteTree( Node*& root ) {
	if ( root != NULL ) {
		deleteTree( root->left );
		deleteTree( root->right );
		delete root;
		root = NULL;
	}
}

/**
 * Recursively find a node with key 'key'.
 */
Node *& find(KType key, Node *& r) {
	if (r == NULL) return r;
	if (key < r->key)
		return find(key, r->left);
	if (key > r->key)
		return find(key, r->right);
	return r;
}

Node* find_max(Node* root) {
	if (root == NULL) {
		return NULL;
	}

	else if (root->right == NULL) {
		return root;
	}

	else {
		return find_max(root->right);
  }
}

/**
 * Finds the parent of node in the tree rooted at rootNode
 */
Node* find_parent(Node* rootNode, Node* node) {
  if ( rootNode == NULL || rootNode == node ) {
    return NULL;
  }
  else if ( rootNode->left == node || rootNode->right == node ) {
    return rootNode;
  }
  else if (node->key < rootNode->key) {
    return find_parent(rootNode->left, node);
  }
  else {
    return find_parent(rootNode->right, node);
  }
}

/**
 * Deletes a node containing 'key' in the tree rooted at 'root'.
 */
bool delete_node(Node*& root, KType key) {

	// find target node to delete
	Node* target = find(key, root);
	if (!target) return false;

	// find parent of target
	Node* parent = find_parent(root, target);

	// case 1: target is a leaf
	if (target->left == NULL && target->right == NULL) {
		// set parent's child pointer
		if (parent != NULL) {
			if ( parent->left == target )
				parent->left = NULL;
			else
				parent->right = NULL;
		}
		else
			root = NULL;

		// free target
		delete target;
		return true;
	}

	// case 2: target has two children
	else if (target->left != NULL && target->right != NULL) {
		// finding predecessor and its parent
		Node* predecessor = find_max(target->left);
		Node* preParent = find_parent(root, predecessor);

		std::cout << target->key << " " << predecessor->key << " " << preParent->key << std::endl;

		predecessor->right = target->right;

		if (preParent == target) {
			if (parent != NULL) {
				parent->left = predecessor;
				delete target;
				return true;
			}

			else {
				root = predecessor;
				delete target;
				return true;
			}
		}

		else {
			preParent->right = predecessor->left;
			predecessor->left = preParent;
			if (parent != NULL) {
				if (parent->right == target)
					parent->right = predecessor;
				else 
					parent->left = predecessor;
			}

			else {
				root = predecessor;
				delete target;
				return true;
			}
		}
	}


	// case 3: target has only left child
	else if (target->left != NULL) {
		// set parent's child pointer
		if (parent != NULL) {
			if ( parent->left == target )
				parent->left = target->left;
			else
				parent->right = target->left;
		}
		else
			root = target->left;

		// free target
		delete target;
		return true;
	}

	// case 4: target has only right child
	else {
		// set parent's child pointer
		if (parent != NULL) {
			if (parent->left == target)
				parent->left = target->right;
			else
				parent->right = target->right;
		}
		else
			root = target->right;

		// free target
		delete target;
		return true;
	}
}

/**
 * Inserts key 'key' into the tree rooted at 'root'.
 */
void insert(KType key, Node *& root) {
	Node *& target = find(key, root);
	if( target != NULL ) {
		std::cerr << "Duplicate key \"" << key << "\" not inserted." << std::endl;
	} else {
		target = createNode(key);
	}
}

/**
 * Prints out the tree sideways.
 */
void printTree( Node * r, int d = 0 ) {
	if( r == NULL ) return;
	printTree( r->right, d+1 );
	std::cout << std::setw( 3 * d ) << ""; // output 3 * d spaces
	std::cout << r->key << std::endl;
	printTree( r->left, d+1 );
}

/**
 * Returns the number of nodes in the tree rooted at root.
 */
int numNodes(Node* root) {
	if (!root) {
		return 0;
	}

	else {
		return numNodes(root->left) + 1 + numNodes(root->right);
	}
}

/**
 * Returns the number of leaves in the tree rooted at root.
 */
int numLeaves( Node* root ) {
	if (!root) {
		return 0;
	}

	else if (!root->left && !root->right) {
		return 1;
	}

	else {
		return numLeaves(root->left) + numLeaves(root->right);
	}
}

/**
 * Returns the height of node x.
 */
int height( Node* x ) {
	if (!x) {
		return -1;
	}

	else {
		int left = height(x->left);
		int right = height(x->right);

		if (left>right) {
			return left + 1;
		}

		else {
			return right +1;
		}
	}
}


int find_depth(int data, Node* root, int level) {
	if (root == NULL) {
		return 0;
	}

	else if (root->key == data) {
		return level;
	}

	else {
		int curr = find_depth(data, root->left, level + 1);
		if (curr != 0)
			return curr;

		curr = find_depth(data, root->right, level + 1);
		return curr;
	}
}


/**
 * Returns the depth of node x in the tree rooted at root.
 */
int depth( Node* x , Node* root ) {
	int data = x->key;
	return find_depth(data, root, 0);
}

/**
 * Returns true if the tree rooted at root is a binary search tree
 */
bool isBstHelper(Node* root, int low, int high) {
	if (!root) {
		return true;
	}

	if (low < root->key && root->key < high) {
		return isBstHelper(root->left, low, root->key) &&
		isBstHelper(root->right, root->key, high);
	}

	else {
		return false;
	}
}

bool is_bst( Node* root ) {
	return isBstHelper(root, INT_MIN, INT_MAX);
}

/**
 * Traverse a tree rooted at rootNode in-order and use 'v' to visit each node.
 */
void in_order( Node*& rootNode, int level, Visitor& v ) {
	if (rootNode != NULL) {
	in_order(rootNode->left, level + 1, v);
	v.visit(rootNode, level);
	in_order(rootNode->right, level + 1, v);
}

}

/**
 * Traverse a tree rooted at rootNode pre-order and use 'v' to visit each node.
 */
void pre_order( Node*& rootNode, int level, Visitor& v ) {
	if (rootNode == NULL)
		return;

	v.visit(rootNode, level);
	pre_order(rootNode->left, level + 1, v);
	pre_order(rootNode->right, level + 1, v);

}


/**
 * Traverse a tree rooted at rootNode post-order and use 'v' to visit each node.
 */
void post_order( Node*& rootNode, int level, Visitor& v ) {
	if (rootNode != NULL) {

	post_order(rootNode->left, level, v);
	post_order(rootNode->right, level, v);
	v.visit(rootNode, level);
}
}




///////////////////////////////////////////////////////////////////////////////
// MAIN
///////////////////////////////////////////////////////////////////////////////

/**
 * Might be useful in your testing.
 */
class PrintVisitor : public Visitor {
  void visit(Node* node, int level) {
    std::cout << "Level " << level << ": ( " << node->key << " )" << std::endl;
  }
};

/**
 * Insert your own custom tests and print statements here to test your
 * implementation. These will be executed if you supply the tree keys on the
 * command line. For marking, you will execute the program without arguments,
 * and it will run the automated tests.
 */
void runMain(Node*& tree, std::vector<int> keys) {
	/*
	 * CHANGE THIS CODE ALL YOU LIKE. This is your test space.
	 */
	std::cout << "Full Tree:" << std::endl;
	printTree(tree);
	std::cout << std::endl;

	std::cout << "number of Nodes  = " << numNodes(tree) << std::endl;
	std::cout << "number of Leaves = " << numLeaves(tree) << std::endl;
	std::cout << "height of tree   = " << height(tree) << std::endl;
	std::cout << std::endl;
	for (unsigned int i = 0; i < keys.size(); ++i) {
		std::cout << "height(" << keys[i] << ") = " << height(find(keys[i], tree)) << std::endl;
		std::cout << "depth(" << keys[i] << ") = " << depth(find(keys[i], tree), tree) << std::endl;
	}
	std::cout << std::endl;

	PrintVisitor v;
	std::cout << "In-order traversal:" << std::endl;
	in_order(tree, 0, v);
}

///////////////////////////////////////////////////////////////////////////////
// DO NOT ALTER BEYOND THIS POINT
///////////////////////////////////////////////////////////////////////////////

/** values to use for tests */
int keysArr[] =		{5, 3, 2, 1, 6, 8, 4, 7, 9};
int heightArr[] =	{3, 2, 1, 0, 2, 1, 0, 0, 0};
int depthArr[] =	{0, 1, 2, 3, 1, 2, 2, 3, 3};
int inArr[] =		{1, 2, 3, 4, 5, 6, 7, 8, 9};
int preArr[] =		{5, 3, 2, 1, 4, 6, 8, 7, 9};
int postArr[] =		{1, 2, 4, 3, 7, 9, 8, 6, 5};
std::vector<int> keys(keysArr, keysArr + sizeof(keysArr) / sizeof(int));
std::vector<int> heights(heightArr, heightArr + sizeof(heightArr) / sizeof(int));
std::vector<int> depths(depthArr, depthArr + sizeof(depthArr) / sizeof(int));
std::vector<int> inOrder(inArr, inArr + sizeof(inArr) / sizeof(int));
std::vector<int> preOrder(preArr, preArr + sizeof(preArr) / sizeof(int));
std::vector<int> postOrder(postArr, postArr + sizeof(postArr) / sizeof(int));

class TestVisitor : public Visitor {
public:
	std::vector<int> q;
	void visit(Node* node, int level) {
		q.push_back(node->key);
	}
};

/**
 * Runs the tests for marking. You can run it yourself to check your solution.
 */
void runTests(Node*& tree, std::vector<int> keys) {
	Unit unit;
	TestVisitor v;

	std::cout << "Testing Tree:" << std::endl;
	printTree(tree);
	std::cout << std::endl;

	unit.assertEquals("Number of nodes", 9, numNodes(tree));
	unit.assertEquals("Number of leaves", 4, numLeaves(tree));
	unit.assertEquals("Height", 3, height(tree));
	unit.assertEquals("Is a BST", true, is_bst(tree));

  std::stringstream ss;
  std::string sstring;
	std::string h("height");
	std::string d("depth");
	for (unsigned int i = 0; i < keys.size(); ++i) {
    ss << "(" << keys[i] << ")";
    sstring = ss.str();
    unit.assertEquals(h + sstring, heights[i], height(find(keys[i], tree)));
    unit.assertEquals(d + sstring, depths[i], depth(find(keys[i], tree), tree));
    ss.str(std::string());
	}

	v.q.clear();
	in_order(tree, 0, v);
	unit.assertvectorEquals("In-order traversal", inOrder, v.q);
	v.q.clear();
	pre_order(tree, 0, v);
	unit.assertvectorEquals("Pre-order traversal", preOrder, v.q);
	v.q.clear();
	post_order(tree, 0, v);
	unit.assertvectorEquals("Post-order traversal", postOrder, v.q);

	//Delete a leaf
	unit.assertNonNull("Finding 9 (a leaf)", find(9, tree));
	delete_node(tree, 9);
	unit.assertNull("Deleting 9 (a leaf)", find(9, tree));
	unit.assertEquals("Number of nodes", 8, numNodes(tree));
	unit.assertEquals("Number of leaves", 3, numLeaves(tree));
	unit.assertEquals("Height", 3, height(tree));
	unit.assertEquals("Is a BST", true, is_bst(tree));

	//Delete a node with one child
	unit.assertNonNull("Finding 2 (a node with one child)", find(2, tree));
	delete_node(tree, 2);
	unit.assertNull("Deleting 2 (a node with one child)", find(2, tree));
	unit.assertEquals("Number of nodes", 7, numNodes(tree));
	unit.assertEquals("Number of leaves", 3, numLeaves(tree));
	unit.assertEquals("Height", 3, height(tree));
	unit.assertEquals("Is a BST", true, is_bst(tree));


	//Delete a node with two children
	unit.assertNonNull("Finding 3 (a node with two children)", find(3, tree));
	delete_node(tree, 3);
	unit.assertNull("Deleting 3 (a node with two children)", find(3, tree));
	unit.assertEquals("Number of nodes", 6, numNodes(tree));
	unit.assertEquals("Number of leaves", 2, numLeaves(tree));
	unit.assertEquals("Height", 3, height(tree));
	unit.assertEquals("Is a BST", true, is_bst(tree));

	//Delete root
	unit.assertNonNull("Finding 5 (the root)", find(5, tree));
	delete_node(tree, 5);
	unit.assertNull("Deleting 5 (the root)", find(5, tree));
	unit.assertEquals("Number of nodes", 5, numNodes(tree));
	unit.assertEquals("Number of leaves", 2, numLeaves(tree));
	unit.assertEquals("Height", 3, height(tree));
	unit.assertEquals("Is a BST", true, is_bst(tree));
	//Additional tests for delete, height, depth
	
	Node* tree2 = NULL;
	insert(7, tree2);
	insert(9, tree2);
	insert(5, tree2);
	insert(1, tree2);
	
	//Test height, depth
	Node* node_9 = find(9, tree2);
	unit.assertEquals("Height of 9", 0, height(node_9));
	unit.assertEquals("Depth of 9", 1, depth(node_9, tree2));
	Node* node_1 = find(1, tree2);
	unit.assertEquals("Height of 1", 0, height(node_1));
	unit.assertEquals("Depth of 1", 2, depth(node_1, tree2));
	std::cout << "Made it" << std::endl;
	
	// delete root with 2 children, left child of root is predecessor with 1 child
	delete_node(tree2, 7);

	unit.assertEquals("Number of nodes", 3, numNodes(tree2));
	unit.assertEquals("Number of leaves", 2, numLeaves(tree2));
	unit.assertEquals("Height", 1, height(tree2));
	unit.assertEquals("Is a BST", true, is_bst(tree2));
	
    // delete root with 2 children, left child of root is not predecessor, predecessor has 1 child
	insert(4, tree2);
	insert(2, tree2);
	delete_node(tree2, 5);
	unit.assertEquals("Number of nodes", 4, numNodes(tree2));
	unit.assertEquals("Number of leaves", 2, numLeaves(tree2));
	unit.assertEquals("Height", 2, height(tree2));
	unit.assertEquals("Is a BST", true, is_bst(tree2));


	std::cout << std::endl;
	unit.printResults();
}

/**
 * Runs custom code if given command line arguments, otherwise runs tests.
 */
int main( int argc, char *argv[] ) {
	Node *tree = NULL;

	bool custom = argc > 1;
	if (custom) {	// if keys given on command line, run tests
		keys.clear();
		for (int i=0; i < argc-1; ++i) {
			keys.push_back(atoi(argv[i+1]));
		}
	}

	for (unsigned int i=0; i < keys.size(); ++i) {
		insert(keys[i], tree);
	}

	if (custom) {
		runMain(tree, keys);
	} else {
		runTests(tree, keys);
	}

	// Free up allocated memory
	deleteTree(tree);
#ifdef _WIN32
  system("pause");
#endif
	return 0;
}

