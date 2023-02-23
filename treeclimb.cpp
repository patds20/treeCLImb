#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

class TreeMindMap {
public:
    TreeMindMap(string name, string filename) {
        root_ = new TreeNode(name, 0);
        filename_ = filename;
    }

    ~TreeMindMap() {
        delete root_;
    }

    void save() {
        ofstream file(filename_);
        if (file.is_open()) {
            root_->save(file);
            file.close();
        } else {
            cerr << "Error: Unable to open file " << filename_ << endl;
        }
    }

    void appendNode(string parentName, string name) {
        TreeNode* parent = findNodeByName(parentName);
        if (parent == nullptr) {
            cerr << "Error: Unable to find parent node " << parentName << endl;
            return;
        }
        if (parent->children.size() >= 5) {
            cerr << "Error: Maximum children limit reached for node " << parentName << endl;
            return;
        }
        if (findNodeByName(name) != nullptr) {
            cerr << "Error: Node " << name << " already exists" << endl;
            return;
        }
        TreeNode* node = new TreeNode(name, parent->level_ + 1);
        parent->addChild(node);
        save();
    }
    void print(){
        root_->write();
    }

/*    void print() {
        cout << endl << setw(root_->level_*4) << "";
        cout << "[" << root_->name_ << "]" << endl;
        printChildren(root_);
        cout << endl;
    }*/


    static TreeMindMap* load(string filename) {
        TreeMindMap* tree = new TreeMindMap("", filename);
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                size_t level = line.find_first_not_of(" ");
                string name = line.substr(level, line.length() - level);
                tree->addNode(name, level / 4);
            }
            file.close();
        } else {
            cerr << "Error: Unable to open file " << filename << endl;
        }
        return tree;
    }

private:
    class TreeNode {
    public:
        TreeNode(string name, int level) {
            name_ = name;
            level_ = level;
        }

        ~TreeNode() {
            for (auto child : children) {
                delete child;
            }
        }

        void addChild(TreeNode* child) {
            children.push_back(child);
        }

        void save(ofstream& file) {
            file << setw(level_ * 4) << "";
            file << name_ << endl;
            for (auto child : children) {
                child->save(file);
            }
        }

        string name_;
        int level_;
        vector<TreeNode*> children;

        void write(int indent = 0) {
            int box_width = name_.size() + 4;
            for (int i = 0; i < indent; i++) {
                cout << " ";
            }
            cout << "┏";
            for (int i = 0; i < box_width; i++) {
                cout << "━";
            }
            cout << "┓" << endl;
            for (int i = 0; i < indent; i++) {
                cout << " ";
            }
            cout << "┃\033[1m " << name_ << " \033[0m┃" << endl;
            for (int i = 0; i < indent; i++) {
                cout << " ";
            }
            cout << "┣";
            for (int i = 0; i < box_width; i++) {
                cout << "━";
            }
            cout << "┫" << endl;

            for (int i = 0; i < children.size(); i++) {
                children[i]->write(indent + box_width + 2);
            }
            for (int i = 0; i < indent; i++) {
                cout << " ";
            }
            cout << "┗";
            for (int i = 0; i < box_width; i++) {
                cout << "━";
            }
            cout << "┛" << endl;
        }


    };

    void addNode(string name, int level) {
        TreeNode* node = new TreeNode(name, level);
        if (level == 0) {
            root_ = node;
        } else {
            TreeNode* parent = findNodeByLevel(level - 1);
            parent->addChild(node);
        }
    }

    TreeNode* findNodeByName(string name) {
        return findNodeByNameHelper(root_, name);
    }

    TreeNode* findNodeByNameHelper(TreeNode* node, string name) {
        if (node->name_ == name) {
            return node;
        }
        for (auto child : node->children) {
            TreeNode* result = findNodeByNameHelper(child, name);
            if (result != nullptr) {
                return result;
            }
        }
        return nullptr;
    }

    TreeNode* findNodeByLevel(int level) {
        return findNodeByLevelHelper(root_, level);
    }

    TreeNode* findNodeByLevelHelper(TreeNode* node, int level) {
        if (node->level_ == level) {
            return node;
        }
        for (auto child : node->children) {
            TreeNode* result = findNodeByLevelHelper(child, level);
            if (result != nullptr) {
                return result;
            }
        }
        return nullptr;
    }

    void printChildren(TreeNode* node) {
        if (!node->children.empty()) {
            for (auto child : node->children) {
                cout << setw(child->level_*4 + 2) << setfill('-') << "";
                cout << "[" << child->name_ << "]" << endl;
                printChildren(child);
            }
        }
    }


    TreeNode* root_;
    string filename_;
};

void printHelp() {
    cout << "TreeCLImb Tool (c) 2023 Patrick De Smet" << endl;
    cout << "Usage:" << endl;
    cout << "   init <name> <filename>  Initialize a new tree with the root node <name> and saves it in the file <filename>." << endl;
    cout << "   node <filename> <parent> <label>  Append the node label to parent and saves it again." << endl;
    cout << "   print <filename>  Prints the tree Mindmap in ASCII style." << endl;
    cout << "   help  Print this help page." << endl;
}

int main(int argc, char* argv[]) {
    if (argc == 1 || (argc == 2 && (string(argv[1]) == "--help") || string(argv[1]) == "help")) {
        printHelp();
        return 0;
    }

    if (argc == 4 && string(argv[1]) == "init") {
        TreeMindMap* tree = new TreeMindMap(argv[2], argv[3]);
        tree->save();
        delete tree;
        cout << "Tree MindMap initialized successfully!" << endl;
        return 0;
    }

    if (argc == 5 && string(argv[1]) == "node") {
        TreeMindMap* tree = TreeMindMap::load(argv[2]);
        tree->appendNode(argv[3], argv[4]);
        delete tree;
        cout << "Node added successfully!" << endl;
        return 0;
    }

    if (argc == 3 && string(argv[1]) == "print") {
        TreeMindMap* tree = TreeMindMap::load(argv[2]);
        tree->print();
        delete tree;
        return 0;
    }

    cout << "Error: Invalid command or arguments. Use '--help' for usage information." << endl;
    return 1;
}
