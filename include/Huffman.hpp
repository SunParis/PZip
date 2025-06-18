# pragma once

# ifndef _HUFFMAN_HPP_
# define _HUFFMAN_HPP_ 1

# include <thread>
# include <vector>
# include <unordered_map>
# include <queue>
# include <cstdint>
# include <functional>

# include "File.hpp"

class Huffman {

private:
    
    std::string file_name_;

    std::size_t thread_num_;

    const std::size_t chunk_size_ = sizeof(std::uint64_t) * 1024;

    std::uintmax_t file_size_;

    std::unordered_map<std::uint64_t, std::pair<std::size_t, std::vector<char>>> frequencies;

    std::vector<char> remain;

    void count_sub_thread(File& file, std::size_t start, std::unordered_map<std::uint64_t, std::size_t> ret) {
        std::vector<std::uint64_t> buffer(this->chunk_size_);
        for (std::uintmax_t i = 0; i < this->file_size_;) {
            Result<std::streamsize> result = file.read(buffer.data(), buffer.size(), start + i);
            if (!result.is_success()) {
                throw std::runtime_error("Error reading file: " + result.get_error());
            }
            std::streamsize bytes_read = result.get_value();
            for (std::size_t j = 0; j < buffer.size(); j += 1) {
                ret[buffer[j]] += 1;
            }
            i += (this->thread_num_ * this->chunk_size_);
        }
    }

    struct HuffmanNode {
        std::size_t freq;
        std::uint64_t key;
        HuffmanNode* left;
        HuffmanNode* right;
        
        HuffmanNode(std::size_t f, std::uint64_t k)
        :   freq(f), key(k), left(nullptr), right(nullptr)
        {}
        
        HuffmanNode(std::size_t f, HuffmanNode* l, HuffmanNode* r)
        :   freq(f), key(0), left(l), right(r)
        {}
    };

    struct CompareNode {
        bool operator()(HuffmanNode* a, HuffmanNode* b) {
            return a->freq > b->freq;
        }
    };

    HuffmanNode* buildHuffmanTree(const std::unordered_map<std::uint64_t, std::pair<std::size_t, std::vector<char>>>& frequencies_) {
        std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareNode> pq;
        for (const auto& entry : frequencies_) {
            pq.push(new HuffmanNode(entry.second.first, entry.first));
        }
        while (pq.size() > 1) {
            HuffmanNode* left = pq.top(); pq.pop();
            HuffmanNode* right = pq.top(); pq.pop();
            pq.push(new HuffmanNode(left->freq + right->freq, left, right));
        }
        return pq.empty() ? nullptr : pq.top();
    }

    void generateCodesChar(HuffmanNode* node, std::vector<char>& code, std::unordered_map<std::uint64_t, std::vector<char>>& codes) {
        if (!node) return;
        if (!node->left && !node->right) {
            codes[node->key] = code;
            return;
        }
        code.push_back('0');
        generateCodesChar(node->left, code, codes);
        code.pop_back();

        code.push_back('1');
        generateCodesChar(node->right, code, codes);
        code.pop_back();
    }

    void assignCodesChar(std::unordered_map<std::uint64_t, std::pair<std::size_t, std::vector<char>>>& frequencies) {
        HuffmanNode* root = buildHuffmanTree(frequencies);
        if (!root) return;
        std::unordered_map<std::uint64_t, std::vector<char>> codes;
        std::vector<char> code;
        generateCodesChar(root, code, codes);

        for (auto& entry : frequencies) {
            auto it = codes.find(entry.first);
            if (it != codes.end()) {
                entry.second.second = it->second;
            }
        }

        std::function<void(HuffmanNode*)> deleteTree = [&](HuffmanNode* node) {
            if (!node) return;
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        };

        deleteTree(root);
    }



public:

    Huffman(const std::string_view& file_name, std::size_t thread_num = 4)
    :   file_name_(file_name), thread_num_(thread_num) 
    {
        if (thread_num_ < 1) {
            throw std::invalid_argument("Number of threads must be at least 1.");
        }
    }    
    
    void count_frequencies() {
        File file(file_name_, File::Mode::Read);
        this->file_size_ = file.file_size();

        if (this->file_size_ < this->chunk_size_) {
            return;
        }
        
        std::vector<std::pair<std::thread *, std::unordered_map<std::uint64_t, std::size_t>>> threads;
        threads.reserve(thread_num_);

        for (std::size_t i = 0; i < thread_num_; ++i) {
            std::size_t start = i * chunk_size_;
            threads[i].first = new std::thread(&Huffman::count_sub_thread, this,
                std::ref(file), start, std::ref(threads[i].second));
        }

        if (this->file_size_ % this->chunk_size_ > 0) {
            this->remain.resize(this->file_size_ % this->chunk_size_);
            Result<std::streamsize> result = file.read(remain.data(), remain.size(), this->file_size_ - remain.size());
            if (!result.is_success()) {
                throw std::runtime_error("Error reading file: " + result.get_error());
            }
        }


        for (auto& thread : threads) {
            thread.first->join();           
            delete thread.first;
            for (const auto& pair : thread.second) {
                this->frequencies[pair.first].first += pair.second;
            }
        }

        this->assignCodesChar(this->frequencies);
    }

    const std::vector<char>& get_code(std::uint64_t key) const {
        auto it = frequencies.find(key);
        if (it != frequencies.end()) {
            return it->second.second;
        }
        throw std::runtime_error("Key not found in frequencies.");
    }

    void print() {
        if (frequencies.empty()) {
            std::cout << "No frequencies counted." << std::endl;
            return;
        }
        for (const auto& entry : frequencies) {
            std::cout << "Key: " << entry.first << ", Frequency: " << entry.second.first << ", Code: ";
            for (char bit : entry.second.second) {
                std::cout << bit;
            }
            std::cout << std::endl;
        }
    }

};

# endif // Huffman.hpp
