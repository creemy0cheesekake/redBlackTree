#include <iostream>

#include "red_black_tree.hpp"

int main() {
	std::cout << std::boolalpha;
	RedBlackTree<int> tree;
	std::srand(time(0));
	for (size_t i = 0; i < 27; i++) {
		int num;
		do {
			num = std::rand() % 1000;
		} while (tree.exists(num));
		tree.insert(num);
	}
	tree.print();
	std::cout << "\n---\n"
			  << std::endl;
	tree.printInOrderTraversal();

	RedBlackTree<std::string> strTree;
	strTree.insert("quick");
	strTree.insert("brown");
	strTree.insert("fox");
	strTree.insert("jumped");
	strTree.insert("over");
	strTree.insert("the");
	strTree.insert("lazy");
	strTree.insert("river");
	strTree.insert("apple");
	strTree.insert("cat");
	strTree.insert("bat");
	strTree.print();
	std::cout << "\n---\n"
			  << std::endl;
	strTree.printInOrderTraversal();
}
