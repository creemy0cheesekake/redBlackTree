#include <functional>
#include <iostream>
#include <stdexcept>

template <typename T>
RedBlackTree<T>::RedBlackTree() {
	NIL	   = new TreeNode();
	m_root = NIL;
}
template <typename T>
RedBlackTree<T>::~RedBlackTree() {
	recursiveDelete(m_root);
	delete NIL;
}

template <typename T>
void RedBlackTree<T>::recursiveDelete(TreeNode* node) {
	if (node == NIL) return;
	recursiveDelete(node->left);
	recursiveDelete(node->right);
	delete node;
}

template <typename T>
void RedBlackTree<T>::insert(T data) {
	if (m_size == 0) {
		m_root = new TreeNode(data, BLACK, NIL);
		m_size++;
		return;
	}

	TreeNode* parent = NIL;
	TreeNode* curr	 = m_root;
	while (curr != NIL) {
		parent = curr;
		if (data < curr->data) curr = curr->left;
		else if (data > curr->data) curr = curr->right;
		else throw std::logic_error("Cannot have duplicates in BST");
	}
	curr		 = new TreeNode(data, RED, NIL);
	curr->parent = parent;
	if (data < parent->data) parent->left = curr;
	else parent->right = curr;

	while (curr->color == RED && curr->parent->color == RED) {
		parent			  = curr->parent;
		TreeNode* grandpa = parent->parent;

		if (parent == grandpa->left) {
			TreeNode* uncle = grandpa->right;
			if (uncle->color == RED) {
				grandpa->color = RED;
				uncle->color   = BLACK;
				parent->color  = BLACK;
				curr		   = grandpa;
			} else {
				if (curr == parent->right) {
					// left triangle
					rotateLeft(parent);
					// realign pointers after rotation
					std::swap(curr, parent);
				}
				// left line
				std::swap(parent->color, grandpa->color);
				rotateRight(grandpa);
			}
		} else {
			TreeNode* uncle = grandpa->left;
			if (uncle->color == RED) {
				grandpa->color = RED;
				uncle->color   = BLACK;
				parent->color  = BLACK;
				curr		   = grandpa;
			} else {
				if (curr == parent->left) {
					// right triangle
					rotateRight(parent);
					// realign pointers after rotation
					std::swap(curr, parent);
				}
				// right line
				std::swap(parent->color, grandpa->color);
				rotateLeft(grandpa);
			}
		}
	}

	m_root->color = BLACK;
	m_size++;
}

template <typename T>
void RedBlackTree<T>::rotateLeft(TreeNode* node) {
	TreeNode* newParent = node->right;
	newParent->parent	= node->parent;

	if (node->parent != NIL) {
		bool leftChild = node->parent->left == node;
		if (leftChild) node->parent->left = newParent;
		else node->parent->right = newParent;
	} else {
		m_root = newParent;
	}

	node->right = newParent->left;
	if (newParent->left != NIL)
		newParent->left->parent = node;
	newParent->left = node;
	node->parent	= newParent;
}

template <typename T>
void RedBlackTree<T>::rotateRight(TreeNode* node) {
	TreeNode* newParent = node->left;
	newParent->parent	= node->parent;

	if (node->parent != NIL) {
		bool leftChild = node->parent->left == node;
		if (leftChild) node->parent->left = newParent;
		else node->parent->right = newParent;
	} else {
		m_root = newParent;
	}

	node->left = newParent->right;
	if (newParent->right != NIL)
		newParent->right->parent = node;
	newParent->right = node;
	node->parent	 = newParent;
}

template <typename T>
void RedBlackTree<T>::remove(TreeNode* node) {
	if (node == nullptr || node == NIL) return;

	TreeNode* x					= NIL;
	TreeNode* replacementParent = node->parent;
	bool needsFixing			= (node->color == BLACK);

	if (node->left == NIL && node->right == NIL) {
		if (node == m_root) {
			m_root = NIL;
		} else {
			if (node->parent->left == node) {
				node->parent->left = NIL;
			} else {
				node->parent->right = NIL;
			}
			if (needsFixing) {
				removeFixup(NIL, replacementParent);
			}
		}
		delete node;
		return;
	} else if (node->left == NIL) {
		x = node->right;
		if (node == m_root) {
			m_root = x;
		} else if (node->parent->left == node) {
			node->parent->left = x;
		} else {
			node->parent->right = x;
		}
		x->parent = replacementParent;

		if (needsFixing) {
			if (x->color == RED) x->color = BLACK;
			else removeFixup(x, x->parent);
		}
		delete node;
		return;
	} else if (node->right == NIL) {
		x = node->left;
		if (node == m_root) {
			m_root = x;
		} else if (node->parent->left == node) {
			node->parent->left = x;
		} else {
			node->parent->right = x;
		}
		x->parent = replacementParent;

		if (needsFixing) {
			if (x->color == RED) x->color = BLACK;
			else removeFixup(x, x->parent);
		}
		delete node;
		return;
	} else {
		// largest in left subtree
		TreeNode* replacementVal = node->left;
		while (replacementVal->right != NIL) replacementVal = replacementVal->right;
		// copy
		node->data = replacementVal->data;
		// remove
		remove(replacementVal);
	}
}

template <typename T>
void RedBlackTree<T>::removeFixup(TreeNode* replacementNode, TreeNode* parent) {
	while (replacementNode != m_root && (replacementNode == NIL || replacementNode->color == BLACK)) {
		if (replacementNode == parent->left) {
			TreeNode* sibling = parent->right;
			if (sibling->color == RED) {
				sibling->color = BLACK;
				parent->color  = RED;
				rotateLeft(parent);
				sibling = parent->right;
			}
			if ((sibling->left == NIL || sibling->left->color == BLACK) && (sibling->right == NIL || sibling->right->color == BLACK)) {
				sibling->color	= RED;
				replacementNode = parent;
				parent			= replacementNode->parent;
			} else {
				if (sibling->right == NIL || sibling->right->color == BLACK) {
					sibling->left->color = BLACK;
					sibling->color		 = RED;
					rotateRight(sibling);
					sibling = parent->right;
				}
				sibling->color = parent->color;
				parent->color  = BLACK;
				if (sibling->right != NIL) sibling->right->color = BLACK;
				rotateLeft(parent);
				replacementNode = m_root;  // break the loop
			}
		} else {
			TreeNode* sibling = parent->left;
			if (sibling->color == RED) {
				sibling->color = BLACK;
				parent->color  = RED;
				rotateRight(parent);
				sibling = parent->left;
			}
			if ((sibling->right == NIL || sibling->right->color == BLACK) &&
				(sibling->left == NIL || sibling->left->color == BLACK)) {
				sibling->color	= RED;
				replacementNode = parent;
				parent			= replacementNode->parent;
			} else {
				if (sibling->left == NIL || sibling->left->color == BLACK) {
					sibling->right->color = BLACK;
					sibling->color		  = RED;
					rotateLeft(sibling);
					sibling = parent->left;
				}

				sibling->color = parent->color;
				parent->color  = BLACK;
				if (sibling->left != NIL) sibling->left->color = BLACK;
				rotateRight(parent);
				replacementNode = m_root;
			}
		}
	}
	if (replacementNode != NIL) replacementNode->color = BLACK;
}

template <typename T>
void RedBlackTree<T>::remove(T data) {
	if (!exists(data)) throw std::logic_error(std::to_string(data) + "doesn't exist in the tree.");
	remove(find(data));
	m_size--;
}

template <typename T>
bool RedBlackTree<T>::exists(T data) {
	return find(data) != nullptr;
}

template <typename T>
void RedBlackTree<T>::printHelper(TreeNode* root, int space) {
	if (root == NIL) return;
	int COUNT = 10;
	space += COUNT;
	printHelper(root->right, space);
	std::cout << std::endl;
	for (int i = COUNT; i < space; i++) {
		std::cout << " ";
	}
	if (root->color == RED) std::cout << "\033[41;37m" << root->data << "\033[0m" << std::endl;
	else std::cout << "\033[40;37m" << root->data << "\033[0m" << std::endl;
	printHelper(root->left, space);
}

template <typename T>
void RedBlackTree<T>::print() {
	TreeNode* p = m_root;
	printHelper(p, 0);
}

template <typename T>
std::vector<T> RedBlackTree<T>::inOrderTraversal() {
	std::vector<T> res;
	std::function<void(TreeNode*)> traverse = [&](TreeNode* root) {
		if (root == NIL) return;
		traverse(root->left);
		res.push_back(root->data);
		traverse(root->right);
	};
	traverse(m_root);
	return res;
}

template <typename T>
typename std::enable_if_t<std::is_arithmetic_v<T>, std::string>
ensure_string(T val) {
	return std::to_string(val);
}

inline std::string ensure_string(const std::string& val) {
	return val;
}

template <typename T>
typename std::enable_if_t<!std::is_arithmetic_v<T> && !std::is_same_v<T, std::string>, std::string>
ensure_string(T val) {
	static_assert(std::is_same_v<T, std::string> || std::is_arithmetic_v<T>, "Not supported type");
}

template <typename T>
void RedBlackTree<T>::printInOrderTraversal() {
	for (T n : inOrderTraversal()) {
		std::cout << ensure_string(n) << " ";
	}
	std::cout << std::endl;
}

template <typename T>
typename RedBlackTree<T>::TreeNode* RedBlackTree<T>::find(T data) {
	TreeNode* curr = m_root;
	while (curr != NIL) {
		if (data < curr->data) {
			curr = curr->left;
		} else if (data > curr->data) {
			curr = curr->right;
		} else {
			return curr;
		}
	}
	return nullptr;
}

template <typename T>
bool RedBlackTree<T>::validateTree() {
	if (m_root == NIL || m_root == nullptr) {
		return true;
	}
	if (m_root->color != BLACK) return false;
	int expectedBlackHeight = -1;
	return validateHelper(m_root, 0, expectedBlackHeight);
}

template <typename T>
bool RedBlackTree<T>::validateHelper(TreeNode* node, int currentBlackCount, int& expectedBlackHeight) {
	if (node == NIL) {
		currentBlackCount++;
		if (expectedBlackHeight == -1) {
			expectedBlackHeight = currentBlackCount;
			return true;
		}
		return currentBlackCount == expectedBlackHeight;
	}
	if (node->color == RED) {
		if (node->left->color == RED || node->right->color == RED) {
			return false;
		}
	}
	if (node->color == BLACK) {
		currentBlackCount++;
	}
	return validateHelper(node->left, currentBlackCount, expectedBlackHeight) &&
		validateHelper(node->right, currentBlackCount, expectedBlackHeight);
}
