#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <filesystem>
#include <map>
#include <unordered_map>

const std::string DB_FILE = "bptree.db";

// Optimized file-based key-value store using unordered_map for better performance
// Only writes to file at program end to minimize I/O operations
class OptimizedKVStore {
private:
    std::unordered_map<std::string, std::vector<int>> data;
    std::string filename;
    bool dirty;
    
    void save_to_file() {
        if (!dirty) return;
        
        std::ofstream file(filename);
        if (!file.is_open()) return;
        
        for (const auto& pair : data) {
            for (int value : pair.second) {
                file << pair.first << " " << value << "\n";
            }
        }
        file.close();
        dirty = false;
    }
    
    void load_from_file() {
        std::ifstream file(filename);
        if (!file.is_open()) return;
        
        data.clear();
        std::string key;
        int value;
        
        while (file >> key >> value) {
            data[key].push_back(value);
        }
        
        // Sort values for each key
        for (auto& pair : data) {
            std::sort(pair.second.begin(), pair.second.end());
        }
        
        file.close();
        dirty = false;
    }
    
public:
    OptimizedKVStore(const std::string& fname) : filename(fname), dirty(false) {
        load_from_file();
    }
    
    ~OptimizedKVStore() {
        save_to_file();
    }
    
    void insert(const std::string& key, int value) {
        auto& values = data[key];
        if (std::find(values.begin(), values.end(), value) == values.end()) {
            values.push_back(value);
            std::sort(values.begin(), values.end());
            dirty = true;
        }
    }
    
    void remove(const std::string& key, int value) {
        auto it = data.find(key);
        if (it != data.end()) {
            auto& values = it->second;
            auto val_it = std::find(values.begin(), values.end(), value);
            if (val_it != values.end()) {
                values.erase(val_it);
                if (values.empty()) {
                    data.erase(it);
                }
                dirty = true;
            }
        }
    }
    
    std::vector<int> find(const std::string& key) {
        auto it = data.find(key);
        if (it != data.end()) {
            return it->second;
        }
        return {};
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    OptimizedKVStore store(DB_FILE);
    
    int n;
    std::cin >> n;
    
    for (int i = 0; i < n; i++) {
        std::string command;
        std::cin >> command;
        
        if (command == "insert") {
            std::string key;
            int value;
            std::cin >> key >> value;
            store.insert(key, value);
        } else if (command == "delete") {
            std::string key;
            int value;
            std::cin >> key >> value;
            store.remove(key, value);
        } else if (command == "find") {
            std::string key;
            std::cin >> key;
            std::vector<int> results = store.find(key);
            
            if (results.empty()) {
                std::cout << "null";
            } else {
                for (size_t j = 0; j < results.size(); j++) {
                    if (j > 0) std::cout << " ";
                    std::cout << results[j];
                }
            }
            std::cout << "\n";
        }
    }
    
    return 0;
}