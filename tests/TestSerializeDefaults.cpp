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

static void serialize_attributes_with_defaults(pugi_serializer::serializer_base ser, some_values& values)
{
    auto node_Noddy = ser.child("Noddy");
    node_Noddy.attribute("attr1", values.value1, "YES");
    node_Noddy.attribute("attr2", values.value2, "YES");
    
    // what if both create "Big_Ears" ?
    ser.child_and_attribute("Big_Ears", "attr3", values.value3, "YES");
    ser.child_and_attribute("Big_Ears", "attr4", values.value4, "YES");
}

TEST(TestSerializeDefaults, AttributeWithDefault)
{
    pugi::xml_document pdoc;
    {
        pugi_serializer::writer w(pdoc, "AttributeWithDefault");
        w.set_should_write_default_values(false);
        some_values values{"YES", "NO", "YES", "NO"};
        serialize_attributes_with_defaults(w, values);
        
        // xml should be: <AttributeWithDefault><node attr2="NO"></AttributeWithDefault>
        pdoc.print(std::cerr, "");
        
        auto top_node = pdoc.document_element();
        EXPECT_STREQ(top_node.name(), "AttributeWithDefault") << "!>>> expected top node named AttributeWithDefault not " << top_node.name();
        EXPECT_TRUE(top_node.child("Noddy")) << "!>>> expected a node named Noddy";
        EXPECT_FALSE(top_node.child("Noddy").attribute("attr1")) << "!>>> expected not to have an attribute named attr1";
        EXPECT_TRUE(top_node.child("Noddy").attribute("attr2")) << "!>>> expected an attribute named attr2";
        EXPECT_STREQ(top_node.child("Noddy").attribute("attr2").value(), "NO") << "!>>>  expected value of attribute named attr2 to be 'NO' got '"<< top_node.child("Noddy").attribute("attr2").value() << "'";
    }
    {
        pugi_serializer::reader r(pdoc);
        some_values values;
        serialize_attributes_with_defaults(r, values);
        EXPECT_EQ(values.value1, "YES") << "!>>>  expected value1 to be 'YES' got " << values.value1;
        EXPECT_EQ(values.value2, "NO") << "!>>>  expected value2 to be 'NO' got " << values.value2;
    }
}


static void serialize_values_with_defaults(pugi_serializer::serializer_base ser, some_values& values)
{
    auto node_Noddy = ser.child("Noddy");
    node_Noddy.child_and_text("value1", values.value1, "YES");
    node_Noddy.child_and_text("value2", values.value2, "YES");
}

TEST(TestSerializeDefaults, ValuesWithDefault)
{
    pugi::xml_document pdoc;
    {
        pugi_serializer::writer w(pdoc, "ValuesWithDefault");
        w.set_should_write_default_values(false);
        some_values values{"YES", "NO"};
        serialize_values_with_defaults(w, values);
        
        // xml should be: <AttributeWithDefault><value2>NO<value2></AttributeWithDefault>
        pdoc.print(std::cerr, "");
        
        auto top_node = pdoc.document_element();
        EXPECT_STREQ(top_node.name(), "ValuesWithDefault") << "!>>> expected top node named ValuesWithDefault not " << top_node.name();
        EXPECT_TRUE(top_node.child("Noddy")) << "!>>> expected a node named Noddy";
        EXPECT_FALSE(top_node.child("Noddy").child("value1")) << "!>>> expected not to have a node named value1";
        EXPECT_TRUE(top_node.child("Noddy").child("value2")) << "!>>> expected a node named value2";
        EXPECT_STREQ(top_node.child("Noddy").child("value2").text().as_string(), "NO")<< "!>>>  expected value of node named value2 to be 'NO' got '"<< top_node.child("Noddy").child("value2").text().as_string() << "'";
    }
    {
        pugi_serializer::reader r(pdoc);
        some_values values;
        serialize_values_with_defaults(r, values);
        EXPECT_EQ(values.value1, "YES") << "!>>>  expected value1 to be 'YES' got " << values.value1;
        EXPECT_EQ(values.value2, "NO") << "!>>>  expected value2 to be 'NO' got " << values.value2;
    }
}
