#pragma once

#include <algorithm>
#include <chrono>
#include <format>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <ranges>
#include <cassert>

static int ITERATIONS = 10;
static const char* RESULTS_FILE_NAME = "results.csv";
static const char* LIST_ONE_STRINGS = "list_one.txt"; // The file for the first set of strings.
static const char* LIST_TWO_STRINGS = "list_two.txt"; // The file for the second set of strings.

using HashSize = size_t; 
using WordsCount = std::vector<int>; 
using VectorStrings = std::vector<std::string>;
using VectorHashes = std::vector<HashSize>;
using SetStrings = std::unordered_set<std::string>;
using MapHashes = std::unordered_map<HashSize, std::string>;
using MapStrings = std::unordered_map<std::string, std::string>;

class TestBase; 
using TTests = std::vector<std::unique_ptr<TestBase>>;

template<typename ContainerType>
using CompareFn = int(*)(const VectorStrings&, const ContainerType&);

template<typename ContainerType>
using InsertFn = void(*)(const VectorStrings&, ContainerType&);


static std::string GetLowercaseString(std::string_view String)
{
    std::string LowercaseString;
    LowercaseString.reserve(String.size());
    std::ranges::transform(String, std::back_inserter(LowercaseString), [](char c) { return std::tolower(c); });
    return LowercaseString;
}

static HashSize GetStringHash(std::string_view Key, const bool bUseLowerCase = false)
{
    return static_cast<HashSize>(std::hash<std::string>{}(bUseLowerCase ? GetLowercaseString(Key) : Key.data()));
}

template<typename ContainerType>
static void LoadStringsFromFile(ContainerType& Container, const std::string& FileName, int WordLimit = 100)
{
    std::ifstream InputFile(FileName);
    assert(InputFile.is_open() && "Failed to open file. Check the file path, change SOLUTION_DIR in Templates.h");

    std::string Word;
    while(InputFile >> Word && WordLimit-- > 0)
        Container.insert(Container.end(), Word); 

    InputFile.close();    
}

template<typename T>
class TStringData
{

public:

    VectorStrings ListOne;
    T ListTwo;

    void SetupListOne(const int WordLimit = 1000)
    {
        LoadStringsFromFile(ListOne, LIST_ONE_STRINGS, WordLimit);
    }
    
    void SetupStringVector(const bool bLowerCase, const bool bSort, const int WordLimit = 1000)
    {
        static_assert(std::is_same_v<T, VectorStrings> && "ListTwo must be of type VectorStrings");
        InitFromFile(WordLimit);

        for(const auto& String : TempList)
            ListTwo.emplace_back(bLowerCase ? GetLowercaseString(String) : String);

        if(bSort)
            std::ranges::sort(ListTwo);
    }

    void SetupHashVector(const bool bLowerCase, const bool bSort, const int WordLimit = 1000)
    {
        static_assert(std::is_same_v<T, VectorHashes> && "ListTwo must be of type VectorHashes");
        InitFromFile(WordLimit);

        for(const auto& String : TempList)
            ListTwo.emplace_back(GetStringHash(String, bLowerCase));

        if(bSort)
            std::ranges::sort(ListTwo);
    }

    void SetupSet(const bool bLowerCase, int WordLimit = 1000)
    {
        static_assert(std::is_same_v<T, SetStrings> && "ListTwo must be of type SetStrings");
        InitFromFile(WordLimit);

        for(const auto& String : TempList)
            ListTwo.insert(bLowerCase ? GetLowercaseString(String) : String);
    }

    void SetupHashMap(const bool bLowerCase, int WordLimit = 1000)
    {
        static_assert(std::is_same_v<T, MapHashes> && "ListTwo must be of type MapHashes");
        InitFromFile(WordLimit);

        for(const auto& String : TempList)
            ListTwo[GetStringHash(String, bLowerCase)] = String;
    }

    void SetupStringMap(const bool bLowerCase, int WordLimit = 1000)
    {
        static_assert(std::is_same_v<T, MapStrings> && "ListTwo must be of type MapStrings");
        InitFromFile(WordLimit);

        for(const auto& String : TempList)
            ListTwo[bLowerCase ? GetLowercaseString(String) : String] = String;
    }
    
private:

    VectorStrings TempList; 

    void InitFromFile(const int WordLimit = 1000)
    {
        LoadStringsFromFile(ListOne, LIST_ONE_STRINGS, WordLimit);
        LoadStringsFromFile(TempList, LIST_TWO_STRINGS, WordLimit);
    }
};

template<typename T>
static std::string RunAndMeasure(TStringData<T> StringData, CompareFn<T> Function, int Iterations = 1)
{
    int EqualAmount = 0;

    const auto Start = std::chrono::steady_clock::now();
    while(Iterations-- > 0)
        EqualAmount += Function(StringData.ListOne, StringData.ListTwo);
    const auto End = std::chrono::steady_clock::now();        

    return std::format("{:.4f},", std::chrono::duration<double, std::milli>(End - Start).count()); 
}

template<typename T>
static std::string RunAndMeasure(TStringData<T> StringData, InsertFn<T> Function, int Iterations = 1)
{
    const auto Start = std::chrono::steady_clock::now();
    while(Iterations-- > 0)
        Function(StringData.ListOne, StringData.ListTwo);
    const auto End = std::chrono::steady_clock::now();        

    return std::format("{:.4f},", std::chrono::duration<double, std::milli>(End - Start).count()); 
}