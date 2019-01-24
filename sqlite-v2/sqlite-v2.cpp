#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"
#include <vector>
#include <iostream>

using namespace Poco::Data::Keywords;
using namespace std;
using Poco::Data::Session;
using Poco::Data::Statement;

class Person
{
public:
    string getFirstName() const {
        return _firstName;
    }

    string getLastName() const {
        return _lastName;
    }

    uint32_t getSocialSecNr() const {
        return _socialSecNr;
    }

    void setFirstName(string firstName) {
        _firstName = firstName;
    }

    void setLastName(string lastName) {
        _lastName = lastName;
    }

    void setSocialSecNr(uint32_t socialSecNr) {
        _socialSecNr = socialSecNr;
    }

    bool operator <(const Person& p) const
        /// we need this for set and multiset support
    {
        return _socialSecNr < p._socialSecNr;
    }

    Poco::UInt64 operator()() const
        /// we need this operator to return the key for the map and multimap
    {
        return _socialSecNr;
    }

private:
    std::string _firstName;
    std::string _lastName;
    Poco::UInt64 _socialSecNr;
};

namespace Poco {
namespace Data {

template <>
class TypeHandler<class Person>
{
public:
    static void bind(std::size_t pos, const Person& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
    {
        poco_assert_dbg (!pBinder.isNull());
        // the table is defined as Person (FirstName VARCHAR(30), lastName VARCHAR, SocialSecNr INTEGER(3))
        // Note that we advance pos by the number of columns the datatype uses! For string/int this is one.
        pos -= 1;// pos start at 1
        TypeHandler<std::string>::bind(pos++, obj.getFirstName(), pBinder, dir);
        TypeHandler<std::string>::bind(pos++, obj.getLastName(), pBinder, dir);
        TypeHandler<Poco::UInt64>::bind(pos++, obj.getSocialSecNr(), pBinder, dir);
    }

    static std::size_t size()
    {
        // cout << "ckt test2" << endl;
        return 3; // we handle three columns of the Table!
    }

    static void prepare(std::size_t pos, const Person& obj, AbstractPreparator::Ptr pPrepare)
    {
        poco_assert_dbg (!pPrepare.isNull());
        // the table is defined as Person (FirstName VARCHAR(30), lastName VARCHAR, SocialSecNr INTEGER(3))
        // Note that we advance pos by the number of columns the datatype uses! For string/int this is one.
        pos -= 1;// pos start at 1
        TypeHandler<std::string>::prepare(pos++, obj.getFirstName(), pPrepare);
        TypeHandler<std::string>::prepare(pos++, obj.getLastName(), pPrepare);
        TypeHandler<Poco::UInt64>::prepare(pos++, obj.getSocialSecNr(), pPrepare);
    }

    static void extract(std::size_t pos, Person& obj, const Person& defVal, AbstractExtractor::Ptr pExt)
        /// obj will contain the result, defVal contains values we should use when one column is NULL
    {
        poco_assert_dbg (!pExt.isNull());
        std::string firstName;
        std::string lastName;
        Poco::UInt64 socialSecNr = 0;

        pos -= 1;// pos start at 1
        TypeHandler<std::string>::extract(pos++, firstName, defVal.getFirstName(), pExt);
        TypeHandler<std::string>::extract(pos++, lastName, defVal.getLastName(), pExt);
        TypeHandler<Poco::UInt64>::extract(pos++, socialSecNr, defVal.getSocialSecNr(), pExt);
        obj.setFirstName(firstName);
        obj.setLastName(lastName);
        obj.setSocialSecNr(socialSecNr);
    }

private:
    TypeHandler();
    ~TypeHandler();
    TypeHandler(const TypeHandler&);
    TypeHandler& operator=(const TypeHandler&);
};

} } // namespace Poco::Data

int main(int argc, char **argv) {
    // register SQLite connector
    Poco::Data::SQLite::Connector::registerConnector();

    // create a session
    Session session("SQLite", "sample.db");
    // Insert some values to Person table

    typedef Poco::Tuple<Poco::UInt64, Person> Persons;
    std::vector<Persons> people;
    uint32_t count;
    session << "SELECT * FROM Person", into(people), range(1, 10), now;
    cout << "count: " << people.size() << endl;
    for (std::vector<Persons>::const_iterator it = people.begin();
         it != people.end(); ++it) {
        std::cout << it->get<0>() << ",  " 
        << it->get<1>().getFirstName() << ",  " 
        << it->get<1>().getSocialSecNr() << ",  " 
        << it->get<1>().getLastName() << endl;
    }
    return 0;
}
