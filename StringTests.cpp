﻿#include "StringTests.h"

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

int DefaultSearch::MyMethod(const VectorStrings& A, const VectorStrings& B)
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

int DefaultSearch::EqualOperator(const VectorStrings& A, const VectorStrings& B)
{
    int EqualStrings = 0;
    
    for(const auto& StringOne : A) {
        if(std::ranges::find(B, StringOne) != B.end())
            EqualStrings++;
    }
    
    return EqualStrings; 
}

int ArrayHashSearch::HashCaseSensitive(const VectorStrings& A, const VectorHashes& B)
{
    int Equal = 0;
    for(const auto& String : A) {
        auto HashedString = GetStringHash(String, false);
        if(std::ranges::find(B, HashedString) != B.end())
            Equal++;
    }         
    return Equal; 
}

int ArrayHashSearch::HashLowerCase(const VectorStrings& A, const VectorHashes& B)
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

int SearchByKey::StringSetLowercase(const VectorStrings& A, const SetStrings& B)
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

int SearchByKey::HashMapLowercase(const VectorStrings& A, const MapHashes& B)
{
    int Equal = 0;
    for(const auto& String : A) {
        auto HashedString = GetStringHash(String, true);
        if(B.contains(HashedString))
            Equal++;
    }

    return Equal;
}

void BinarySearchTest::InitTest()
{
    for(const int Words : mWordsPerTest)
    {        
        if(bHashString) {
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
        mResult += RunAndMeasure(StringData, bIgnoreCase ? SearchByKey::StringSetLowercase : SearchByKey::StringSetCaseSensitive, ITERATIONS);
    }
}

void UnorderedMapTest::InitTest()
{
    for(const int Words : mWordsPerTest)
    {
        TStringData<MapHashes> StringData;
        StringData.SetupMap(bIgnoreCase, Words);
        mResult += RunAndMeasure(StringData, bIgnoreCase ? SearchByKey::HashMapLowercase : SearchByKey::HashMapCaseSensitive, ITERATIONS);
    }
}

void TestEntryPoint::StartTests()
{
    std::cout << "Creating Binary Search Tests...\n";
    mTests.push_back(std::make_unique<BinarySearchTest>(mWordsPerTest, FTestSetup("BS_S_CS"), false));
    mTests.push_back(std::make_unique<BinarySearchTest>(mWordsPerTest, FTestSetup("BS_S_CI", true), false));
    mTests.push_back(std::make_unique<BinarySearchTest>(mWordsPerTest, FTestSetup("BS_H_CS"), true));
    mTests.push_back(std::make_unique<BinarySearchTest>(mWordsPerTest, FTestSetup("BS_H_CI", true), true));

    std::cout << "Creating Unordered Set Tests...\n";
    mTests.push_back(std::make_unique<UnorderedSetTest>(mWordsPerTest, FTestSetup("US_S_CS")));
    mTests.push_back(std::make_unique<UnorderedSetTest>(mWordsPerTest, FTestSetup("US_S_CI", false))); 

    std::cout << "Creating Unordered Map Tests...\n";
    mTests.push_back(std::make_unique<UnorderedMapTest>(mWordsPerTest, FTestSetup("UM_S_CS")));
    mTests.push_back(std::make_unique<UnorderedMapTest>(mWordsPerTest, FTestSetup("UM_S_CI", false)));

    std::cout << "Starting Tests...\n";

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
}
