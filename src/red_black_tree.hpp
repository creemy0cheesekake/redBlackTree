#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <cstddef>
#include <vector>

enum Color {
	RED,
	BLACK,
};

template <typename T>
class RedBlackTree {
	class TreeNode {
		friend struct RedBlackTree<T>;
		T data;
		Color color;
		TreeNode* left;
		TreeNode* right;
		TreeNode* parent;

		TreeNode() : data(T{}), color(BLACK), left(nullptr), right(nullptr), parent(nullptr) {}								// constructor for NIL
		TreeNode(T _data, Color _color, TreeNode* nil) : data(_data), color(_color), left(nil), right(nil), parent(nil) {}	// constructor for all other nodes
	};

private:
	TreeNode* NIL{nullptr};
	TreeNode* m_root{nullptr};
	size_t m_size{0};

	void rotateLeft(TreeNode*);
	void rotateRight(TreeNode*);
	void printHelper(TreeNode*, int space = 10);
	void recursiveDelete(TreeNode*);
	TreeNode* find(T data);
	void removeFixup(TreeNode* replacementNode, TreeNode* parent);
	bool validateHelper(TreeNode* node, int currentBlackCount, int& expectedBlackHeight);
	void remove(TreeNode* node);

public:
	RedBlackTree();
	~RedBlackTree();
	RedBlackTree(RedBlackTree&&)				 = delete;
	RedBlackTree& operator=(RedBlackTree&&)		 = delete;
	RedBlackTree(const RedBlackTree&)			 = delete;
	RedBlackTree& operator=(const RedBlackTree&) = delete;

	void insert(T data);
	void remove(T data);
	bool exists(T data);
	void print();

	constexpr bool isEmpty() const { return m_size == 0; }
	constexpr size_t size() const { return m_size; }
	std::vector<T> inOrderTraversal();
	void printInOrderTraversal();
	bool validateTree();
};

#include "red_black_tree.ipp"
#endif
