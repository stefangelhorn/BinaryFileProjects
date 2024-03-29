#include <fstream>
#include <iostream>
#include <vector>

struct Person {
  std::string vorname;
  std::string nachname;

  void write_to_binary_stream(std::ostream &bos) const;
  void read_form_binary_stream(std::istream &bis);
};

int main() {
  std::vector<Person> persons;

  persons.push_back(Person{"Thomas", "Leiter"});
  persons.push_back(Person{"Stefan", "Gelhorn"});

  // Es wird ein Outputstream angelegt und der binary-Modus angegeben.
  std::ofstream fileOutput("binaryFile.txt", std::ios::binary);

  // Der Outputstream wird der Methode write_to_binary_stream übergeben.
  persons[0].write_to_binary_stream(fileOutput);

  fileOutput.close(); // Datei muss geschlossen werden, damit der Input wieder
                      // öffnen kann.

  std::ifstream fileInput("binaryFile.txt", std::ios::binary);
  persons[1].read_form_binary_stream(fileInput);

  std::cout << persons[1].vorname << ' ' << persons[1].nachname << '\n';

  return 0;
}

void Person::write_to_binary_stream(std::ostream &bos) const {
    int stringlength = vorname.size(); // 1. Schritt:in der int-Variablen stringlength speichern wir die Länge des binär zu speichernden Strings.
    auto slp = reinterpret_cast<char *>(&stringlength); // slp = stringlengthpointer: 
    // mit dem reinterpret_cast zu char* halten wir fest, dass nun Byteweise gelesen werden soll. Worauf der Pointer stößt, das soll als char-Buffer interpretiert werden. 
    // Und weil es ein Pointer ist, brauchen wir eine Adresse. Der Pointer soll auf die Adresse der in die Datei zu schreibenden Stringlänge zeigen

    // Der Outpoustream hat die Methode write. Diese benötigt einmal die Adresse, von der gelesen werden soll, als zweiten 
    // Parameter die Anzahl der Bytes, die nun zu lesen und in die Datei zu schreiben sind. In der Binärdatei steht nun in einer Länge von vier Byte die Größe des jetzt folgenden Strings. 
    // Da es im vorliegenden Beispiel um den Vornamen "Thomas" und somit eine Länge von 6 geht, steht nun zunächst in der Datei die zahl sechs (in binär).
    // Dies nimmt im vorliegenden Beispiel vier Byte ein, weil eben als int-Größe, und int hat in dieser Architektur vier Byte.
    bos.write(slp, sizeof(stringlength));


    // Die Angabe der Länge des folgenden Strings in der Binärdatei ist wichtig beim Auslesen.
    // Nun wird in den Outputstream wieder geschrieben. Als erster Parameter ist wieder wichtig, woraus gelesen werden soll. 
    // Gelesen werden soll der Vorname (Die Methode write braucht einen C-String) und als zweiter Parameter ist wieder wichtig, wie lang der String ist. 
    bos.write(vorname.c_str(), vorname.size());

  /*  Alternativ
          for(const auto& c : vorname)
          {
              bos.put(c);
          }
  */

    // Genau so wie beim Vornamen läuft es nun mit dem Nachnamen
    stringlength = nachname.size();
    slp = reinterpret_cast<char *>(&stringlength);
    bos.write(slp, sizeof(stringlength));

    bos.write(nachname.c_str(), nachname.size());
}

void Person::read_form_binary_stream(std::istream &bis) 
{
    int stringlength = 0;
    auto slp = reinterpret_cast<char *>(&stringlength); // slp = stringlengthpointer
    bis.read(slp, sizeof(stringlength));
    char buffer_vorname[stringlength + 1];
    bis.read(buffer_vorname, stringlength);
    buffer_vorname[stringlength] = '\0';
    vorname = buffer_vorname;

    stringlength = 0;
    slp = reinterpret_cast<char *>(&stringlength);
    bis.read(slp, sizeof(stringlength));
    char buffer_nachname[stringlength + 1];
    bis.read(buffer_nachname, stringlength);
    buffer_nachname[stringlength] = '\0';
    nachname = buffer_nachname;
}
