#include <iostream>

#include "gtest/gtest.h"
#include "pugi_serializer.hpp"

struct attrib_values
{
    std::string attrib1_value;
    std::string attrib2_value;
};

static void serialize_attributes_with_defaults(pugi_serializer::serializer_base ser, attrib_values& values)
{
    auto node_Noddy = ser.child("Noddy");
    node_Noddy.attribute("attr1", values.attrib1_value, "YES");
    node_Noddy.attribute("attr2", values.attrib2_value, "YES");
}

TEST(TestSerializeDefaults, AttributeWithDefault)
{
    pugi::xml_document pdoc;
    {
        pugi_serializer::writer w(pdoc, "AttributeWithDefault");
        w.set_should_write_default_values(false);
        attrib_values values{"YES", "NO"};
        serialize_attributes_with_defaults(w, values);
        
        // xml should be: <AttributeWithDefault><node attr2="NO"></AttributeWithDefault>
        // pdoc.print(std::cerr, "");
        
        auto top_node = pdoc.document_element();
        EXPECT_STREQ(top_node.name(), "AttributeWithDefault") << "!>>> top node should be named AttributeWithDefault not " << top_node.name();
        EXPECT_TRUE(top_node.child("Noddy")) << "!>>> there shoud be a node named Noddy";
        EXPECT_FALSE(top_node.child("Noddy").attribute("attr1")) << "!>>> there should not be an attribute named attr1";
        EXPECT_TRUE(top_node.child("Noddy").attribute("attr2")) << "!>>> there should be an attribute named attr2";
        EXPECT_STREQ(top_node.child("Noddy").attribute("attr2").value(), "NO")<< "!>>>  value of attribute named attr2 expected 'NO' got '"<< top_node.child("Noddy").attribute("attr2").value() << "'";
    }
    {
        pugi_serializer::reader r(pdoc);
        attrib_values values;
        serialize_attributes_with_defaults(r, values);
        EXPECT_EQ(values.attrib1_value, "YES") << "!>>>  expected attrib1 with default value 'YES' got " << values.attrib1_value;
        EXPECT_EQ(values.attrib2_value, "NO") << "!>>>  expected attrib2 with value 'NO' got " << values.attrib2_value;
    }
}
