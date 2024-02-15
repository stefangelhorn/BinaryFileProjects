#include <iostream>
#include <fstream>

struct Instrument
{
    std::string name;
    std::string material;
    double preis;
    void write_to_binary(std::string filename);
};


int main()
{
    Instrument geige{"Geige", "Ebenholz", 699.00};



    return 0;
}

void Instrument::write_to_binary(std::string filename)
{
    std::ofstream file(filename, std::ios::binary);

    if(!file.is_open())
    {
        std::cout << "Datei konnte nicht geöffnet werden!\n";
        return;
    }

    int stringlength = name.size();
    auto ptr = reinterpret_cast<char*>(&stringlength);
    file.write(ptr, sizeof(stringlength));
    file.write(name.c_str(), stringlength);

    stringlength = material.size();
    ptr = reinterpret_cast<char*>(&stringlength);
    file.write(ptr, sizeof(stringlength));
    file.write(material.c_str(), stringlength);

    ptr = reinterpret_cast<char*>(&preis);
    file.write(ptr, sizeof(preis));

    file.close();
}