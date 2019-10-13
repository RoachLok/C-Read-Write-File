#include <iostream>
#include <fstream>
#include <boost/algorithm/string/predicate.hpp>
using namespace std;

int i=0, k=0, aceptados=0;
string parsed[3];
fstream aceptxt;

void writeAceptado (string* aceptado) { //Recibe un solicitante ya aceptado.
    aceptxt.open ("aceptados.bec", ios::app | ios::binary); //Si no existe aceptados.bec lo crea. Escribe en modo append (empieza a escribir desde el final).
    aceptxt << *aceptado << ' ' << *(aceptado+1) << ' ' << *(aceptado+2) << endl; //Escribe en aceptados.bec
    aceptados++; //Lleva el recuento de los nuevos aceptados.
    aceptxt.close();
}

void checkSolicitante (string* parsed) { //Obtiene de parseSolicitante el array con los campos separados en cada posición.
    float renta = std::stof(*(parsed+1)); //Recupera de parsed los campos donde se guardaron los campos numéricos y los pasa a float.
    float nota = std::stof(*(parsed+2));
    if (renta<22000 && nota > 6) //Comprueba si cumple los requisitos para recibir la beca.
        writeAceptado (parsed);
}

void parseSolicitante (string line) { //Recibe una línea que no ha sido aceptada todavía de solicitantes.txt.
    string holder;
    for (i ; line.at(i) != ' ' ; i++) {     //Lee caracter por caracter la línea hasta encontrar un espacio.
        holder += line.at(i);               //Escribe cada char en holder.
    }
    *(parsed+k) = holder; //Guarda el string que contiene el "campo" en string parsed[].
    k++; i++; //Suma uno en i para saltarse el ' ' en el string line. k sirve para llevar constancia de cuales campos se han escrito ya en parsed.
    if (k < 2) 
        parseSolicitante(line); //Se vuelve a llamar a si mismo con i+1 para mantener la posición en el string line e ignorar el ' '.
    else { //Significa que los dos primeros campos de string parsed[] están completos.
        *(parsed+k) = line.at(i); //Como el tercer campo es un solo caracter, lo introduce directamente en string parsed[3]
        k = 0; i = 0; //Resetea las variables para la siguiente lectura de otro string.
        checkSolicitante(parsed); //Pasa string parsed[] a checkSolicitante(string* parsed) con los campos del string ya separados.
    }
}

int main(){
    bool repeated = false; //Flag para marcar si un solicitante ya ha sido aceptado.
    string line, checker; 
    ifstream solicitxt;
    cout<<"Verificando lista de solicitantes..."<<endl<<endl;
    solicitxt.open("solicitantes.txt");

    if (solicitxt.is_open()) { //Comprueba si el archivo se ha abierto correctamente.
        while(getline(solicitxt, line) && line != "") {                                         //getline lee hasta encontrar /n, devuelve true y escribe lo obtenido en line. (line != "") Evita que las líneas en blanco se tengan en cuenta a la hora de buscar solicitantes.
            aceptxt.open("aceptados.bec");
            while(getline(aceptxt, checker) && checker != "") { //Comprueba si la línea extraída de solicitantes.txt se encuentra ya en aceptados.bec.
                if (/*line.compare(checker) == 0*/boost::iequals(line, checker)) {              //Compara ignorando upper case / lower case.
                    repeated = true;                                                            //Si hay coincidencia marca flag de repetida.
                    break;                                                                      //Sale del while para no seguir buscando más líneas iguales.
                }
            }
            aceptxt.close();    
            if (!repeated) //Si se comprueba que el solicitante no está en la lista de aceptados, se pasa a parseSolicitnte.
                parseSolicitante(line); 
            repeated = false;
        }
        solicitxt.close();
        cout<<"Lista de aceptados creada en aceptados.bec."<<endl<<"Total de aceptados: "<< aceptados << "."
        <<endl<<endl<<"   ---- Saliendo ----";

    }else //Si no se ha abierto solicitantes.txt finaliza el programa.
        cout<<"No se pudo abrir solicitantes.txt. ¿Se encuentra en el directorio?";
    
}