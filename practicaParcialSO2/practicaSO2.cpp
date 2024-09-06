#include <iostream>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <vector>
#include <map>
#include <queue>

#define MAX_SIZE 1000
#define MAX_LINEA 1000

using namespace std;

map<string, map<string, string> > procesos;
map<string, map<string, string> > copiaProcesos;


// map<string, int>atendidos;

queue<string> nivel1;
queue<string> nivel2;

queue<string> espera;
// queue<string> fcfs;
// queue<string> fcfsEspera;
// queue<string> atendidosnivel1;

int timeCpu = 0;
int quantum = 3;
int cantProcesos = 0;

// void imprimirTiemposDeEspera(string tipoDato) {
//     int suma = 0;
//     int promedio;
//     cout << endl << "_______TIEMPOS PROMEDIO_______" << endl << endl;
    
//     // Iterador explícito para el mapa de procesos
//     map<string, map<string, string> >::iterator it;
    
//     for (it = procesos.begin(); it != procesos.end(); ++it) {
//         const string& proceso = it->first;
//         const map<string, string>& datos = it->second;
        
//         int wt = stoi(datos.at(tipoDato)); 
//         suma+=wt;
//         promedio = suma/cantProcesos;
//         cout << wt <<endl;
        
//         printf("Proceso %s tiene un tiempo de espera de %d. \n", proceso.c_str(), wt);
//     }
//     printf("Promedio de %s es : %d\n", tipoDato.c_str(), promedio);
// }


// void imprimirTiemposRespuesta() {

//     cout << endl << "_______TIEMPOS RESPUESTA_______" << endl << endl;
    
//     map<string, map<string, string> >::iterator it;
    
//     for (it = procesos.begin(); it != procesos.end(); ++it) {
//         const string& proceso = it->first;
//         const map<string, string>& datos = it->second;
        
//         int rt = stoi(datos.at("rt")); 
        
//         printf("Proceso %s tiene un tiempo de respuesta de %d. \n", proceso.c_str(),rt);
//     }
// }

void roundRobin() {
    int wt = 0;
    cout << endl <<"_____EN EJECUCION ROUND ROBIN_____" << endl << endl;
    while (!nivel1.empty()) {
        string proceso = nivel1.front();

        if (!nivel1.empty()) {
            nivel1.pop();
            int bursTime = stoi(procesos[proceso]["BursTime"]);
            int anivel1ivalTime = stoi(procesos[proceso]["Anivel1ivalTime"]);
            int cont = stoi(procesos[proceso]["procesado"])+1;
            procesos[proceso]["procesado"] = to_string(cont);
            if (bursTime > quantum) {

                procesos[proceso]["BursTime"] = to_string(bursTime - quantum);
                if (cont == 1){
                    procesos[proceso]["rt"] = to_string(timeCpu);
                }
                timeCpu +=quantum;

                

                if (espera.empty()){
                    nivel2.push(proceso);

                }else{
                    while (!espera.empty()) 
                    {
                        string procesoEspera = espera.front();
                        int arrivalTime = stoi(procesos[procesoEspera]["Anivel1ivalTime"]);

                        if (anivel1ivalTime <= timeCpu) {
                            espera.pop();
                            // nivel1.push(procesoEspera);
                            cout << "se agrego proceso " << procesoEspera << " a la cola en tiempo " << timeCpu << endl;
                        } else {
                            break;
                        }
                    }
                    nivel2.push(proceso);
                }

                printf("Proceso %s no terminó en cuántum y en tiempo %d\n", proceso.c_str(), timeCpu);

            } else {
                
                if (cont == 1){
                    procesos[proceso]["rt"] = to_string(timeCpu);
                }
                timeCpu += bursTime;

                procesos[proceso]["BursTime"] = "0";
                procesos[proceso]["ct"] = to_string(timeCpu);
                // int tl = stoi(procesos[proceso]["rt"]);
                int bt = stoi(copiaProcesos[proceso]["BursTime"]);
                int ct = stoi(procesos[proceso]["ct"]);
                int turnAround = ct - anivel1ivalTime;
                int waiting = turnAround - bt;
                // printf("turn anivel1ound %d of process %s \n", turnAround, proceso.c_str());
                // int waiting = ct - (tl+bt);
                // printf(" tl %d  bt %d ct %d  de el proceso: %s\n", tl,bt,ct, proceso.c_str());
                procesos[proceso]["wt"] = to_string(waiting);
                printf("Proceso %s terminó en cuántum y en tiempo %d\n", proceso.c_str(), timeCpu);
                

                
            }

        }
    }
}




void algoritmosJuntos(){


    roundRobin();
    // cout << endl <<"_____EN EJECUCION FCFS_____" << endl << endl;
    // while (!fcfs.empty()) 
    // // cout << "_____EN EJECUCION FCFS_____" << endl;
    // {
    //     if (nivel1Espera.empty())
    //     {   

    //         string proceso = fcfs.front();
    //         fcfs.pop();
    //         int bursTime = stoi(procesos[proceso]["BursTime"]);
    //         int anivel1ival = stoi(procesos[proceso]["Anivel1ivalTime"]);


    //         // Ejecutar el proceso FCFS
    //         timeCpu += bursTime;
    //         procesos[proceso]["ct"] = to_string(timeCpu);
    //         int ct = stoi(procesos[proceso]["ct"]);;
    //         int turnAround = ct - anivel1ival;
    //         int waiting = turnAround - bursTime;
    //         procesos[proceso]["wt"] = to_string(waiting);
    //         printf("Proceso %s finalizó en FCFS en tiempo %d \n", proceso.c_str(), timeCpu);

    //         while (!fcfsEspera.empty()) {
    //             string procesoEspera = fcfsEspera.front();
    //             int anivel1ivalTime = stoi(procesos[procesoEspera]["Anivel1ivalTime"]);

    //             if (anivel1ivalTime <= timeCpu) {
    //                 fcfsEspera.pop();
    //                 fcfs.push(procesoEspera);
    //             } else {
    //                 break;
    //             }
    //         }
    //     }else
    //     {
    //         string procesoF = fcfs.front();
    //         int bursTime = stoi(procesos[procesoF]["BursTime"]);
    //         timeCpu+=1;
    //         bursTime-=1;
    //         if (bursTime == 0){
    //             fcfs.pop();
    //             printf("Proceso %s finalizó en FCFS en tiempo %d\n", procesoF.c_str(), timeCpu);
    //         }else{
    //             printf("Proceso %s no finalizó en FCFS en tiempo %d\n", procesoF.c_str(), timeCpu);
    //             // cout << "burstTime " << bursTime << endl;
    //             procesos[procesoF]["BursTime"] = to_string(bursTime);
    //             while (!nivel1Espera.empty()) {
    //                 string procesoEspera = nivel1Espera.front();
    //                 int anivel1ivalTime = stoi(procesos[procesoEspera]["Anivel1ivalTime"]);

    //                 if (anivel1ivalTime <= timeCpu) {
    //                     nivel1Espera.pop();
    //                     nivel1.push(procesoEspera);
    //                     cout << "se agrego proceso " << procesoEspera << " a la cola nivel1 en tiempo " << timeCpu << endl;
    //                 } else {
    //                     break;
    //                 }
    //             }
    //             if (!nivel1.empty()){
    //                 roundRobin();
    //             }
    //             cout << endl <<"_____EN EJECUCION FCFS_____" << endl << endl;
    //         }
    //     }
    // } 

    // firstComeFirstServed();
    int tamCola2 = nivel2.size();

    cout<< endl << "-------Tiempo de cpu: " << timeCpu << "tamaño cola 2 -> " <<tamCola2<<"-------" <<endl;

}

void procesamientosDatos(const vector<vector<string> >& instrucciones) {
    string proceso;
    int anivel1ivalTime;
    int bursTime;
    int algoritmo;
    int wt = 0;
    int ct = 0;
    int rt = 0;
    int tat = 0;
    int procesado = 0;
    
    for (size_t j = 0; j < instrucciones.size(); ++j) {
        cantProcesos+=1;

        proceso = instrucciones[j][0];
        anivel1ivalTime = stoi(instrucciones[j][1]);
        bursTime = stoi(instrucciones[j][2]);
        algoritmo = stoi(instrucciones[j][3]);

        // Guardar en cola conivel1espondiente
        if (anivel1ivalTime == 0){
            nivel1.push(proceso);
        }else{
            espera.push(proceso);
        }
        // Guardar en el mapa
        procesos[proceso]["Anivel1ivalTime"] = to_string(anivel1ivalTime);
        procesos[proceso]["BursTime"] = to_string(bursTime);
        procesos[proceso]["Algoritmo"] = to_string(algoritmo);
        procesos[proceso]["wt"] = to_string(wt);
        procesos[proceso]["ct"] = to_string(ct);
        procesos[proceso]["rt"] = to_string(rt);
        procesos[proceso]["tat"] = to_string(tat);
        procesos[proceso]["procesado"] = to_string(procesado);


    }

    // Ejecutar Round Robin y luego FCFS
    copiaProcesos = procesos;
    algoritmosJuntos();

}

void lecturaArchivo() {
    char lista[MAX_SIZE][MAX_LINEA];
    int iterador = 0;

    FILE* archivo = fopen("entrada.txt", "r");
    if (archivo == NULL) {
        perror("Fallo el intento de abrir el archivo");
        return;
    }

    char linea[MAX_LINEA];
    while (fgets(linea, sizeof(linea), archivo) && iterador < MAX_SIZE) {
        strncpy(lista[iterador], linea, MAX_LINEA - 1);
        lista[iterador][MAX_LINEA - 1] = '\0'; 
        iterador++;
    }

    fclose(archivo);

    vector<vector<string> > instrucciones;
    
    for (int i = 0; i < iterador; i++) {
        istringstream iss(lista[i]);
        string palabra;
        vector<string> partes;

        while (iss >> palabra) {
            partes.push_back(palabra);
        }

        instrucciones.push_back(partes);
    }

    procesamientosDatos(instrucciones);
}

int main() {
    lecturaArchivo();
    // imprimirCopiaProcesos();
    // imprimirTiemposDeEspera("wt");
    // imprimirTiemposRespuesta();
    // imprimirTiemposDeEspera("wt");
    return 0;
}