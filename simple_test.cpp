#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    std::map<std::string, std::vector<int>> db;
    
    int n;
    std::cin >> n;
    
    for (int i = 0; i < n; i++) {
        std::string command;
        std::cin >> command;
        
        if (command == "insert") {
            std::string key;
            int value;
            std::cin >> key >> value;
            
            // Check for duplicate
            auto& values = db[key];
            if (std::find(values.begin(), values.end(), value) == values.end()) {
                values.push_back(value);
                std::sort(values.begin(), values.end());
            }
        } else if (command == "delete") {
            std::string key;
            int value;
            std::cin >> key >> value;
            
            auto it = db.find(key);
            if (it != db.end()) {
                auto& values = it->second;
                auto val_it = std::find(values.begin(), values.end(), value);
                if (val_it != values.end()) {
                    values.erase(val_it);
                    if (values.empty()) {
                        db.erase(it);
                    }
                }
            }
        } else if (command == "find") {
            std::string key;
            std::cin >> key;
            
            auto it = db.find(key);
            if (it == db.end() || it->second.empty()) {
                std::cout << "null";
            } else {
                for (size_t j = 0; j < it->second.size(); j++) {
                    if (j > 0) std::cout << " ";
                    std::cout << it->second[j];
                }
            }
            std::cout << "\n";
        }
    }
    
    return 0;
}