#include <iostream>

#include "gtest/gtest.h"
#include "pugi_serializer.hpp"


struct some_values
{
    std::string value1;
    std::string value2;
    std::string value3;
    std::string value4;
};

bool operator==(const some_values& lhs, const some_values& rhs)
{
    return
        lhs.value1 == rhs.value1 &&
        lhs.value2 == rhs.value2 &&
        lhs.value3 == rhs.value3 &&
        lhs.value4 == rhs.value4;
}
static void serialize_attributes_with_defaults(pugi_serializer::serializer_base ser, some_values& values)
{
    auto node_Noddy = ser.child("Noddy");
    node_Noddy.attribute("attr1", values.value1, "YES");
    node_Noddy.attribute("attr2", values.value2, "YES");
    
    // what if both create "Big_Ears" ?
    ser.child_with_attribute("Big_Ears", "attr3", values.value3, "YES");
    ser.child_with_attribute("Big_Ears", "attr4", values.value4, "YES");
}

TEST(TestSerializeDefaults, AttributeWithDefault)
{
    pugi::xml_document pdoc;
    some_values values_to_write{"YES", "NO", "YES", "NO"};
    {
        pugi_serializer::writer w(pdoc, "AttributeWithDefault");
        w.set_should_write_default_values(false);
        
        serialize_attributes_with_defaults(w, values_to_write);
        
        auto expected_xml = R"(<AttributeWithDefault><Noddy attr2="NO"/><Big_Ears attr4="NO"/></AttributeWithDefault>)";
        std::ostringstream oss;
        pdoc.save(oss, "", pugi::format_raw|pugi::format_no_declaration);
        
        EXPECT_STREQ(oss.str().c_str(), expected_xml);
    }
    {
        pugi_serializer::reader r(pdoc);
        some_values values_to_read;
        
        serialize_attributes_with_defaults(r, values_to_read);

        // compare written values to read values - they should be the same
        EXPECT_EQ(values_to_read, values_to_write);
    }
}

static void serialize_values_with_defaults(pugi_serializer::serializer_base ser, some_values& values)
{
    auto node_Noddy = ser.child("Noddy");
    node_Noddy.child_with_text("value1", values.value1, "YES");
    node_Noddy.child_with_text("value2", values.value2, "YES");
}

TEST(TestSerializeDefaults, ValuesWithDefault)
{
    pugi::xml_document pdoc;
    some_values values_to_write{"YES", "NO"};
    {
        pugi_serializer::writer w(pdoc, "ValuesWithDefault");
        w.set_should_write_default_values(false);
        
        serialize_values_with_defaults(w, values_to_write);
        
        // xml should be:
        auto expected_xml = R"(<ValuesWithDefault><Noddy><value2>NO</value2></Noddy></ValuesWithDefault>)";
        std::ostringstream oss;
        pdoc.save(oss, "", pugi::format_raw|pugi::format_no_declaration);
        
        EXPECT_STREQ(oss.str().c_str(), expected_xml);
    }
    {
        pugi_serializer::reader r(pdoc);
        some_values values_to_read;
        
        serialize_values_with_defaults(r, values_to_read);
        
        // compare written values to read values - they should be the same
        EXPECT_EQ(values_to_read, values_to_write);
    }
}
