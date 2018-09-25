#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << "numberOfFiles outputDirectory" << std::endl;
        return EXIT_FAILURE;
    }
    
    int number_of_files = atoi(argv[1]);
    std::string output_directory(argv[2]);

    std::cout << "Selected program options: numberOfFiles = " << number_of_files;
    std::cout << ", outputDirectory = " << output_directory << std::endl;

    time_t time_program_start, time_program_end;
    time_program_start = time(NULL);


    for (int iteration = 0; iteration < number_of_files; ++iteration)
    {
        //


        //std::cout << "Getting random seed from /dev/urandom..." << std::endl;
        
        long random_seed;
        size_t seed_size = sizeof(random_seed);

        std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary);

        if (nullptr != urandom)
        {
            //
            urandom.read(reinterpret_cast<char*>(&random_seed), seed_size);
            if (nullptr != urandom)
            {
                //std::cout << "Random seed = " << random_seed << std::endl;
            }
            else
            {
                //
                std::cerr << "Error reading /dev/urandom" << std::endl;
            }
            urandom.close();
        }
        else
        {
            //
            std::cerr << "Error opening /dev/urandom" << std::endl;
        }

        std::mt19937 rng;

        rng.seed(random_seed);

        std::uniform_int_distribution<long> long_dist(0, 999999);

        long random_number = long_dist(rng);

        //std::cout << "Generated random number is " << random_number << std::endl;
        
        size_t buf_size = 8;
        char filename[buf_size];
        snprintf(filename, buf_size, "x%.6i", random_number);

        //std::cout << "Filename: " << filename << std::endl;

        std::string filepath;
        filepath = output_directory;
        if ('/' != filepath.back()) { filepath += '/'; }
        filepath += filename;

        int fd = open(filepath.c_str(), O_CREAT | O_WRONLY);
        
        if (-1 == fd)
        {
            std::cerr << "Error opening file " << filename << std::endl;
            return EXIT_FAILURE;
        }

        ssize_t bytes = write(fd, "A", 1);
        if (-1 == bytes)
        {
            std::cerr << "Error writing to file " << filename << std::endl;
            return EXIT_FAILURE;
        }


        if (-1 == close(fd))
        {
            std::cerr << "Error closing file " << filename << std::endl;
            return EXIT_FAILURE;
        } 

    } // main for loop (iteration)



    time_program_end = time(NULL);

    time_t time_executing = time_program_end - time_program_start;

    std::cout << "Program was executing for " << time_executing << " seconds." << std::endl;
    
    return EXIT_SUCCESS;
}
