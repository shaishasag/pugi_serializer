pugi_serializer is a C++ XML serialisation library based on pugixml (https://pugixml.org/)

The main feature of pugi_serializer is that the same function is used to read and write the xml.

## Example
Here's an example of how code using pugi_serializer looks; it defines serialisation function for a class, and shows how to serialize to and from pugi::xml_node:

```c++
class Person
{
public:
    std::string m_first_name;
    std::string m_nickname;
    std::string m_last_name;
    unsigned m_age = 0;
};

// this functions defines how a Person object will be serialized from/to pugi::xml_node:
void serialize_person(Person& person, pugi_serializer::serilaizer_base in_ser)
{
    auto name_ser = in_ser.child("first_name");
    name_ser.text(person.m_first_name);
    name_ser.attribute("nickname", person.m_nickname, "");

    in_ser.child("last_name").text(person.m_last_name);
    in_ser.child("age").text(person.m_age, 0);
}

void write_and_read_serialization_example()
{
    // To write a Person object to xml_node,
    // create pugi_serializer::writer instance and call serialize_person:

    Person a_person{"William", "Bill", "Ockham", 62};
    pugi::xml_document person_doc;
    pugi_serializer::writer xml_writer(person_doc, "Person_List");
    serialize_person(a_person, xml_writer);

    // To read a Person object from xml_node,
    // create pugi_serializer::reader instance and call serialize_person:

    Person b_person;
    pugi_serializer::reader xml_reader(person_doc.document_element());
    serialize_person(b_person, xml_reader);

    // a_person & b_person should be the same
    assert(a_person == b_person);

    // output person_doc to stdout:
    person_doc.save(std::cout, "    ");
}
```

The XML created by the code above will be:

```xml
<?xml version="1.0"?>
<Person>
    <first_name nickname="Bill">William</first_name>
    <last_name>Ockham</last_name>
    <age>62</age>
</Person>
```

## License
Copyright (C) 2021, by Shai Shasag (shaishasag@yahoo.co.uk)
This library is available to anybody free of charge, under the terms of MIT License (see LICENSE.md).
