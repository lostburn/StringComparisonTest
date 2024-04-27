#include "StringTests.h"
#include <algorithm>
#include <iostream>

void StringInsertion::InsertIntoVector(const VectorStrings& A, VectorStrings& B)
{
    for(const auto& String : A)
        B.emplace_back(String); 
}

void StringInsertion::InsertIntoSet(const VectorStrings& A, SetStrings& B)
{
    for(const auto& String : A)
        B.emplace(String);
}

void StringInsertion::InsertIntoMap(const VectorStrings& A, MapStrings& B)
{
    for(const auto& String : A)
        B[String] = String;
}

int BinarySearch::StringCaseSensitive(const VectorStrings& A, const VectorStrings& B)
{
    int Equal = 0;        
    for(const auto& String : A) {
        if(std::ranges::binary_search(B, String))
            Equal++;
    }     
    return Equal; 
}

int BinarySearch::StringIgnoreCase(const VectorStrings& A, const VectorStrings& B)
{
    int Equal = 0;
    for(const auto& String : A) {
        auto LowerCaseString = GetLowercaseString(String);
        if(std::ranges::binary_search(B, LowerCaseString))
            Equal++;
    }
    return Equal; 
}

int BinarySearch::HashCaseSensitive(const VectorStrings& A, const VectorHashes& B)
{
    int Equal = 0;
    for(const auto& String : A) {
        auto HashedString = GetStringHash(String);
        if(std::ranges::binary_search(B, HashedString))
            Equal++;
    }
    return Equal; 
}

int BinarySearch::HashIgnoreCase(const VectorStrings& A, const VectorHashes& B)
{
    int Equal = 0;
    for(const auto& String : A) {
        auto HashedString = GetStringHash(GetLowercaseString(String), true);
        if(std::ranges::binary_search(B, HashedString))
            Equal++;
    }
    return Equal; 
}

int LinearSearch::MyMethod(const VectorStrings& A, const VectorStrings& B)
{
    int EqualStrings = 0;

    for(const auto& StringOne : A)
    {
        if(std::ranges::find_if(B, [&StringOne](std::string_view StringTwo)
        {
            if(StringOne.size() != StringTwo.size())
                return false;

            for(auto i = 0; i < StringOne.size(); i++)
            {
                if(StringOne[i] == StringTwo[i]) continue;

                if(std::tolower(StringOne[i]) != std::tolower(StringTwo[i]))
                    return false;
            }
                    
            return true;
            
        }) != B.end())
            EqualStrings++;
    }

    return EqualStrings; 
}

int LinearSearch::EqualOperator(const VectorStrings& A, const VectorStrings& B)
{
    int EqualStrings = 0;
    
    for(const auto& StringOne : A) {
        if(std::ranges::find(B, StringOne) != B.end())
            EqualStrings++;
    }
    
    return EqualStrings; 
}

int LinearSearch::HashCaseSensitive(const VectorStrings& A, const VectorHashes& B)
{
    int Equal = 0;
    for(const auto& String : A) {
        auto HashedString = GetStringHash(String, false);
        if(std::ranges::find(B, HashedString) != B.end())
            Equal++;
    }         
    return Equal; 
}

int LinearSearch::HashIgnoreCase(const VectorStrings& A, const VectorHashes& B)
{
    int Equal = 0;
    for(const auto& String : A) {
        auto HashedString = GetStringHash(GetLowercaseString(String), true);
        if(std::ranges::find(B, HashedString) != B.end())
            Equal++;
    }         
    return Equal; 
}

int SearchByKey::StringSetCaseSensitive(const VectorStrings& A, const SetStrings& B)
{
    int Equal = 0;
    for(const auto& String : A) {
        if(B.contains(String))
            Equal++;
    }

    return Equal; 
}

int SearchByKey::StringSetIgnoreCase(const VectorStrings& A, const SetStrings& B)
{
    int Equal = 0;
    for(const auto& String : A) {
        if(B.contains(GetLowercaseString(String)))
            Equal++;
    }

    return Equal; 
}

int SearchByKey::HashMapCaseSensitive(const VectorStrings& A, const MapHashes& B)
{
    int Equal = 0;
    for(const auto& String : A) {
        auto HashedString = GetStringHash(String);
        if(B.contains(HashedString))
            Equal++;
    }

    return Equal;
}

int SearchByKey::HashMapIgnoreCase(const VectorStrings& A, const MapHashes& B)
{
    int Equal = 0;
    for(const auto& String : A) {
        auto HashedString = GetStringHash(String, true);
        if(B.contains(HashedString))
            Equal++;
    }

    return Equal;
}

int SearchByKey::StringMapCaseSensitive(const VectorStrings& A, const MapStrings& B)
{
    int Equal = 0;
    for(const auto& String : A) {
        if(B.contains(String))
            Equal++;
    }

    return Equal; 
}

int SearchByKey::StringMapIgnoreCase(const VectorStrings& A, const MapStrings& B)
{
    int Equal = 0;
    for(const auto& String : A) {
        if(B.contains(GetLowercaseString(String)))
            Equal++;
    }

    return Equal; 
}

void BinarySearchTest::InitTest()
{
    for(const int Words : mWordsPerTest)
    {        
        if(bUseHash) {
            TStringData<VectorHashes> StringData; 
            StringData.SetupHashVector(bIgnoreCase, true, Words);
            mResult += RunAndMeasure(StringData, bIgnoreCase ? BinarySearch::HashIgnoreCase : BinarySearch::HashCaseSensitive, ITERATIONS);
        }

        else
        {
            TStringData<VectorStrings> StringData;
            StringData.SetupStringVector(bIgnoreCase, true, Words); 
            mResult += RunAndMeasure(StringData, bIgnoreCase ? BinarySearch::StringIgnoreCase : BinarySearch::StringCaseSensitive, ITERATIONS);
        }            
    }
}

void UnorderedSetTest::InitTest()
{
    for(const int Words : mWordsPerTest)
    {
        TStringData<SetStrings> StringData;
        StringData.SetupSet(bIgnoreCase, Words);
        mResult += RunAndMeasure(StringData, bIgnoreCase ? SearchByKey::StringSetIgnoreCase : SearchByKey::StringSetCaseSensitive, ITERATIONS);
    }
}

void UnorderedMapTest::InitTest()
{
    for(const int Words : mWordsPerTest)
    {
        if(bUseHash)
        {
            TStringData<MapHashes> StringData;
            StringData.SetupHashMap(bIgnoreCase, Words);
            mResult += RunAndMeasure(StringData, bIgnoreCase ? SearchByKey::HashMapIgnoreCase : SearchByKey::HashMapCaseSensitive, ITERATIONS);
        }

        else
        {
            TStringData<MapStrings> StringData;
            StringData.SetupStringMap(bIgnoreCase, Words);
            mResult += RunAndMeasure(StringData, bIgnoreCase ? SearchByKey::StringMapIgnoreCase : SearchByKey::StringMapCaseSensitive, ITERATIONS);
        }
    }
}

void InsertionTest::InitTest()
{
    for(const int Words : mWordsPerTest)
    {
        switch(mInsertionType)
        {
            case(EInsertionType::Vector):
            {
                TStringData<VectorStrings> StringData;
                StringData.SetupListOne(Words);                    
                mResult += RunAndMeasure(StringData, StringInsertion::InsertIntoVector, ITERATIONS);
                break; 
            }
            case(EInsertionType::Set):
            {
                TStringData<SetStrings> StringData;
                StringData.SetupListOne(Words);
                mResult += RunAndMeasure(StringData, StringInsertion::InsertIntoSet, ITERATIONS);
                break;
            }
            case(EInsertionType::Map):
            {
                TStringData<MapStrings> StringData;
                StringData.SetupListOne(Words);
                mResult += RunAndMeasure(StringData, StringInsertion::InsertIntoMap, ITERATIONS);
                break; 
            }
            
            default: assert(false && "Insertion Type not Handled"); 
        }
    }
}

TestEntryPoint::TestEntryPoint(WordsCount InWordsCount) : mWordsPerTest(std::move(InWordsCount))
{
    std::cout << "Choose iteration count between (1, 10)" << std::endl;
    std::cin >> ITERATIONS;
    
    if(ITERATIONS < 1)
        std::cout << MIN_ITERATIONS_ERROR_TEXT << std::endl;
    
    else if(ITERATIONS > 10)
        std::cout << MAX_ITERATIONS_ERROR_TEXT << std::endl; 
        
    std::cout << "Initiating Tests with " << ITERATIONS << " iterations per word count" << std::endl;
} 

void TestEntryPoint::StartTests()
{
    std::cout << "Creating Binary Search Tests..." << std::endl;
    mTests.push_back(std::make_unique<BinarySearchTest>(mWordsPerTest, FTestSetup("BS_S_CS"), false));
    mTests.push_back(std::make_unique<BinarySearchTest>(mWordsPerTest, FTestSetup("BS_S_CI", true), false));
    mTests.push_back(std::make_unique<BinarySearchTest>(mWordsPerTest, FTestSetup("BS_H_CS"), true));
    mTests.push_back(std::make_unique<BinarySearchTest>(mWordsPerTest, FTestSetup("BS_H_CI", true), true));

    std::cout << "Creating Unordered Set Tests..." << std::endl;
    mTests.push_back(std::make_unique<UnorderedSetTest>(mWordsPerTest, FTestSetup("US_S_CS")));
    mTests.push_back(std::make_unique<UnorderedSetTest>(mWordsPerTest, FTestSetup("US_S_CI", false))); 

    std::cout << "Creating Unordered Map Tests..." << std::endl;
    mTests.push_back(std::make_unique<UnorderedMapTest>(mWordsPerTest, FTestSetup("UM_S_CS"), false));
    mTests.push_back(std::make_unique<UnorderedMapTest>(mWordsPerTest, FTestSetup("UM_S_CI", false), false));
    mTests.push_back(std::make_unique<UnorderedMapTest>(mWordsPerTest, FTestSetup("UM_H_CS"), true));
    mTests.push_back(std::make_unique<UnorderedMapTest>(mWordsPerTest, FTestSetup("UM_H_CI", true), true));

    std::cout << "Creating Insertion Tests..."  << std::endl;
    mTests.push_back(std::make_unique<InsertionTest>(mWordsPerTest, FTestSetup("V_Ins"), EInsertionType::Vector));
    mTests.push_back(std::make_unique<InsertionTest>(mWordsPerTest, FTestSetup("S_Ins"), EInsertionType::Set));
    mTests.push_back(std::make_unique<InsertionTest>(mWordsPerTest, FTestSetup("M_Ins"), EInsertionType::Map));

    std::cout << "Running Tests..." << std::endl;

    for(const auto& Test : mTests)
        Test->Start();
}

void TestEntryPoint::SaveResults(const bool bPrintToConsole)
{
    std::string Results = { "Compare Type, " };
    
    for(const auto Word : mWordsPerTest)
        Results += std::to_string(Word) + ", ";

    Results += "\n";

    for(const auto& Test : mTests){
        Results += Test->GetResult() + "\n"; 
    }

    if(bPrintToConsole)
        std::cout << Results << "\n";

    std::ofstream OutputFile(RESULTS_FILE_NAME);
    OutputFile << Results;
    OutputFile.close();
    std::cout << "Tests Completed. Results saved in root folder" << std::endl;
}
