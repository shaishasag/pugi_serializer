#include <iostream>

#include "gtest/gtest.h"
#include "pugi_serializer.hpp"


TEST(TestConstruction, writer_ctor1)
{
    pugi::xml_document doc;
    
    pugi_serializer::writer w(doc);
    EXPECT_TRUE(bool(w)) << "properly constructed pugi_serializer::writer should evaluate to true";
    EXPECT_TRUE(w.writing()) << "pugi_serializer::writer::writing() should return true";
    EXPECT_FALSE(w.reading()) << "pugi_serializer::writer::reading() should return false";
}

TEST(TestConstruction, writer_ctor2)
{
    pugi::xml_document doc;
    
    pugi_serializer::writer w(doc, "DocElementName");
    EXPECT_TRUE(bool(w.curr_node())) << "pugi_serializer::writer constructed with doc_element_name should have valid curr_node";
    EXPECT_STREQ(w.curr_node().name(), "DocElementName");
}

TEST(TestConstruction, set_get_should_write_default_values)
{
    pugi::xml_document doc;
    
    pugi_serializer::writer w(doc);
    EXPECT_TRUE(w.get_should_write_default_values()) << "pugi_serializer::writer::get_should_write_default_values() should evaluate to true by default";
    
    w.set_should_write_default_values(false);
    EXPECT_FALSE(w.get_should_write_default_values()) << "pugi_serializer::writer::get_should_write_default_values() should be false after calling set_should_write_default_values(false)";
    
    w.set_should_write_default_values(true);
    EXPECT_TRUE(w.get_should_write_default_values()) << "pugi_serializer::writer::get_should_write_default_values() should be true after calling set_should_write_default_values(true)";
}


TEST(TestConstruction, reader_ctor)
{
    pugi::xml_document doc;
    
    pugi_serializer::reader r(doc);
    EXPECT_FALSE(bool(r)) << "pugi_serializer::reader should evaluate to false before reading anything";
    EXPECT_FALSE(r.writing()) << "pugi_serializer::reader::writing() should return false";
    EXPECT_TRUE(r.reading()) << "pugi_serializer::reader::reading() should return true";
}
