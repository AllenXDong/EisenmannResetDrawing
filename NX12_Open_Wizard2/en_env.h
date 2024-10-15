/****************************************************************************************
 *   Author:        Michael Künne
 *   Datum:         03.04.2009
 *   Beschreibung:  Klasse für Umgebungsvariable
 ***************************************************************************************/

#include <string>

#define DllExport __declspec(dllexport)

class ENenv {
public:
    DllExport ENenv(const char *varName);
    DllExport ~ENenv();

    DllExport bool exists();
    DllExport const char *valToSZ();
    DllExport const std::string &valToString();
    DllExport const char *nameToSZ();
    DllExport const std::string &nameToString();
    DllExport const int &length();

    DllExport const bool contains(const char *innerEnvValue) const;
    DllExport const bool contains(const std::string &innerEnvValue) const;
    DllExport const bool contains(ENenv &innerEnv) const;

    DllExport void write(const char *newValue);
    DllExport void write(const std::string &newValue);

private:
    ENenv();

    char *_sz;
    bool _exists;
    std::string _name;
    std::string _value;
    int _length;
};