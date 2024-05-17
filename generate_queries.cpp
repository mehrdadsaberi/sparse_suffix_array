#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

void read_dna_sequence(const std::string& filename, std::string& dna_sequence) {
    std::ifstream infile(filename);
    std::string line;

    // Skip the header line
    if (std::getline(infile, line)) {
        while (std::getline(infile, line)) {
            if (line[0] == '>') {
                continue; // skip header lines in the middle of the file
            }
            std::string line_filtered = "";
            for(auto c:line)
                if(c == 'A' || c == 'C' || c=='G' || c == 'T')
                    line_filtered += c;

            if(dna_sequence.size() > 1000 * 1000)
                break;
            dna_sequence += line_filtered;
        }
    }
    infile.close();
}

void sample_substrings(const std::string& dna_sequence, std::vector<std::pair<int, std::string>>& samples, int num_samples, int substring_length) {
    int max_start_index = dna_sequence.size() - substring_length;
    for (int i = 0; i < num_samples; ++i) {
        int start_index = (long long) rand() * rand() * rand() % max_start_index;
        std::string substring = dna_sequence.substr(start_index, substring_length);
        samples.emplace_back(start_index, substring);
    }
}

void write_samples_to_file(const std::string& filename, const std::vector<std::pair<int, std::string>>& samples) {
    std::ofstream outfile(filename);
    for (const auto& sample : samples) {
        outfile << sample.first << " " << sample.second << "\n";
    }
    outfile.close();
}

int main() {
    std::string dna_sequence;
    std::vector<std::pair<int, std::string>> samples;
    int num_samples = 1000;
    int substring_length = 100;
    
    // Seed the random number generator
    srand(7);

    // Read the DNA sequence from the file
    read_dna_sequence("Homo_sapiens.GRCh38.dna.alt.fa", dna_sequence);

    // std::cout<< dna_sequence.size() <<std::endl;
    // std::cout<< dna_sequence.substr(55518926, 100) << std::endl;

    // Sample substrings from the DNA sequence
    sample_substrings(dna_sequence, samples, num_samples, substring_length);

    // Write the samples to an output file
    write_samples_to_file("sampled_substrings.txt", samples);

    std::cout << "Sampling complete. Results are stored in sampled_substrings.txt" << std::endl;

    return 0;
}
