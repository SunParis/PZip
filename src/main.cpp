
# include "Huffman.hpp"

void write_sth() {
    std::ofstream out("output.dat");
    if (!out) {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }
    for (std::size_t i = 0; i < 4000000000; i++) {
        char a = 1;
        out.write(&a, sizeof(char));
    }
    out.close();
}


int main(int argc, char *argv[]) {

    // if (argc < 2) {
    //     std::cerr << "Usage: " << argv[0] << " <file_name>" << std::endl;
    //     return 1;
    // }
    try {
        // write_sth();
        // File file("output.dat", File::Mode::Read);
        // std::cout << "File opened successfully: " << file.get_file_name() << std::endl;
        // std::cout << "File size: " << file.file_size() << " bytes" << std::endl;
        Huffman huffman("output.dat", 4);
        huffman.count_frequencies();
        huffman.print();
        
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}


