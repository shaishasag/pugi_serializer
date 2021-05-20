#include <iostream>

#include "gtest/gtest.h"
#include "pugi_serializer.hpp"


TEST(TestProperties, IsReadingOrWriting)
{
    pugi::xml_document doc;
    
    pugi_serializer::writer w(doc);
    EXPECT_TRUE(bool(w)) << "properly constructed pugi_serializer::writer should evaluate to true";
    EXPECT_TRUE(w.writing()) << "pugi_serializer::writer::writing() should return true";
    EXPECT_FALSE(w.reading()) << "pugi_serializer::writer::reading() should return false";
    
    pugi_serializer::reader r(doc);
    EXPECT_FALSE(bool(r)) << "pugi_serializer::reader should evaluate to false before reading anything";
    EXPECT_FALSE(r.writing()) << "pugi_serializer::reader::writing() should return false";
    EXPECT_TRUE(r.reading()) << "pugi_serializer::reader::reading() should return true";
}

TEST(TestProperties, should_write_default_values)
{
    pugi::xml_document doc;
    
    pugi_serializer::writer w(doc);
    EXPECT_TRUE(w.get_should_write_default_values()) << "default constructed pugi_serializer::writer should should_write_default_values to true";
    
    w.set_should_write_default_values(false);
    EXPECT_FALSE(w.get_should_write_default_values()) << "failed to set should_write_default_values to false";
    w.set_should_write_default_values(true);
    EXPECT_TRUE(w.get_should_write_default_values()) << "failed to set should_write_default_values to true";
}
