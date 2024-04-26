#include "StringTests.h"

// Modify this array to change the number of words to load in each container from string list files. Default files has 466550 words max. 
static WordsCount WordsPerTest {
    100, 500, 1000, 2500, 5000, 10000, 20000, 30000, 50000, 75000, 100000,
    150000, 200000, 250000, 300000, 350000, 400000, 450000, 466550
};

int main(int argc, char* argv[])
{
    std::cout << "Initiating Tests...\n";
    
    TestEntryPoint NewTest(WordsPerTest);
    NewTest.StartTests(); 
    NewTest.SaveResults(false);

    std::cout << "Tests Completed. Results saved in root folder" << std::endl;
    system("pause"); 
    
    return 0;
}     

