#pragma once
#include <thread>
#include "Templates.h"

class StringInsertion
{
public:
    
    static void InsertIntoVector(const VectorStrings& A, VectorStrings& B);
    static void InsertIntoSet(const VectorStrings& A, SetStrings& B);
    static void InsertIntoMap(const VectorStrings& A, MapStrings& B);
};

class BinarySearch
{
public:
    
    static int StringCaseSensitive(const VectorStrings& A, const VectorStrings& B);
    static int StringIgnoreCase(const VectorStrings& A, const VectorStrings& B);
    static int HashCaseSensitive(const VectorStrings& A, const VectorHashes& B);
    static int HashIgnoreCase(const VectorStrings& A, const VectorHashes& B);
};

// Methods to compare two strings in a linear search are VERY SLOW. I don't recommend past 10k words. 
class LinearSearch
{
public:

    // Compare the two strings by size, then by letter in a linear search, using lower case when the letter fails. -- VERY SLOW -- 
    static int MyMethod(const VectorStrings& A, const VectorStrings& B);

    // Default == operator to compare two strings. -- VERY SLOW -- 
    static int EqualOperator(const VectorStrings& A, const VectorStrings& B);

    // Hash the string then search for it in the Vector Hashes Array. Case Sensitive. -- VERY SLOW -- 
    static int HashCaseSensitive(const VectorStrings& A, const VectorHashes& B);

    // Get the Lowercase version of the string, hash it, then search for it in the Vector Hashes Array. -- VERY SLOW --
    static int HashIgnoreCase(const VectorStrings& A, const VectorHashes& B);
};

class SearchByKey
{
public:

    static int StringSetCaseSensitive(const VectorStrings& A, const SetStrings& B);
    static int StringSetIgnoreCase(const VectorStrings& A, const SetStrings& B);
    static int HashMapCaseSensitive(const VectorStrings& A, const MapHashes& B);
    static int HashMapIgnoreCase(const VectorStrings& A, const MapHashes& B);
    static int StringMapCaseSensitive(const VectorStrings& A, const MapStrings& B);
    static int StringMapIgnoreCase(const VectorStrings& A, const MapStrings& B);
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
        : TestBase(std::move(WordsCount), TestSetup), bUseHash(HashString){ } 

protected:    
    void InitTest() override;
    
private:    
    bool bUseHash;         
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
    UnorderedMapTest(WordsCount WordsCount, const FTestSetup& TestSetup, bool HashString)
        : TestBase(std::move(WordsCount), TestSetup), bUseHash(HashString){ }
    
protected:

    void InitTest() override;

private:

    bool bUseHash; 
};

enum class EInsertionType
{
    Vector,
    Set,
    Map
};

class InsertionTest : public TestBase
{
public:
    
    InsertionTest(WordsCount WordsCount, const FTestSetup& TestSetup, const EInsertionType& InsertionType)
        : TestBase(std::move(WordsCount), TestSetup), mInsertionType(InsertionType) { }
    
protected:

    void InitTest() override;

private:

    EInsertionType mInsertionType; 
    
};

class TestEntryPoint
{
public:

    TestEntryPoint(WordsCount InWordsCount); 

    void StartTests();
    void SaveResults(const bool bPrintToConsole); 

private:
    
    static void ValidateInput(); 

    WordsCount mWordsPerTest;
    TTests mTests; 

};
