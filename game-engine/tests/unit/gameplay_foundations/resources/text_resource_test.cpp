// Unit tests for TextResource class using Google Test

#include <gtest/gtest.h>
#include <fstream>
#include <string>

#include "gameplay_foundations/resources/text_resource.hpp"

TEST(TextResourceTest, Constructor) {
    nathan::TextResource resource("test.txt");
    EXPECT_EQ(resource.get_path(), "test.txt");
    EXPECT_EQ(resource.get_type(), nathan::Resource::Type::Text);
    EXPECT_FALSE(resource.is_loaded());
}

TEST(TextResourceTest, LoadExistingFile) {
    const char kPath[] = "test_text_resource.txt";
    const char kContent[] = "Hello, World!";
    
    // Create test file
    {
        std::ofstream file(kPath);
        file << kContent;
    }
    
    nathan::TextResource resource(kPath);
    EXPECT_TRUE(resource.load());
    EXPECT_TRUE(resource.is_loaded());
    EXPECT_EQ(resource.get_content(), kContent);
    
    // Cleanup
    std::remove(kPath);
}

TEST(TextResourceTest, LoadMissingFile) {
    nathan::TextResource resource("nonexistent.txt");
    EXPECT_FALSE(resource.load());
    EXPECT_FALSE(resource.is_loaded());
    EXPECT_TRUE(resource.get_content().empty());
}

TEST(TextResourceTest, Unload) {
    const char kPath[] = "test_text_unload.txt";
    const char kContent[] = "Test content";
    
    // Create test file
    {
        std::ofstream file(kPath);
        file << kContent;
    }
    
    nathan::TextResource resource(kPath);
    EXPECT_TRUE(resource.load());
    EXPECT_TRUE(resource.is_loaded());
    EXPECT_EQ(resource.get_content(), kContent);
    
    resource.unload();
    EXPECT_FALSE(resource.is_loaded());
    EXPECT_TRUE(resource.get_content().empty());
    
    // Cleanup
    std::remove(kPath);
}

TEST(TextResourceTest, IsLoaded) {
    nathan::TextResource resource("test.txt");
    EXPECT_FALSE(resource.is_loaded());
    
    // Create a temp file
    const char kPath[] = "test_isloaded.txt";
    {
        std::ofstream file(kPath);
        file << "content";
    }
    resource = nathan::TextResource(kPath);
    EXPECT_FALSE(resource.is_loaded());
    EXPECT_TRUE(resource.load());
    EXPECT_TRUE(resource.is_loaded());
    
    // Cleanup
    std::remove(kPath);
}

TEST(TextResourceTest, GetContent) {
    const char kPath[] = "test_getcontent.txt";
    const std::string kContent = "Line 1\nLine 2\nLine 3";
    
    // Create test file
    {
        std::ofstream file(kPath);
        file << kContent;
    }
    
    nathan::TextResource resource(kPath);
    EXPECT_TRUE(resource.load());
    EXPECT_EQ(resource.get_content(), kContent);
    
    // Cleanup
    std::remove(kPath);
}

TEST(TextResourceTest, EmptyFile) {
    const char kPath[] = "test_empty.txt";
    
    // Create empty test file
    {
        std::ofstream file(kPath);
    }
    
    nathan::TextResource resource(kPath);
    EXPECT_TRUE(resource.load());
    EXPECT_TRUE(resource.is_loaded());
    EXPECT_TRUE(resource.get_content().empty());
    
    // Cleanup
    std::remove(kPath);
}
