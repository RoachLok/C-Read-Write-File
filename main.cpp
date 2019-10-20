#include <iostream>
#include <fstream>
#include <boost/algorithm/string/predicate.hpp>
#include <bits/stdc++.h>
using namespace std;

int i=0, k=0, aceptados=0;
string parsed[3];
fstream aceptxt; ofstream acepbin;

void writeAceptado (string name, float income, string grade) { //Recibe un solicitante ya aceptado.
    aceptxt.open ("aceptrack.txt", ios::app); //Abre o crea aceptrack.txt y esribe en modo append.
    aceptxt << name << ' ' << income << ' ' << grade << endl; //Escribe en aceptrack.txt el solicitante aceptado para
    aceptxt.close();

    acepbin.write(name.c_str(), strlen(name.c_str())); //Guarda byte a byte los caracteres del string name.
    acepbin.write(reinterpret_cast<const char*>(&income), sizeof(float)); //Guarda 4bytes (float) que representan la renta.
    acepbin.write(grade.c_str(), 1); //Gurda un byte que contiene la información de la meia.

    aceptados++; //Lleva el recuento de los aceptados.
}

void checkSolicitante (string* parsed) { //Obtiene de parseSolicitante el array con los campos separados en cada posición.
    float income = std::stof(*(parsed+1)); //Recupera de parsed los campos donde se guardaron los campos numéricos y los pasa a float.
    float grade = std::stof(*(parsed+2));
    if (income<=22000 && grade > 6) //Comprueba si cumple los requisitos para recibir la beca.
        writeAceptado (*parsed, income, *(parsed+2));
}

void parseSolicitante (string line, int k, int i) { //Recibe una línea que no ha sido aceptada todavía de solicitantes.txt.
    if (k == 2) {
        *(parsed+k) = line.at(i); //Como el tercer campo es un solo caracter, lo introduce directamente en string parsed[3]
        checkSolicitante(parsed); //Pasa string parsed[] a checkSolicitante(string* parsed) con los campos del string ya separados.
    }else {
        string holder;
        for (i ; line.at(i) != ' ' ; i++)       //Lee caracter por caracter la línea hasta encontrar un espacio.
            holder += line.at(i);                   //Escribe cada char en su correspondiente espacio.

        *(parsed+k) = holder; 
        parseSolicitante(line, k+1, i+1);
    }
}

int main(){
    bool repeated = false; //Flag para marcar si un solicitante ya ha sido aceptado.
    string line, checker; 
    ifstream solicitxt;
    cout<<"Verificando lista de solicitantes..."<<endl<<endl;
    solicitxt.open("solicitantes.txt");

    if (solicitxt.is_open()) { //Comprueba si el archivo se ha abierto correctamente.
        acepbin.open ("aceptados.bec", ios::binary);    //Donde se guardarán los aceptados en binario.
        while(getline(solicitxt, line) && line != "") {                             //getline lee hasta encontrar /n, devuelve true y escribe lo obtenido en line. (line != "") Evita que las líneas en blanco se tengan en cuenta a la hora de buscar solicitantes.
            aceptxt.open("aceptrack.txt");
            while(getline(aceptxt, checker) && checker != "") { //Comprueba si la línea extraída de solicitantes.txt se encuentra ya en aceptrack.txt.
                if (/*line.compare(checker) == 0*/boost::iequals(line, checker)) {  //Compara ignorando upper case / lower case. #include <boost/algorithm/string/predicate.hpp>. Sin librería se puede usar el otro método.
                    repeated = true;                                                //Si hay coincidencia marca flag de repetida.
                    break;                                                          //Sale del while para no seguir buscando más líneas iguales.
                }
            }
            aceptxt.close();    
            if (!repeated) //Si se comprueba que el solicitante no está en la lista de aceptados, se pasa a parseSolicitnte.
                parseSolicitante(line, 0, 0); 
            repeated = false;
        }
        solicitxt.close();
        cout<<"Lista de aceptados creada en aceptrack.txt."<<endl<<"Total de aceptados: "<<aceptados<<"."<<endl<<endl<<"   ---- Saliendo ----";

    }else //Si no se ha abierto solicitantes.txt finaliza el programa.
        cout<<"No se pudo abrir solicitantes.txt. ¿Se encuentra en el directorio?";

    acepbin.close(); 
}