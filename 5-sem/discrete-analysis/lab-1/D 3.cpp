#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iterator>

const int INF = 1e9;

struct SuffixTreeNode {
    int firstPos_;
    int edgeLength_;
    SuffixTreeNode* parent_;
    std::map<int, SuffixTreeNode*> childs_;
    SuffixTreeNode* suffixLink_;

    SuffixTreeNode(const int fpos, const int tmpEdgeLength, SuffixTreeNode* tmpParent) {
        firstPos_ = fpos;
        edgeLength_ = tmpEdgeLength;
        parent_ = tmpParent;
    }
};

class SuffixTree {
  public:
    SuffixTreeNode* root_;

    SuffixTree() {
        n_ = 0;
        pos_ = 0;
        root_ = new SuffixTreeNode(0, INF, nullptr);
        root_->parent_ = root_;
    }

    void ExtendSuffTree(const int sym) {
        symPos_.emplace_back(sym);

        ++n_;
        ++pos_;

        SuffixTreeNode* node = root_;
        SuffixTreeNode* last = root_;

        while (pos_ > 0) {
            while ((node->childs_.find(symPos_[n_ - pos_]) != node->childs_.end()) &&
                   (pos_ > node->childs_[symPos_[n_ - pos_]]->edgeLength_)) {
                   node = node->childs_[symPos_[n_ - pos_]];
                   pos_ -= node->edgeLength_;
            }

            int edge = symPos_[n_ - pos_];

            std::map<int, SuffixTreeNode*>::iterator child = node->childs_.find(edge);
            int t = (child == node->childs_.end()) ? -1 : symPos_[child->second->firstPos_ + pos_ - 1];

            if (child == node->childs_.end()) {
                node->childs_[edge] = new SuffixTreeNode(n_ - pos_, INF, node);
                last->suffixLink_ = node;
                last = root_;
            } else if (t == sym) {
                last->suffixLink_ = node;

                return;
            } else {
                SuffixTreeNode* splitNode = new SuffixTreeNode(child->second->firstPos_, pos_ - 1, child->second->parent_);

                splitNode->childs_[sym] = new SuffixTreeNode(n_ - 1, INF, splitNode);
                splitNode->childs_[t] = child->second;

                child->second->firstPos_ += pos_ - 1;
                child->second->edgeLength_ -= pos_ - 1;
                child->second->parent_ = splitNode;
                child->second = splitNode;
                
                last->suffixLink_ = splitNode;
                last = splitNode;
            }

            if (node == root_) {
                --pos_;
            } else {
                node = node->suffixLink_;
            }
        }
    }

    void MatchStatistics(std::vector<int>& ms, const std::string& text) {
        root_->suffixLink_ = root_;
        SuffixTreeNode* node = setFirst(ms[0], text);

        for (int i = 1; i < ms.size(); i++) {
            if (ms[i - 1] == n_) {
                std::cout << i << std::endl;
            }

            int nodeegin = node->firstPos_;
            int whatsLeft = n_ - node->firstPos_;

            node = (node->edgeLength_ > n_) ? node->parent_->suffixLink_ : node->suffixLink_;

            while (whatsLeft > node->edgeLength_) {
                for (const auto& child: node->childs_) {
                    if (child.second->firstPos_ == nodeegin) {
                        node = child.second;
                        break;
                    }
                }

                whatsLeft -= node->edgeLength_;
                nodeegin += n_ - node->firstPos_;
            }

            for (int j = i, l = nodeegin; j < text.size() && l < n_; ++j, ++l) {
                if (text[j] != symPos_[l]) {
                    break;
                }

                ++ms[i];

                if (l == node->edgeLength_) {
                    std::map<int, SuffixTreeNode*>::iterator child = node->childs_.find(text[j + 1]);

                    if(child != node->childs_.end()) {
                        node = child->second;
                        l = node->firstPos_;
                    } else {
                        l = n_ + 1;
                    }
                }
            }
        }
    }

  private:
    int n_;
    int pos_;
    std::vector<int> symPos_;

    SuffixTreeNode* setFirst(int& ms, const std::string& pattern) {
        int smth = pattern.find(pattern[0]);
        SuffixTreeNode* tmp = root_;

        if (smth != std::string::npos) {
            std::map<int, SuffixTreeNode*>::iterator child = tmp->childs_.find(pattern[0]);
            while (smth < pattern.size()) {

                if (child != tmp->childs_.end()) {

                    for (int l = 0, i = child->second->firstPos_;
                        l < child->second->edgeLength_ && i < n_ && smth < pattern.size();
                        i++, smth++, l++) 
                    {

                        if (pattern[smth] != symPos_[i]) {
                            return tmp;
                        }
                        ++ms;
                    }

                    tmp = child->second;
                    child = child->second->childs_.find(pattern[smth]);
                } else {
                    return tmp;
                }
            }
        }

        return tmp;
    }
};

int main() {
    std::string pattern;
    std::string text;

    SuffixTree suffix_tree;

    std::cin >> pattern;
  
    for (const auto& i: pattern) {
        suffix_tree.ExtendSuffTree(i);
    }

    while (std::cin >> text) {
        std::vector<int> ms(text.size(), 0);
        suffix_tree.MatchStatistics(ms, text);
    }

    return 0;
}