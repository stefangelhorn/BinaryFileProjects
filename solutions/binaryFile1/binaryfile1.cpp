/**
 * @file binaryfile1.cpp
 * @brief Beispielcode zum Speichern und Lesen von Strings in Binärdateien.
 * @date 16.02.2024 – 17.02.2024
 * @author Qualidy GmbH
 * 
 * /// Sind Erklärungen hinter Code um diesen zu erklären
 * // Sind Erklärungen, die dem Code voran gehen.
 * /* Leitet optionalen Code samt Erklärungen dazu innerhalb dessen ein.
 * /** In solchen Blöcken stehen Zusammenfassungen Funktionen und Klassen.
 */
#include <fstream>
#include <iostream>
#include <vector>

/**
 * Die Person struct repräsentiert eine Person mit einem Vor- und einem Nachnamen.
 */
struct Person
{
    std::string vorname; /// Vorname der Person.
    std::string nachname; /// Nachname der Person

    /**
     * Schreibt von this (Person-Object) in output file stream (Ausgabedateistrom).
     * 
     * `bos` ist der Ausgabestrom, in den geschrieben werden soll
     */
    void write_to_binary_stream( std::ostream& bos ) const;

    /**
     * Liest aus Dateistrom in this (Person-Object).
     * 
     * `bis` ist der Eingabestrom von dem gelesen werden soll.
     */
    void read_from_binary_stream( std::istream& bis );
};

/**
 * Einstiegspunkt in Programm, dass die Anwendung der Klasse zeigt
 */
int main()
{
    std::vector< Person > persons;

    persons.push_back( Person { "Thomas", "Leiter" } );
    persons.push_back( Person { "Stefan", "Gelhorn" } );


    // Optionales öffnen eines Scopes/Gültigkeitsbereichs ... Wegen 1)
    {
        // Es wird ein Outputstream angelegt und der binary-Modus angegeben.
        std::ofstream fileOutput( "binaryFile.txt", std::ios::binary );

        // Der Outputstream wird der Methode write_to_binary_stream übergeben.
        persons[0].write_to_binary_stream( fileOutput );

        // 1) Stream muss geschlossen sein, damit ein anderer Stream Datei öffnen
        //   kann. Dies geschieht entweder ...
        // ... manuell mit
        fileOutput.close();
    }  /// ... oder automatisch, wenn das Stream-Objekt »out of scope« gerät.

    std::ifstream fileInput( "binaryFile.txt", std::ios::binary );

    // Wir überschreiben das zweite Vektor-Element -- Stefan Gelhorn -- mit den
    // Strings der ersten gespeicherten Person in der Datei (Thomas Leiter).
    persons[1].read_form_binary_stream( fileInput );

    std::cout << persons[1].forename << ' ' << persons[1].surname << '\n';

    return 0;
}


void Person::write_to_binary_stream( std::ostream& bos ) const
{
    // 1. Schritt: in der int-Variablen stringlength speichern wir die Länge des binär zu speichernden Strings.
    int stringlength =
        forename.size();
    
    // Mit dem reinterpret_cast zu char* halten wir fest, dass nun Byteweise gelesen
    // werden soll. Worauf der Pointer stößt, das soll als char-Buffer interpretiert
    // werden. Und weil es ein Pointer ist, brauchen wir eine Adresse. Der Pointer
    // soll auf die Adresse der in die Datei zu schreibenden Stringlänge zeigen 
    auto slp =
        reinterpret_cast< char* >( &stringlength ); /// slp = stringlengthpointer:
    

    // Der Outpoustream hat die Methode write. Diese benötigt einmal die Adresse, von
    // der gelesen werden soll, als zweiten Parameter die Anzahl der Bytes, die nun
    // zu lesen und in die Datei zu schreiben sind. In der Binärdatei steht nun in
    // einer Länge von vier Byte die Größe des jetzt folgenden Strings. Da es im
    // vorliegenden Beispiel um den Vornamen "Thomas" und somit eine Länge von 6
    // geht, steht nun zunächst in der Datei die zahl sechs (in binär). Dies nimmt im
    // vorliegenden Beispiel vier Byte ein, weil eben als int-Größe, und int hat in
    // dieser Architektur vier Byte.
    bos.write( slp, sizeof( stringlength ) );


    // Die Angabe der Länge des folgenden Strings in der Binärdatei ist wichtig beim
    // Auslesen. Nun wird in den Outputstream wieder geschrieben. Als erster
    // Parameter ist wieder wichtig, woraus gelesen werden soll. Gelesen werden soll
    // der Vorname (Die Methode write braucht einen C-String) und als zweiter
    // Parameter ist wieder wichtig, wie lang der String ist.
    bos.write( forename.c_str(), forename.size() );

    /*
    // Alternativ Zeichen/Byte für Zeichen/Byte
        for(const auto& c : vorname)
        {
            bos.put(c);
        }
    */

    // Genau so wie beim Vornamen läuft es nun mit dem Nachnamen
    stringlength = surname.size();
    slp          = reinterpret_cast< char* >( &stringlength );
    
    bos.write( slp, sizeof( stringlength ) );
    bos.write( surname.c_str(), surname.size() );
}

void Person::read_form_binary_stream( std::istream& bis )
{
    // Hier werden Stringlängen für Vor- und Nachnamen hier rein geschrieben bzw. überschrieben.
    int stringlength = 0; 
    
    // Zeiger auf die Adresse für die read-Funktion.
    auto slp = reinterpret_cast< char* >( &stringlength ); 
    
    // Einlesen in stringlength, da in dieser Reihenfolge in Binärdatei geschrieben.
    bis.read( slp, sizeof( stringlength ) );
    /*
    // Alternativschreibweise für Codezeile von zuvor:
    bis.read((char*)&stringlength, sizeof(stringlength)); /// C++ geht dann
    /// verschiedene casts durch und wendet den ersten an, der Funktioniert. Da es
    /// sich hier um neuinterpretation/Typumwandlung von gelesenen char* nach int
    /// handelt, trifft reinterpret_cast<char *> automatisch zu.
    */
    
    // Hier lesen wir dann die Anzahl chars in den Buffer und weisen dem Vornamen
    // dann den buffer zu.
    char buffer_vorname[stringlength + 1];
    bis.read( buffer_vorname, stringlength );
    buffer_vorname[stringlength] = '\0';
    forename                     = buffer_vorname;

    // Selbe Schritte wie oben für Nachnamen
    stringlength = 0;
    slp          = reinterpret_cast< char* >( &stringlength );
    bis.read( slp, sizeof( stringlength ) );
    char buffer_nachname[stringlength + 1];
    bis.read( buffer_nachname, stringlength );
    buffer_nachname[stringlength] = '\0';
    surname                       = buffer_nachname;
}
