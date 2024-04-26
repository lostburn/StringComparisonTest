#pragma once
#include <thread>
#include "Templates.h"

class BinarySearch
{
public:
    
    static int StringCaseSensitive(const VectorStrings& A, const VectorStrings& B);
    static int StringIgnoreCase(const VectorStrings& A, const VectorStrings& B);
    static int HashCaseSensitive(const VectorStrings& A, const VectorHashes& B);
    static int HashIgnoreCase(const VectorStrings& A, const VectorHashes& B);
};

class DefaultSearch
{
public:

    static int MyMethod(const VectorStrings& A, const VectorStrings& B);
    static int EqualOperator(const VectorStrings& A, const VectorStrings& B);
};

class ArrayHashSearch
{
public:

    // Hash the string then search for it in the Vector Hashes Array. Case Sensitive.
    static int HashCaseSensitive(const VectorStrings& A, const VectorHashes& B);

    // Get the Lowercase version of the string, hash it, then search for it in the Vector Hashes Array.
    static int HashLowerCase(const VectorStrings& A, const VectorHashes& B);
};

class SearchByKey
{
public:

    static int StringSetCaseSensitive(const VectorStrings& A, const SetStrings& B);
    static int StringSetLowercase(const VectorStrings& A, const SetStrings& B);
    static int HashMapCaseSensitive(const VectorStrings& A, const MapHashes& B);
    static int HashMapLowercase(const VectorStrings& A, const MapHashes& B);   
};

struct FTestSetup
{
    FTestSetup(std::string InName, bool IgnoreCase = false) : mName(std::move(InName)), bIgnoreCase(IgnoreCase) { } 
    std::string mName;
    bool bIgnoreCase;    
};

class TestBase
{
public:

    TestBase() = default; 
    explicit TestBase(WordsCount Words, const FTestSetup& Setup)
        : mWordsPerTest(std::move(Words)), mTestName(Setup.mName), bIgnoreCase(Setup.bIgnoreCase) { }

    virtual ~TestBase() = default;

    void Start()
    {
        std::thread Thread([this](){
            InitTest();
        });

        Thread.join(); 
    }
    
    [[nodiscard]] std::string GetResult() const { return mTestName + ", " + mResult; }

protected:

    virtual void InitTest() = 0;

    WordsCount mWordsPerTest;
    std::string mTestName;
    bool bIgnoreCase = false;
    VectorStrings ListOne;     
    std::string mResult;  
};

class BinarySearchTest : public TestBase
{    
public:    
    BinarySearchTest(WordsCount WordsCount, const FTestSetup& TestSetup, bool HashString)
        : TestBase(std::move(WordsCount), TestSetup), bHashString(HashString){ } 

protected:    
    void InitTest() override;
    
private:    
    bool bHashString;         
};

class UnorderedSetTest : public TestBase
{
public:
    UnorderedSetTest(WordsCount WordsCount, const FTestSetup& TestSetup)
        : TestBase(std::move(WordsCount), TestSetup){ }

protected:
    void InitTest() override;
    
};

class UnorderedMapTest : public TestBase
{
public:
    UnorderedMapTest(WordsCount WordsCount, const FTestSetup& TestSetup)
        : TestBase(std::move(WordsCount), TestSetup){ }
    
protected:

    void InitTest() override;
};

class TestEntryPoint
{
public:

    TestEntryPoint(WordsCount InWordsCount)
        : mWordsPerTest(std::move(InWordsCount)) { } 

    void StartTests();
    void SaveResults(const bool bPrintToConsole); 

private:

    WordsCount mWordsPerTest;
    TTests mTests; 

};
