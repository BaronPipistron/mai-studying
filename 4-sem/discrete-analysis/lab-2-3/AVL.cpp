#include <algorithm>
#include <cctype>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <utility>
#include <string>

namespace avl {

template <typename KeyTp, typename ValueTp>
class AVL {
    struct Node {
        Node* left_ = nullptr;
        Node* right_ = nullptr;
        KeyTp key_;
        ValueTp value_;
		int8_t height_;

		Node(const KeyTp& key__, const ValueTp& value__) : 
			 left_{nullptr}, right_{nullptr}, key_(key__), value_(value__), height_(1) {}
    };
  
  public:  // methods
	bool insert(const KeyTp& key, const ValueTp& value);
	bool remove(const KeyTp& key);

	std::pair<bool, ValueTp> find(const KeyTp& key);

	bool save(std::ofstream& ofs);
	bool load(std::ifstream& ifs);

	void swap(AVL<KeyTp, ValueTp>& other);
	void clear();
    virtual ~AVL() noexcept;

  private: // methods
	Node* insert(Node* node, Node* new_node);
	Node* remove(Node* node, const KeyTp& key);
	bool save(Node* node, std::ofstream& ofs);
	void destroyTree(Node* node);

	Node* left_Rotation(Node* node);
	Node* rightRotation(Node* node);

	int8_t height(Node* node);
	void reHeight(Node* node);

	int8_t balanceFactor(Node* node);
	Node* balance(Node* node);

	Node* findMin(Node* node);
	Node* removeMin(Node* node);

  private: // fields
    Node* root_ = nullptr;
};

// public methodss

template <typename KeyTp, typename ValueTp>
bool AVL<KeyTp, ValueTp>::insert(const KeyTp& key, const ValueTp& value) {
	if (find(key).first == true) return false;

	root_ = insert(root_, new Node(key, value));
	return true;
}

template <typename KeyTp, typename ValueTp>
bool AVL<KeyTp, ValueTp>::remove(const KeyTp& key) {
	if (find(key).first == false) return false;

	root_ = remove(root_, key);
	return true;
}

template <typename KeyTp, typename ValueTp>
std::pair<bool, ValueTp> AVL<KeyTp, ValueTp>::find(const KeyTp& key) {
	Node* node = root_;

	while (node != nullptr) {
		if (node->key_ == key) {
			return std::make_pair(true, node->value_);
		}

		node = (key < node->key_) ? node->left_ : node->right_;
	}

	return std::make_pair(false, ValueTp());
}

template <typename KeyTp, typename ValueTp>
bool AVL<KeyTp, ValueTp>::save(std::ofstream& ofs) {
	return save(root_, ofs);
}

template <typename KeyTp, typename ValueTp>
bool AVL<KeyTp, ValueTp>::load(std::ifstream& ifs) {
	clear();

	KeyTp key = KeyTp();
	ValueTp value = ValueTp();

	while (ifs >> key >> value) {
		insert(key, value);
	}

	return true;
}

template <typename KeyTp, typename ValueTp>
void AVL<KeyTp, ValueTp>::swap(AVL<KeyTp, ValueTp>& other) {
	Node* tmp = root_;
	root_ = other.root_;
	other.root_ = tmp;
}

template <typename KeyTp, typename ValueTp>
void AVL<KeyTp, ValueTp>::clear() {
	destroyTree(root_);
	root_ = nullptr;
}

template <typename KeyTp, typename ValueTp>
AVL<KeyTp, ValueTp>::~AVL() noexcept {
	destroyTree(root_);
}

// private methods

template <typename KeyTp, typename ValueTp>
typename AVL<KeyTp, ValueTp>::Node* AVL<KeyTp, ValueTp>::insert(Node* node, Node* new_node) {
	if (node == nullptr) return new_node;

	if (new_node->key_ < node->key_) {
		Node* tmp = insert(node->left_, new_node);

		if (tmp == nullptr) return nullptr;

		node->left_ = tmp;
	} else if (new_node->key_ > node->key_) {
		Node* tmp = insert(node->right_, new_node);

		if (tmp == nullptr) return nullptr;

		node->right_ = tmp;
	} else {
		return nullptr;
	}

	reHeight(node);
	return balance(node);
}

template <typename KeyTp, typename ValueTp>
typename AVL<KeyTp, ValueTp>::Node* AVL<KeyTp, ValueTp>::remove(Node* node, const KeyTp& key) {
	if (node == nullptr) return nullptr;

	if (key < node->key_) {
		Node* tmp = remove(node->left_, key);
		node->left_ = tmp;
	} else if (key > node->key_) {
		Node* tmp = remove(node->right_, key);
		node->right_ = tmp;
	} else {
		if (node->right_ == nullptr) {
			Node* tmp = node->left_;
			delete node;
			return tmp;
		} else if (node->left_ == nullptr) {
			Node* tmp = node->right_;
			delete node;
			return tmp;
		} else {
			Node* min = findMin(node->right_);

			node->key_ = min->key_;
			node->value_ = min->value_;

			node->right_ = removeMin(node->right_);
			delete min;
		}
	}

	reHeight(node);
	return balance(node);
}

template <typename KeyTp, typename ValueTp>
bool AVL<KeyTp, ValueTp>::save(Node* node, std::ofstream& ofs) {
	if (node == nullptr) return true;

	int isSuccessful = 1;
	isSuccessful &= save(node->left_, ofs);
	ofs << node->key_ << " " << node->value_ << std::endl;
	isSuccessful &= save(node->right_, ofs);

	return isSuccessful;
}

template <typename KeyTp, typename ValueTp>
void AVL<KeyTp, ValueTp>::destroyTree(Node* node) {
	if (node == nullptr) return;

	destroyTree(node->left_);
	destroyTree(node->right_);

	delete node;
}

template <typename KeyTp, typename ValueTp>
typename AVL<KeyTp, ValueTp>::Node* AVL<KeyTp, ValueTp>::left_Rotation(Node* node) {
	Node* tmp = node->right_;
	node->right_ = tmp->left_;
	tmp->left_ = node;

	reHeight(node);
	reHeight(tmp);

	return tmp;
}

template <typename KeyTp, typename ValueTp>
typename AVL<KeyTp, ValueTp>::Node* AVL<KeyTp, ValueTp>::rightRotation(Node* node) {
	Node* tmp = node->left_;
	node->left_ = tmp->right_;
	tmp->right_ = node;

	reHeight(node);
	reHeight(tmp);

	return tmp;
}

template <typename KeyTp, typename ValueTp>
int8_t AVL<KeyTp, ValueTp>::height(Node* node) {
	return (node == nullptr) ? 0 : node->height_;
}

template <typename KeyTp, typename ValueTp>
void AVL<KeyTp, ValueTp>::reHeight(Node* node) {
	node->height_ = std::max(height(node->left_), height(node->right_)) + 1;
}

template <typename KeyTp, typename ValueTp>
int8_t AVL<KeyTp, ValueTp>::balanceFactor(Node* node) {
	return height(node->right_) - height(node->left_);
}

template <typename KeyTp, typename ValueTp>
typename AVL<KeyTp, ValueTp>::Node* AVL<KeyTp, ValueTp>::balance(Node* node) {
	int8_t bFactor = balanceFactor(node);

	if (bFactor < -1) {
		if (balanceFactor(node->left_) > 0) {
			node->left_ = left_Rotation(node->left_);
		}

		node = rightRotation(node);
	} else if (bFactor > 1) {
		if (balanceFactor(node->right_) < 0) {
			node->right_ = rightRotation(node->right_);
		}

		node = left_Rotation(node);
	}

	reHeight(node);
	return node;
}

template <typename KeyTp, typename ValueTp>
typename AVL<KeyTp, ValueTp>::Node* AVL<KeyTp, ValueTp>::findMin(Node* node) {
	while (node->left_ != nullptr) {
		node = node->left_;
	}

	return node;
}

template <typename KeyTp, typename ValueTp>
typename AVL<KeyTp, ValueTp>::Node* AVL<KeyTp, ValueTp>::removeMin(Node* node) {
	if (node->left_ == nullptr) return node->right_;

	node->left_ = removeMin(node->left_);
	return balance(node);
}

}; // namespace avl

int main() {
	std::ios_base::sync_with_stdio(false);

	avl::AVL<std::string, uint64_t> avlTree;

	std::string command1;
	std::string command2;

	while (true) {
		std::cin >> std::ws;
		if (std::cin.eof()){
			break;
		}

		std::cin >> command1;
		if (command1 == "+") {
			std::cin >> command2;
			if (!isalpha(command2[0])) {
				continue;
			}

			std::transform(command2.begin(), command2.end(), command2.begin(),
    					   [](unsigned char c){ return std::tolower(c); });

			uint64_t value;
			std::cin >> value;
			if (avlTree.insert(command2, value)) {
				std::cout << "OK" << std::endl;
			} else{
				std::cout << "Exist" << std::endl;
			}
		} else if (command1 == "-") {
			std::cin >> command2;
			if (!isalpha(command2[0])) {
				continue;
			}

			std::transform(command2.begin(), command2.end(), command2.begin(),
    					   [](unsigned char c){ return std::tolower(c); });

			if (avlTree.remove(command2)) {
				std::cout << "OK" << std::endl;
			} else {
				std::cout << "NoSuchWord" << std::endl;
			}
		} else if (command1 == "!") {
			std::cin >> command2;

			if (command2 == "Save") {
				std::string path_to_file;
				std::cin >> path_to_file;

				std::ofstream ofs(path_to_file, std::ios::binary);
				if (ofs) {
					avlTree.save(ofs);
					std::cout << "OK" << std::endl;
				} else {
					std::cout << "ERROR: Couldn't create file" << std::endl;
				}
			} else if (command2 == "Load") {
				std::string path_to_file;
				std::cin >> path_to_file;

				std::ifstream ifs(path_to_file, std::ios::binary);
				if (ifs) {
					avl::AVL<std::string, uint64_t> newAVL;

					if (!newAVL.load(ifs)) {
						std::cout << "ERROR: Opened file isn't serialized dict" << std::endl;
						continue;
					} else {
						avlTree.swap(newAVL);
						std::cout << "OK" << std::endl;
					}
				} else {
					std::cout << "ERROR: Couldn't open file" << std::endl;
				}	
			}
		} else if (isalpha(command1[0])) {
			std::transform(command1.begin(), command1.end(), command1.begin(),
    					   [](unsigned char c){ return std::tolower(c); });

			std::pair<bool, uint64_t> findResult = avlTree.find(command1);

			if (findResult.first) {
				std::cout << "OK: " << findResult.second << std::endl;
			} else {
				std::cout << "NoSuchWord" << std::endl;
			}
		}
	}

	return 0;
}