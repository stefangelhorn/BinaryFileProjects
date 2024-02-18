/**
 * @file binaryfile1.cpp
 * @brief Beispielcode zum Speichern und Lesen von einer eigenen Datenstruktur mit Strings in eine Binärdatei.
 * @date 15.02.2024 – 17.02.2024
 * @author Qualidy GmbH
 * 
 */
#include <fstream>
#include <iostream>

/**
 * Templatefunktion um beliebige Datentypen binär zu speichern.
 */
template <typename T> void write_obj_binary(const T &obj, std::ostream &bos) {
  auto ptr = reinterpret_cast<const char *>(&obj);
  bos.write(ptr, sizeof(obj));
}

/**
 * Spezialisierte Templatefunktion für T=std::string um diesen binär zu speichern.
 * 
 * Dazu wird erst die Anzahl zu speichernder Zeichen, dann die Zeichen selbst geschrieben.
 */
template <> void write_obj_binary(const std::string &str, std::ostream &bos) {
  int stringlength = str.size();
  auto slp = reinterpret_cast<char *>(&stringlength);
  bos.write(slp, sizeof(stringlength));
  bos.write(str.c_str(), stringlength);
}

/**
 * Templatefunktion um beliebige Datentypen binär zu laden.
 */
template <typename T> void read_obj_binary(T &obj, std::istream &bis) {
  auto ptr = reinterpret_cast<char *>(&obj);
  bis.read(ptr, sizeof(obj));
}


/**
 * Spezialisierte Templatefunktion für T=std::string um diesen binär zu laden.
 * 
 * Dazu wird erst die Anzahl Zeichen für den String geladen, dann die Zeichen selbst.
 */
template <> void read_obj_binary(std::string &str, std::istream &bis) {
  int stringlength = 0;
  read_obj_binary(stringlength, bis);
  char buffer[stringlength + 1];
  bis.read(buffer, stringlength);
  buffer[stringlength] = '\0';
  str = buffer;
}

/**
 * Eine Klasse mit verschiedenen Datentypen um die Speicherfunktionen zu zeigen
 */
struct Book {
  std::string title;
  std::string autor;
  short release;
  int pages;
  double price;

    /**
     * Hilfsmemberfunktion, schreibt Stringdaten in output file stream.
     * 
     * `str` ist der Name der Datei in die geschrieben werden soll.
     * `bos` ist der Ausgabestrom, in den geschrieben werden soll.
     */
  void write_string_binary(const std::string &str, std::ostream &bos) const {
    int stringlength = str.size();
    auto slp = reinterpret_cast<char *>(&stringlength);
    bos.write(slp, sizeof(stringlength));
    bos.write(str.c_str(), stringlength);
  }

    /**
     * Memberfunktion, schreibt Buch-Objekt binär in Datei.
     * 
     * `filename` ist der Name der Datei in die geschrieben werden soll.
     */
  void write_to_binary(const std::string &filename) const;
  
    /**
     * Memberfunktion, liest Buch-Objekt aus Binärdatei.
     * 
     * `filename` ist der Name der Datei aus der gelesen werden soll.
     */
  void read_from_binary(const std::string &filename);
};
/**
 * Memberfunktion um das gesamte Objekt in eine Binärdatei zu speichern.
 * 
 * Nutzt dazu die globalen Funktionen für einzelne Member.
 */
void Book::write_to_binary(const std::string &filename) const {
  std::ofstream file(filename, std::ios::binary);

  if (!file.is_open()) {
    std::cout << "Something went wrong. File could not be opened!\n";
    return;
  }

  // title reinschreiben
  write_obj_binary(title, file);

  // autor reinschreiben
  write_obj_binary(autor, file);

  // release reinschreiben
  write_obj_binary(release, file);

  // pages reinschreiben
  write_obj_binary(pages, file);

  // price reinschreiben
  write_obj_binary(price, file);
}

/**
 * Memberfunktion um das gesamte Objekt wieder aus der Binärdatei heraus zu lesen.
 * 
 * Nutzt dazu die globalen Funktionen für einzelne Member.
 */
void Book::read_from_binary(const std::string &filename) {
  std::ifstream file(filename, std::ios::binary);

  if (!file.is_open()) {
    std::cout << "Something went wrong! File could not be opened\n";
    return;
  }

  // title auslesen
  read_obj_binary(title, file);

  // autor auslesen
  read_obj_binary(autor, file);

  // releaase auslesen
  read_obj_binary(release, file);

  // pages auslesen
  read_obj_binary(pages, file);

  // price auslesen
  read_obj_binary(price, file);
}

/**
 * Einstiegspunkt in Programm, schreibt `b1` in Datei und läd diese in `b2`.
 */
int main() {
  Book b1{"Achtung, C++", "Thomas Mahr", 2019, 460, 29.99};
  Book b2{"A", "B", 1, 1, 8.15};

  b1.write_to_binary("Books.bin");
  b2.read_from_binary("Books.bin");

  std::cout << b2.title << ' ' << b2.autor << ' ' << b2.pages << ' '
            << b2.release << ' ' << b2.price << '\n';

  return 0;
}
