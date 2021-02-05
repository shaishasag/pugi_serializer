#include <iostream>
#include <string_view>

#include "gtest/gtest.h"
#include "pugi_serializer.hpp"

class ExampleWithTestBase : public testing::Test {
protected:
    const char* xml_to_read;
    pugi::xml_document rdoc;
    
    void read_with_pugi()
    {
        rdoc.load_string(xml_to_read);
    }
    
    std::ostringstream str_srm;
    pugi::xml_document wdoc;

    void write_with_pugi()
    {
        wdoc.print(str_srm, "", pugi::format_raw);
    }
    

    virtual void serialize(pugi_serializer::serializer_base& ser) = 0;
};

class NodeValueExample : public ExampleWithTestBase {
protected:
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        ser.child("node").text(node_value);
    }
    int node_value = -1;
};

// serialize simple integer value to/from content of an xml node
TEST_F(NodeValueExample, NodeValue)
{
    this->xml_to_read = R"(<doc><node>17</node></doc>)";

    {
        this->read_with_pugi(); // read xml using pugixml into this->rdoc
        
        pugi_serializer::reader read_ser(this->rdoc); // create a reading serializer
        this->serialize(read_ser);  // serialize the xml to members of NodeValueExample
        
        EXPECT_EQ(this->node_value, 17) << "expected this->node_value to be 17 got " << this->node_value;
    }

    {
        pugi_serializer::writer write_ser(this->wdoc, "doc"); // create a writinng serializer
        this->serialize(write_ser);  // serialize the xml from members of NodeValueExample

        this->write_with_pugi();  // write the xml to this->str_srm

        // xml that was create should identical to the one that was read above
        EXPECT_EQ(str_srm.str(), this->xml_to_read);
    }
}


class AttribValueExample : public ExampleWithTestBase {
protected:
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        ser.child("node").attribute("value", attrib_value);
    }
    int attrib_value = -1;
};

// serialize simple integer value to/from value of an xml attribute
TEST_F(AttribValueExample, AttribValue)
{
    this->xml_to_read = R"(<doc><node value="17"/></doc>)";
    
    {
        this->read_with_pugi(); // read xml using pugixml into this->rdoc
        
        pugi_serializer::reader read_ser(this->rdoc); // create a reading serializer attached to this->rdoc
        this->serialize(read_ser);  // serialize the xml to members of AttribValueExample
        
        EXPECT_EQ(this->attrib_value, 17) << "expected this->attrib_value to be 17 got " << this->attrib_value;
    }
    
    {
        pugi_serializer::writer write_ser(this->wdoc, "doc"); // create a writinng serializer
        this->serialize(write_ser);  // serialize the xml from members of NodeValueExample
        
        this->write_with_pugi();  // write the xml to this->str_srm
        
        // xml that was create should identical to the one that was read above
        EXPECT_EQ(str_srm.str(), this->xml_to_read);
    }
}
