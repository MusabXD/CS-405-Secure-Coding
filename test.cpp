#include "pch.h"
// uncomment the next line if you do not use precompiled headers
//#include "gtest/gtest.h"

class Environment : public ::testing::Environment
{
public:
    ~Environment() override {}

    void SetUp() override
    {
        srand(time(nullptr));
    }

    void TearDown() override {}
};

class CollectionTest : public ::testing::Test
{
protected:
    std::unique_ptr<std::vector<int>> collection;

    void SetUp() override
    {
        collection.reset(new std::vector<int>);
    }

    void TearDown() override
    {
        collection->clear();
        collection.reset(nullptr);
    }

    void add_entries(int count)
    {
        assert(count > 0);
        for (auto i = 0; i < count; ++i)
            collection->push_back(rand() % 100);
    }
};

// Predefined Tests
TEST_F(CollectionTest, CollectionSmartPointerIsNotNull)
{
    ASSERT_TRUE(collection);
    ASSERT_NE(collection.get(), nullptr);
}

TEST_F(CollectionTest, IsEmptyOnCreate)
{
    ASSERT_TRUE(collection->empty());
    ASSERT_EQ(collection->size(), 0);
}

// Test to show failure (used for demonstration)
//TEST_F(CollectionTest, AlwaysFail)
//{
//    FAIL();
//}

// Test: Add a single value
TEST_F(CollectionTest, CanAddToEmptyVector)
{
    ASSERT_TRUE(collection->empty());
    add_entries(1);
    EXPECT_EQ(collection->size(), 1);
}

// Test: Add five values
TEST_F(CollectionTest, CanAddFiveValuesToVector)
{
    ASSERT_TRUE(collection->empty());
    add_entries(5);
    EXPECT_EQ(collection->size(), 5);
}

// Test: max_size >= size for various sizes
TEST_F(CollectionTest, MaxSizeGreaterThanOrEqualToSize)
{
    std::vector<int>::size_type sizes[] = { 1, 5, 10 };
    for (auto sz : sizes)
    {
        collection->clear();
        add_entries(sz);
        EXPECT_GE(collection->max_size(), collection->size());
    }
}

// Test: capacity >= size for various sizes
TEST_F(CollectionTest, CapacityGreaterThanOrEqualToSize)
{
    std::vector<int>::size_type sizes[] = { 1, 5, 10 };
    for (auto sz : sizes)
    {
        collection->clear();
        add_entries(sz);
        EXPECT_GE(collection->capacity(), collection->size());
    }
}

// Test: resizing increases collection size
TEST_F(CollectionTest, ResizeIncreasesCollection)
{
    collection->resize(5);
    EXPECT_EQ(collection->size(), 5);
    collection->resize(10);
    EXPECT_EQ(collection->size(), 10);
}

// Test: resizing decreases collection size
TEST_F(CollectionTest, ResizeDecreasesCollection)
{
    collection->resize(10);
    collection->resize(5);
    EXPECT_EQ(collection->size(), 5);
}

// Test: resizing to zero clears collection
TEST_F(CollectionTest, ResizeDecreasesToZero)
{
    collection->resize(5);
    collection->resize(0);
    EXPECT_TRUE(collection->empty());
}

// Test: clear() removes all elements
TEST_F(CollectionTest, ClearErasesCollection)
{
    add_entries(5);
    collection->clear();
    EXPECT_TRUE(collection->empty());
    EXPECT_EQ(collection->size(), 0);
}

// Test: erase(begin, end) removes all elements
TEST_F(CollectionTest, EraseRangeErasesCollection)
{
    add_entries(5);
    collection->erase(collection->begin(), collection->end());
    EXPECT_TRUE(collection->empty());
}

// Test: reserve() increases capacity but not size
TEST_F(CollectionTest, ReserveIncreasesCapacityButNotSize)
{
    add_entries(2);
    auto old_size = collection->size();
    auto old_capacity = collection->capacity();
    collection->reserve(old_capacity + 10);
    EXPECT_EQ(collection->size(), old_size);
    EXPECT_GT(collection->capacity(), old_capacity);
}

// Negative Test: accessing out of range throws exception
TEST_F(CollectionTest, ThrowsExceptionWhenAccessingOutOfBounds)
{
    add_entries(3);
    EXPECT_THROW(collection->at(10), std::out_of_range);
}

// Negative Test: accessing with negative index (cast)
TEST_F(CollectionTest, ThrowsExceptionWhenAccessingNegativeIndex)
{
    add_entries(3);
    EXPECT_THROW(collection->at(static_cast<size_t>(-1)), std::out_of_range);
}

// Custom Positive Test: Values in 0–99 range
TEST_F(CollectionTest, ValuesAreInExpectedRange)
{
    add_entries(100);
    for (auto val : *collection)
    {
        EXPECT_GE(val, 0);
        EXPECT_LT(val, 100);
    }
}
