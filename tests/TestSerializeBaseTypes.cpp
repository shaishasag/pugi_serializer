#include <iostream>

#include "gtest/gtest.h"
#include "pugi_serializer.hpp"

// Tests for write/read of different base types, double, float, int, etc.
// list of types is determined by what is available in pugixml.
// values are writen as:
// - content of xml element, e.g. <node>17</node>
// - value of an attribute, e.g. <node attrib="17></node>
// The value are then read from content and attribute and compared to the expected values

template <class T>
class TestSerializeBaseTypes : public testing::Test {
protected:

    T write_to_text = 0;
    T write_to_attrib = 0;
    T read_from_text = 0;
    T read_from_attrib = 0;
    
    pugi::xml_document pdoc;
    
    void init(T write_to_text_, T write_to_attrib_, T read_from_text_, T read_from_attrib_)
    {
        write_to_text = write_to_text_;
        write_to_attrib = write_to_attrib_;
        read_from_text = read_from_text_;
        read_from_attrib = read_from_attrib_;
    }
    
    void write()
    {
        pugi_serializer::writer w(pdoc, "TestSerializeBaseTypes");
        auto write_node = w.child("value");
        write_node.text(write_to_text);
        write_node.attribute("attrib", write_to_attrib);
    }
    
    void read()
    {
        pugi_serializer::reader r(pdoc);
        auto read_node = r.child("value");
        read_node.text(read_from_text);
        read_node.attribute("attrib", read_from_attrib);
    }
};

using testing::Types;

// test signed types
typedef Types<int, long long> signed_base_type;
template <class T>
class TestSerializeSignedTypes : public TestSerializeBaseTypes<T> {};
TYPED_TEST_SUITE(TestSerializeSignedTypes, signed_base_type);
TYPED_TEST(TestSerializeSignedTypes, SignedReadWrite)
{
    this->init(-171, -1917, 10001, 101);
    this->write();
    //this->pdoc.print(std::cerr, "    ");
    this->read();
    
    EXPECT_EQ(this->write_to_text, this->read_from_text);
    EXPECT_EQ(this->write_to_attrib, this->read_from_attrib);
}

// test unsigned types
typedef Types<unsigned, unsigned long long> unsigned_base_type;
template <class T>
class TestSerializeUnsignedTypes : public TestSerializeBaseTypes<T> {};
TYPED_TEST_SUITE(TestSerializeUnsignedTypes, unsigned_base_type);
TYPED_TEST(TestSerializeUnsignedTypes, UnsignedReadWrite)
{
    this->init(5566, 9988, 10001, 101);
    this->write();
    //this->pdoc.print(std::cerr, "    ");
    this->read();
    
    EXPECT_EQ(this->write_to_text, this->read_from_text);
    EXPECT_EQ(this->write_to_attrib, this->read_from_attrib);
}

// test floating point types
typedef Types<float, double> float_base_type;
template <class T>
class TestSerializeFloatTypes : public TestSerializeBaseTypes<T> {};
TYPED_TEST_SUITE(TestSerializeFloatTypes, unsigned_base_type);
TYPED_TEST(TestSerializeFloatTypes, float_base_type)
{
    this->init(-1.0304, 198.456, -1, -2);
    this->write();
    //this->pdoc.print(std::cerr, "    ");
    this->read();
    
    EXPECT_EQ(this->write_to_text, this->read_from_text);
    EXPECT_EQ(this->write_to_attrib, this->read_from_attrib);
}

// test bool type
typedef Types<bool> bool_base_type;
template <class T>
class TestSerializeBoolTypes : public TestSerializeBaseTypes<T> {};
TYPED_TEST_SUITE(TestSerializeBoolTypes, bool_base_type);
TYPED_TEST(TestSerializeBoolTypes, bool_base_type)
{
    this->init(true, false, false, true);
    this->write();
    //this->pdoc.print(std::cerr, "    ");
    this->read();
    
    EXPECT_EQ(this->write_to_text, this->read_from_text);
    EXPECT_EQ(this->write_to_attrib, this->read_from_attrib);
}
