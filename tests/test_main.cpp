#include "../include/doctest.h"
#include "../src/red_black_tree.hpp"
#include <algorithm>
#include <random>
#include <stdexcept>
#include <vector>

TEST_CASE("Constructor and basic properties") {
	RedBlackTree<int> tree;

	SUBCASE("New tree is empty") {
		CHECK(tree.isEmpty());
		CHECK(tree.size() == 0);
	}

	SUBCASE("Root is nullptr for empty tree") {
		CHECK(tree.validateTree());
	}
}

TEST_CASE("Insertion") {
	RedBlackTree<int> tree;

	SUBCASE("Single insertion") {
		tree.insert(10);
		CHECK(tree.size() == 1);
		CHECK_FALSE(tree.isEmpty());
		CHECK(tree.exists(10));
		CHECK(tree.validateTree());
		CHECK(tree.inOrderTraversal() == std::vector<int>{10});
	}

	SUBCASE("Multiple insertions maintain order") {
		tree.insert(10);
		tree.insert(5);
		tree.insert(15);
		CHECK(tree.size() == 3);
		CHECK(tree.inOrderTraversal() == std::vector<int>{5, 10, 15});
		CHECK(tree.validateTree());
	}

	SUBCASE("Insert in ascending order") {
		for (int i = 1; i <= 10; i++) {
			tree.insert(i);
		}
		CHECK(tree.size() == 10);
		CHECK(tree.validateTree());
		auto traversal			  = tree.inOrderTraversal();
		std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		CHECK(traversal == expected);
	}

	SUBCASE("Insert in descending order") {
		for (int i = 10; i >= 1; i--) {
			tree.insert(i);
		}
		CHECK(tree.size() == 10);
		CHECK(tree.validateTree());
		auto traversal			  = tree.inOrderTraversal();
		std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		CHECK(traversal == expected);
	}

	SUBCASE("Insert duplicates throws exception") {
		tree.insert(10);
		CHECK_THROWS_AS(tree.insert(10), std::logic_error);
		CHECK(tree.size() == 1);
	}
}

TEST_CASE("Existence checking") {
	RedBlackTree<int> tree;

	SUBCASE("Exists returns false for empty tree") {
		CHECK_FALSE(tree.exists(10));
	}

	SUBCASE("Exists returns false for non-existent element") {
		tree.insert(10);
		tree.insert(20);
		CHECK_FALSE(tree.exists(15));
	}

	SUBCASE("Exists returns true for inserted elements") {
		tree.insert(10);
		tree.insert(20);
		tree.insert(30);
		CHECK(tree.exists(10));
		CHECK(tree.exists(20));
		CHECK(tree.exists(30));
	}
}

TEST_CASE("Removal") {
	RedBlackTree<int> tree;

	SUBCASE("Remove leaf node") {
		tree.insert(10);
		tree.insert(5);
		tree.insert(15);
		tree.remove(5);
		CHECK(tree.size() == 2);
		CHECK_FALSE(tree.exists(5));
		CHECK(tree.exists(10));
		CHECK(tree.exists(15));
		CHECK(tree.validateTree());
		CHECK(tree.inOrderTraversal() == std::vector<int>{10, 15});
	}

	SUBCASE("Remove node with one right child") {
		tree.insert(10);
		tree.insert(5);
		tree.insert(15);
		tree.insert(20);
		tree.remove(15);
		CHECK(tree.size() == 3);
		CHECK_FALSE(tree.exists(15));
		CHECK(tree.exists(10));
		CHECK(tree.exists(5));
		CHECK(tree.exists(20));
		CHECK(tree.validateTree());
		CHECK(tree.inOrderTraversal() == std::vector<int>{5, 10, 20});
	}

	SUBCASE("Remove node with one left child") {
		tree.insert(10);
		tree.insert(5);
		tree.insert(15);
		tree.insert(3);
		tree.remove(5);
		CHECK(tree.size() == 3);
		CHECK_FALSE(tree.exists(5));
		CHECK(tree.exists(10));
		CHECK(tree.exists(3));
		CHECK(tree.exists(15));
		CHECK(tree.validateTree());
		CHECK(tree.inOrderTraversal() == std::vector<int>{3, 10, 15});
	}

	SUBCASE("Remove node with two children") {
		tree.insert(10);
		tree.insert(5);
		tree.insert(15);
		tree.insert(3);
		tree.insert(7);
		tree.insert(12);
		tree.insert(20);
		tree.remove(10);
		CHECK(tree.size() == 6);
		CHECK_FALSE(tree.exists(10));
		CHECK(tree.validateTree());
		auto traversal			  = tree.inOrderTraversal();
		std::vector<int> expected = {3, 5, 7, 12, 15, 20};
		CHECK(traversal == expected);
	}

	SUBCASE("Remove root") {
		tree.insert(10);
		tree.insert(5);
		tree.insert(15);
		tree.remove(10);
		CHECK(tree.size() == 2);
		CHECK_FALSE(tree.exists(10));
		CHECK(tree.exists(5));
		CHECK(tree.exists(15));
		CHECK(tree.validateTree());
	}

	SUBCASE("Remove all elements") {
		for (int i = 1; i <= 5; i++) {
			tree.insert(i);
		}
		for (int i = 1; i <= 5; i++) {
			tree.remove(i);
		}
		CHECK(tree.size() == 0);
		CHECK(tree.isEmpty());
		CHECK(tree.validateTree());
	}

	SUBCASE("Remove non-existent throws exception") {
		tree.insert(10);
		CHECK_THROWS_AS(tree.remove(999), std::logic_error);
	}

	SUBCASE("Remove from empty tree throws") {
		CHECK_THROWS_AS(tree.remove(10), std::logic_error);
	}
}

TEST_CASE("Red-black tree properties") {
	RedBlackTree<int> tree;

	SUBCASE("Root is always black after insertions") {
		tree.insert(10);
		CHECK(tree.validateTree());

		tree.insert(5);
		CHECK(tree.validateTree());

		tree.insert(15);
		CHECK(tree.validateTree());
	}

	SUBCASE("No two consecutive red nodes") {
		std::vector<int> values = {10, 5, 15, 3, 7, 12, 20, 1, 4, 6, 8};
		for (int v : values) {
			tree.insert(v);
		}
		CHECK(tree.validateTree());
	}

	SUBCASE("All paths have same black height") {
		std::vector<int> values = {41, 38, 31, 12, 19, 8};
		for (int v : values) {
			tree.insert(v);
		}
		CHECK(tree.validateTree());
	}

	SUBCASE("Tree remains valid after multiple removals") {
		std::vector<int> values = {10, 5, 15, 3, 7, 12, 20};
		for (int v : values) {
			tree.insert(v);
		}
		CHECK(tree.validateTree());

		tree.remove(3);
		CHECK(tree.validateTree());

		tree.remove(15);
		CHECK(tree.validateTree());

		tree.remove(10);
		CHECK(tree.validateTree());
	}
}

TEST_CASE("In-order traversal") {
	RedBlackTree<int> tree;

	SUBCASE("Empty tree traversal") {
		CHECK(tree.inOrderTraversal() == std::vector<int>{});
	}

	SUBCASE("Single element traversal") {
		tree.insert(10);
		CHECK(tree.inOrderTraversal() == std::vector<int>{10});
	}

	SUBCASE("Multiple elements sorted") {
		std::vector<int> values = {5, 3, 7, 1, 9, 2, 8, 4, 6};
		for (int v : values) {
			tree.insert(v);
		}
		auto traversal			= tree.inOrderTraversal();
		std::vector<int> sorted = values;
		std::sort(sorted.begin(), sorted.end());
		CHECK(traversal == sorted);
	}

	SUBCASE("Traversal after removals") {
		for (int i = 1; i <= 7; i++) {
			tree.insert(i);
		}
		tree.remove(3);
		tree.remove(5);
		CHECK(tree.inOrderTraversal() == std::vector<int>{1, 2, 4, 6, 7});
	}
}

TEST_CASE("Stress tests") {
	RedBlackTree<int> tree;

	SUBCASE("Insert many sequential values") {
		for (int i = 0; i < 100; i++) {
			tree.insert(i);
		}
		CHECK(tree.size() == 100);
		CHECK(tree.validateTree());
		auto traversal = tree.inOrderTraversal();
		std::vector<int> expected(100);
		for (int i = 0; i < 100; i++) expected[i] = i;
		CHECK(traversal == expected);
	}

	SUBCASE("Insert and remove many values") {
		std::vector<int> values = {50, 25, 75, 12, 37, 62, 87, 6, 18, 31, 43, 56, 68, 81, 93};
		for (int v : values) {
			tree.insert(v);
		}
		CHECK(tree.validateTree());

		tree.remove(25);
		CHECK(tree.validateTree());
		tree.remove(12);
		CHECK(tree.validateTree());
		tree.remove(75);
		CHECK(tree.validateTree());
		tree.remove(50);
		CHECK(tree.validateTree());

		CHECK(tree.size() == 11);
	}

	SUBCASE("Random insertions and deletions") {
		std::vector<int> values(50);
		for (int i = 0; i < 50; i++) values[i] = i;
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(values.begin(), values.end(), g);

		for (int v : values) {
			tree.insert(v);
		}
		CHECK(tree.validateTree());
		CHECK(tree.size() == 50);

		std::shuffle(values.begin(), values.end(), g);
		for (int i = 0; i < 25; i++) {
			tree.remove(values[i]);
			CHECK(tree.validateTree());
		}
		CHECK(tree.size() == 25);
	}
}

TEST_CASE("Specific rotation cases") {
	RedBlackTree<int> tree;

	SUBCASE("Left-Left case (triggers right rotation)") {
		tree.insert(30);
		tree.insert(20);
		tree.insert(10);
		CHECK(tree.validateTree());
		CHECK(tree.inOrderTraversal() == std::vector<int>{10, 20, 30});
	}

	SUBCASE("Right-Right case (triggers left rotation)") {
		tree.insert(10);
		tree.insert(20);
		tree.insert(30);
		CHECK(tree.validateTree());
		CHECK(tree.inOrderTraversal() == std::vector<int>{10, 20, 30});
	}

	SUBCASE("Left-Right case (triggers left then right rotation)") {
		tree.insert(30);
		tree.insert(10);
		tree.insert(20);
		CHECK(tree.validateTree());
		CHECK(tree.inOrderTraversal() == std::vector<int>{10, 20, 30});
	}

	SUBCASE("Right-Left case (triggers right then left rotation)") {
		tree.insert(10);
		tree.insert(30);
		tree.insert(20);
		CHECK(tree.validateTree());
		CHECK(tree.inOrderTraversal() == std::vector<int>{10, 20, 30});
	}
}

TEST_CASE("Edge cases") {
	RedBlackTree<int> tree;

	SUBCASE("Single element insert and remove") {
		tree.insert(42);
		CHECK(tree.size() == 1);
		CHECK(tree.exists(42));
		tree.remove(42);
		CHECK(tree.size() == 0);
		CHECK_FALSE(tree.exists(42));
		CHECK(tree.validateTree());
	}

	SUBCASE("Negative numbers") {
		tree.insert(-5);
		tree.insert(-10);
		tree.insert(-3);
		CHECK(tree.validateTree());
		CHECK(tree.inOrderTraversal() == std::vector<int>{-10, -5, -3});
	}

	SUBCASE("Mixed positive and negative") {
		tree.insert(10);
		tree.insert(-5);
		tree.insert(0);
		tree.insert(-10);
		tree.insert(5);
		CHECK(tree.validateTree());
		CHECK(tree.inOrderTraversal() == std::vector<int>{-10, -5, 0, 5, 10});
	}
}
