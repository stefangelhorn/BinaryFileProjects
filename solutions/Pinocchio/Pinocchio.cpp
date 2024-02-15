#include <iostream>
#include <string>
#include <vector>
#include <fstream>

/**
 * Aufgaben
 *   1) Ermögliche es dem Benutzer eine von ihm festgelete Menge an Personen (in einen std::vector) anzulegen.
 *      Am Ende des Programms sollen die Benutzer in eine Binärdatei schreiben.
 *   2) Das Programm soll beim Start die Benutzerdatei in den Vektor einlesen und ausgeben.
 *   3) Schreibe die Klasse so um, dass sie einen Geburtstag speichert (std::chrono) und daraus das Alter errechnet und die Tage angibt, die seit dem letzten Geburtstag vergangen sind.
 *      Natürlich muss jetzt das Dateiformat angepasst werden.
 *   4) Erweitere das Dateiformat, dass der Zeitpunkt mitgespeichert wird, andem eine Person angelegt wird. Diese Angabe soll vor allen Benutzern in der Datei geschrieben werden, FALLS es weniger als 7 Personen sind, andernfalls wird nach jedem Personenblock dierekt das Erstellungsdatum für diese Person geschrieben. Dieser Zeitpunkt ist nicht Teil der Personenklasse. 
 *   5) Ermögliche das Löschen von Personen über Angabe des Namens. Diese sollen direkt aus der Datei gelöscht werden und der Inhalt des Vektors dann angepasst werden.
 *   6) Ermögliche es, eine Version des Dateiformats mitzuspeichern, so dass das Programm je nach Version andere Lade/Speichermöglichkeiten bietet um Aufwärtskompatiblität zu gewährleisten.
 *   7) Optional: Tausch Quellcode und Binärdateien zwischen einem Windows und einem Mac/Linux System aus. Kompiliere den Code neu und lade die mitgelieferte Datei. Schaue, ob eine so erstellte Datei fehlerfrei eingelesen werden kann.
 */

struct Person
{
    inline static const std::string filename = "binaryPersons.bin";
    std::string firstname;
    std::string lastname;
    int age;

    // Standard-Konstruktor
    Person(){}
    // Überladener Konstruktor
    Person(const std::string& _firstname, const std::string& _lastname, int _age)
    :firstname(_firstname), lastname(_lastname), age(_age){}


    void writeToBinary() const;
    void readBinary(std::ifstream& file);
};

void addPerson(std::vector<Person>& persons);
std::ostream& operator<<(std::ostream& os, const Person p);

void writeStringBinary(std::ofstream& ofs, const std::string& s)
{
    if(!ofs)
    {
        std::cerr << "Fehler beim Schreiben der Binärdatei!\n";
        return;
    }
        auto ptr = s.c_str();
        ofs.write(ptr, s.size());
}

void writeStringsizeBinary(std::ofstream& ofs, const std::string& s)
{
    int strlen = s.size();
    auto ptr = reinterpret_cast<char*>(&strlen);
    ofs.write(ptr, sizeof(strlen));
}

void Person::writeToBinary() const
{
    std::ofstream file(filename, std::ios::out | std::ios::binary | std::ios::app);
    
    if(!file.is_open())
    {
        std::cerr << "Fehler beim Oeffnen der Datei!\n";
        return;
    }

/*Header der Binarydatei:
1. Punkt: Größe des Vornamenstrings einer Person| 
2. Punkt: Einzelne Bytes des Vornames|
3. Punkt: Größe des Nachnamenstrings einer Person|
4. Punkt: Einzelne Bytes des Nachnames
5. Punkt: Wert des Alters|
*/
    // 1. Punkt
    writeStringsizeBinary(file, firstname);
    // 2. Punkt
    writeStringBinary(file, firstname);
    // 3. Punkt
    writeStringsizeBinary(file, lastname);
    // 4. Punkt
    writeStringBinary(file, lastname);
    // 5. Punkt
    auto ptr = (char*)&age;
    file.write(ptr, sizeof(age));
}

int readStringsizeBinary(std::ifstream& ifs)
{
    int stringlength;
    auto ptr = reinterpret_cast<char*>(&stringlength);
    ifs.read(ptr, sizeof(stringlength));
    return stringlength;
}

void readStringBinary(std::ifstream& ifs, std::string& s, int stringlength)
{
    if(!ifs)
    {
        std::cerr << "Fehler beim Lesen der Binärdatei!\n";
        return;
    }

    char* buffer = new char[stringlength + 1];
    ifs.read(buffer, stringlength);
    buffer[stringlength] = '\0';
    s = buffer;

    delete[] buffer;
    buffer = nullptr;
}

void Person::readBinary(std::ifstream& file)
{
    if(!file.is_open())
    {
        std::cerr << "Fehler beim Oeffnen der Datei!\n";
        return;
    }

    /* 1. Punkt: Stringlänge einlesen
       2. Punkt: String (firstname) )einlesen
    */

    // 1. Punkt
    int stringlength = readStringsizeBinary(file);
    // 2. Punkt    
    readStringBinary(file, firstname, stringlength);
    // 3. Punkt
    stringlength = readStringsizeBinary(file);
    // 4. Punkt
    readStringBinary(file, lastname, stringlength);
    // 5. Punkt
    auto ptr = reinterpret_cast<char*>(&age);
    file.read(ptr,sizeof(age));
}




int main()
{
    std::vector<Person> persons;

    int choice;
    do
    {
        addPerson(persons);
        std::cout << "Weitere Person anlegen? Ja(1) / Nein(0) ";
        std::cin >> choice;
    } while (choice);

    for(const auto& p : persons)
    {
        p.writeToBinary();
        std::cout << p;
    }

    std::ifstream file(Person::filename, std::ios::binary);

    if(!file)
    {
        std::cerr << "Fehler beim Öffnen der Datei!\n";
        return 1;
    }
    std::vector<Person> eingelesenePersonen;
    Person tempPerson;
    while(file.peek() != EOF)
    {
        tempPerson.readBinary(file);
        eingelesenePersonen.push_back(tempPerson);
    }
    std::cout << "\n\nEingelesene Personen:\n";
    for(const auto& person : eingelesenePersonen)
    {
        std::cout << person;
    }

    return 0;
}

void addPerson(std::vector<Person>& persons)
{
    std::string first, last;
    int age;
    std::cout << "Firstname: ";
    std::cin >> first;
    std::cout << "Lastname: ";
    std::cin >> last;
    std::cout << "Age: ";
    std::cin >> age;

    persons.emplace_back(first, last, age);
}

std::ostream& operator<<(std::ostream& os, const Person p)
{
    return os << p.firstname << ' ' << p.lastname << " (" << p.age << ")\n";
}