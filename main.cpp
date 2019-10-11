#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int i, a, k, recall = 0;

struct Solicitante{
    string nombre;
    float renta, mediaCalificaciones;
};

void writeAceptado (string* aceptado){
    ofstream aceptxt;
    aceptxt.open ("aceptados.bec");
    for (i = 0; i < 3; i++){
        aceptxt << aceptado[i] << ' ';
    }
    aceptxt<<"\n";
    aceptxt.close();
}

void checkSolicitante (string* parsed) {
    string name = parsed[0];
    float renta = std::stof(parsed[1]);
    float nota = std::stof(parsed[2]);
    if (renta<22000 && nota > 6)
        writeAceptado (parsed);
}

void extractSolicitante (string line) {
    string parsed[3];
    for (i = recall; i < line.length(); i++) {
        if (line.at(i) == ' ') {
            recall = 1;
            char field[i-1];
            for (a = 0; a < i-1 ; a++) {
                field[a] = line.at(a);
            }
            parsed[k] = field;
            k++;
            if (k < 3 ) 
                extractSolicitante(line);
            else 
                break;
        }
    }
    recall = 0;
    checkSolicitante(parsed);
}

void readSolicitantes (){
    string line;
    ifstream solicitxt;
    solicitxt.open("solicitantes.txt"); 
    if (solicitxt.is_open()){
        while(getline(solicitxt, line))
            extractSolicitante(line);
    }else
        cout<<"No se pudo abrir solicitantes.txt. ¿Se encuentra en el directorio?";  
}

int main(){
    readSolicitantes();
}