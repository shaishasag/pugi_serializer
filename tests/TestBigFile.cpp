#include <iostream>

#include "gtest/gtest.h"
#include "pugi_serializer.hpp"

class encompassed : public pugi_serializer::serialized_base
{
public:
    std::string continent;
    float percentage;
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        ser.attribute("continent", continent);
        ser.attribute("percentage", percentage);
    }
};

class border : public pugi_serializer::serialized_base
{
public:
    std::string country;
    float length;
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        ser.attribute("country", country);
        ser.attribute("length", length);
    }
};

// entity has an is and a name
class entity : public pugi_serializer::serialized_base
{
public:
    std::string id;
    std::string name;
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        ser.attribute("id", id);
        ser.attribute("name", name, "");
    }
};

class coordinates : public pugi_serializer::serialized_base
{
public:
    float longitude = 0.0;
    float latitude = 0.0;
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        ser.attribute("longitude", longitude);
        ser.attribute("latitude", latitude);
    }

};

class continent : public entity
{
public:
};

class percentage_value : public pugi_serializer::serialized_base
{
public:
    std::string name;
    float percentage = 0.0;
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        ser.attribute("percentage", percentage);
        ser.text(name);
    }
};

class city : public entity, coordinates
{
public:
    std::string country;
    int population_year = 0;
    unsigned int population = 0;

    void serialize(pugi_serializer::serializer_base& ser) override
    {
        // city has it's name as element text, nit as attribute so cannot call
        //entity::serialize(ser);
        
        ser.attribute("id", id);
        ser.child_and_text("name", name, "");

        ser.attribute("country", country);
        
        coordinates::serialize(ser);
        
        auto population_ser = ser.child_and_text("population", population, 0);
        // note that when wrtting and set_should_write_default_values(false),
        // attrib "year" will only be written if population!=0
        population_ser.attribute("year", population_year);
    }
};

class province : public entity
{
public:
    std::string country;
    std::string capital;
    unsigned int population = 0;
    unsigned int area = 0;
    std::vector<city> cities_vec;

    void serialize(pugi_serializer::serializer_base& ser) override
    {
        entity::serialize(ser);
        ser.attribute("country", country);
        ser.attribute("capital", capital);
        ser.attribute("population", population);
        ser.attribute("area", area);

        pugi_serializer::serialize_container(ser, cities_vec, "city");

    }
};

class country : public entity
{
public:
    std::string name_text;
    std::string capital;
    int population = 0;
    std::string datacode;
    int total_area = 0;
    double population_growth = 0.0;
    double infant_mortality = 0.0;
    double gdp_agri = 0.0;
    int gdp_total = 0;
    float inflation = 0.0;
    std::string indep_date;
    std::string government;
    std::string car_code;
    
    std::vector<city>             cities_vec;
    std::vector<province>         provinces_vec;
    std::vector<percentage_value> ethnicgroups_vec;
    std::vector<percentage_value> religions_vec;
    std::vector<percentage_value> languages_vec;
    std::vector<encompassed>      encompassed_vec;
    std::vector<border>           borders_vec;

    void serialize(pugi_serializer::serializer_base& ser) override
    {
        entity::serialize(ser);
        ser.attribute("capital", capital);
        ser.attribute("population", population);
        ser.attribute("total_area", total_area);
        ser.attribute("population_growth", population_growth);
        ser.attribute("infant_mortality", infant_mortality);
        ser.attribute("gdp_agri", gdp_agri);
        ser.attribute("gdp_total", gdp_total);
        ser.attribute("inflation", inflation, 0.0);
        ser.attribute("population", population);
        ser.attribute("indep_date", indep_date, "");
        ser.attribute("government", government);
        ser.attribute("car_code", car_code);
        
        ser.child("name").text(name_text);

        pugi_serializer::serialize_container(ser, cities_vec, "city");
        pugi_serializer::serialize_container(ser, provinces_vec, "province");
        pugi_serializer::serialize_container(ser, ethnicgroups_vec, "ethnicgroups");
        pugi_serializer::serialize_container(ser, religions_vec, "religions");
        pugi_serializer::serialize_container(ser, languages_vec, "languages");
        pugi_serializer::serialize_container(ser, encompassed_vec, "encompassed");
        pugi_serializer::serialize_container(ser, borders_vec, "border");
    }
};

class organization : entity
{
public:
    std::string abbrev;
    std::string established;
    std::string headq;
    
    class member : public pugi_serializer::serialized_base
    {
    public:
        std::string type;
        std::string country;
        void serialize(pugi_serializer::serializer_base& ser) override
        {
            ser.attribute("type", type);
            ser.attribute("country", country);
        }
    };
    std::vector<member> members_vec;
    
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        entity::serialize(ser);
        ser.attribute("abbrev", abbrev);
        ser.attribute("established", established, "");
        ser.attribute("headq", headq, "");

        pugi_serializer::serialize_container(ser, members_vec, "members");
    }
};

class located : public pugi_serializer::serialized_base
{
public:
    //std::vector<std::pair<std::string, std::string>> locations;
    std::string country;
    std::string province;
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        ser.attribute("country", country);
        ser.attribute("province", province);
    }
};

class feature : public entity, coordinates
{
public:
    std::vector<located> locations_vec;
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        entity::serialize(ser);
        coordinates::serialize(ser);
        pugi_serializer::serialize_container(ser, locations_vec, "located");
    }
};

class mountain : public feature
{
public:
    uint64_t height;
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        feature::serialize(ser);
        ser.attribute("height", height);
    }
};

//<mountain id='f0_36835'
//  name='Kilimanjaro'
//  longitude='37'
//  latitude='-3'
//  height='5895'>
//  <located
//    country='f0_1410'
//    province='f0_20031'/>
//</mountain>
//<desert id='f0_37221'
//  name='Mujunkum'
//  area='37500'>
//  <located
//    country='f0_538'
//    province='f0_18723'/>
//  <located
//    country='f0_538'
//    province='f0_18732'/>
//<island id='f0_37392'
//  name='Madagaskar'
//  area='587041'>
//  <located
//    country='f0_1307'
//    province='f0_19940'/>
//  <located
//    country='f0_1307'
//    province='f0_19941'/>
//<river id='f0_38145'
//  length='1230'
//  name='Cuango'>
//  <located
//    country='f0_1172'
//    province='f0_19842'/>
//<to
//    type='sea'
//    water='f0_38639'/>
//<sea id='f0_38020'
//   name='North Sea'
//   depth='200'>
//   <located
//     country='f0_325'
//     province='f0_17695'/>
//<lake id='f0_39083'
//   name='Ozero Baikal'
//   area='31500'>
//   <located
//     country='f0_358'
//     province='f0_18066'/>

class world : public pugi_serializer::serialized_base
{
public:
    std::vector<continent> continent_vec;
    std::vector<country> country_vec;
    std::vector<organization> organization_vec;
    std::vector<mountain> mountain_vec;
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        pugi_serializer::serialize_container(ser, continent_vec, "continent");
        pugi_serializer::serialize_container(ser, country_vec, "country");
        pugi_serializer::serialize_container(ser, organization_vec, "organization");
        pugi_serializer::serialize_container(ser, mountain_vec, "mountain");
    }
};

const char* big_file_name = "mondial-3.0.xml";
const char* big_file_out_name = "mondial-3.0.out.xml";
const char* big_file_ref_name = "mondial-3.0.ref.xml";

TEST(TestBigFile, read)
{
    pugi::xml_document doc;
    static unsigned int pugi_parse_options = pugi::parse_trim_pcdata | pugi::parse_embed_pcdata | pugi::parse_escapes | pugi::parse_cdata | pugi::parse_eol | pugi::parse_wconv_attribute;
    pugi::xml_parse_result pugi_parse_result = doc.load_file(big_file_name, pugi_parse_options);
    ASSERT_EQ(pugi::status_ok, pugi_parse_result.status) << "failed to read " << big_file_name;
    EXPECT_STREQ(doc.document_element().name(), "mondial") << "document_element is named " << doc.document_element().name() << " instead of " << "'mondial'";

    pugi_serializer::reader reader_serializer(doc);
    world w;
    w.serialize(reader_serializer);
    
    EXPECT_EQ(w.continent_vec.size(), 5) << "expected 5 continents";
    EXPECT_EQ(w.continent_vec.front().id, "f0_119") << "expected 1st continent id to be f0_119";
    EXPECT_EQ(w.continent_vec.front().name, "Europe") << "expected 1st continent name to be Europe";
    EXPECT_EQ(w.continent_vec.back().id, "f0_132") << "expected last continent id to be f0_132";
    EXPECT_EQ(w.continent_vec.back().name, "Africa") << "expected last continent name to be Africa";

    EXPECT_EQ(w.country_vec.size(), 231);
    
    int num_cities = 0;
    for (auto& country : w.country_vec)
        num_cities += country.cities_vec.size();
    EXPECT_EQ(num_cities, 557);
    
    int num_provinces = 0;
    int num_provinces_cities = 0;
    for (auto& country : w.country_vec) {
        num_provinces += country.provinces_vec.size();
        for (auto& province : country.provinces_vec) {
            num_provinces_cities += province.cities_vec.size();
        }
    }
    EXPECT_EQ(num_provinces, 1455) ;
    EXPECT_EQ(num_provinces_cities, 2595);

    int num_ethnicgroups = 0;
    for (auto& country : w.country_vec)
        num_ethnicgroups += country.ethnicgroups_vec.size();
    EXPECT_EQ(num_ethnicgroups, 515);
    
    int num_religions = 0;
    for (auto& country : w.country_vec)
        num_religions += country.religions_vec.size();
    EXPECT_EQ(num_religions, 446);
    
    int num_languages = 0;
    for (auto& country : w.country_vec)
        num_languages += country.languages_vec.size();
    EXPECT_EQ(num_languages, 116);
}

TEST(TestBigFile, write)
{
    pugi::xml_document read_doc;
    static unsigned int pugi_parse_options = pugi::parse_trim_pcdata | pugi::parse_embed_pcdata | pugi::parse_escapes | pugi::parse_cdata | pugi::parse_eol | pugi::parse_wconv_attribute;
    pugi::xml_parse_result pugi_parse_result = read_doc.load_file(big_file_name, pugi_parse_options);
    ASSERT_EQ(pugi::status_ok, pugi_parse_result.status) << "failed to read " << big_file_name;

    pugi_serializer::reader reader_serializer(read_doc);
    world w;
    w.serialize(reader_serializer);
    
    pugi::xml_document write_doc;
    pugi_serializer::writer writer_serializer(write_doc, "mondial");
    writer_serializer.set_should_write_default_values(false);
    w.serialize(writer_serializer);

    bool save_result = write_doc.save_file(big_file_out_name);
    ASSERT_TRUE(save_result);
    
//    EXPECT_STREQ(read_doc.document_element().name(), "mondial") << "document_element is named " << doc.document_element().name() << " instead of " << "'mondial'";

}
