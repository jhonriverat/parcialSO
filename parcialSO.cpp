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

queue<string> rr;
queue<string> rrEspera;
queue<string> fcfs;
queue<string> fcfsEspera;
// queue<string> atendidosRR;

int timeCpu = 0;
int quantum = 3;
int cantProcesos = 0;

void imprimirTiemposDeEspera(string tipoDato) {
    int suma = 0;
    int promedio;
    cout << endl << "_______TIEMPOS PROMEDIO_______" << endl << endl;
    
    // Iterador explícito para el mapa de procesos
    map<string, map<string, string> >::iterator it;
    
    for (it = procesos.begin(); it != procesos.end(); ++it) {
        const string& proceso = it->first;
        const map<string, string>& datos = it->second;
        
        int wt = stoi(datos.at(tipoDato)); 
        suma+=wt;
        promedio = suma/cantProcesos;
        cout << wt <<endl;
        
        printf("Proceso %s tiene un tiempo de espera de %d. \n", proceso.c_str(), wt);
    }
    printf("Promedio de %s es : %d\n", tipoDato.c_str(), promedio);
}


void imprimirTiemposRespuesta() {

    cout << endl << "_______TIEMPOS RESPUESTA_______" << endl << endl;
    
    map<string, map<string, string> >::iterator it;
    
    for (it = procesos.begin(); it != procesos.end(); ++it) {
        const string& proceso = it->first;
        const map<string, string>& datos = it->second;
        
        int rt = stoi(datos.at("rt")); 
        
        printf("Proceso %s tiene un tiempo de respuesta de %d. \n", proceso.c_str(),rt);
    }
}

void roundRobin() {
    int wt = 0;
    cout << endl <<"_____EN EJECUCION ROUND ROBIN_____" << endl << endl;
    while (!rr.empty()) {
        string proceso = rr.front();

        if (!rr.empty()) {
            rr.pop();
            int bursTime = stoi(procesos[proceso]["BursTime"]);
            int arrivalTime = stoi(procesos[proceso]["ArrivalTime"]);
            int cont = stoi(procesos[proceso]["procesado"])+1;
            procesos[proceso]["procesado"] = to_string(cont);
            if (bursTime > quantum) {

                procesos[proceso]["BursTime"] = to_string(bursTime - quantum);
                if (cont == 1){
                    procesos[proceso]["rt"] = to_string(timeCpu);
                }
                timeCpu +=quantum;

                

                if (rrEspera.empty()){
                    rr.push(proceso);

                }else{
                    while (!rrEspera.empty()) 
                    {
                        string procesoEspera = rrEspera.front();
                        int arrivalTime = stoi(procesos[procesoEspera]["ArrivalTime"]);

                        if (arrivalTime <= timeCpu) {
                            rrEspera.pop();
                            rr.push(procesoEspera);
                            cout << "se agrego proceso " << procesoEspera << " a la cola en tiempo " << timeCpu << endl;
                        } else {
                            break;
                        }
                    }
                    rr.push(proceso);
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
                int turnAround = ct - arrivalTime;
                int waiting = turnAround - bt;
                // printf("turn arround %d of process %s \n", turnAround, proceso.c_str());
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
    cout << endl <<"_____EN EJECUCION FCFS_____" << endl << endl;
    while (!fcfs.empty()) 
    // cout << "_____EN EJECUCION FCFS_____" << endl;
    {
        if (rrEspera.empty())
        {   

            string proceso = fcfs.front();
            fcfs.pop();
            int bursTime = stoi(procesos[proceso]["BursTime"]);
            int arrival = stoi(procesos[proceso]["ArrivalTime"]);


            // Ejecutar el proceso FCFS
            timeCpu += bursTime;
            procesos[proceso]["ct"] = to_string(timeCpu);
            int ct = stoi(procesos[proceso]["ct"]);;
            int turnAround = ct - arrival;
            int waiting = turnAround - bursTime;
            procesos[proceso]["wt"] = to_string(waiting);
            printf("Proceso %s finalizó en FCFS en tiempo %d \n", proceso.c_str(), timeCpu);

            while (!fcfsEspera.empty()) {
                string procesoEspera = fcfsEspera.front();
                int arrivalTime = stoi(procesos[procesoEspera]["ArrivalTime"]);

                if (arrivalTime <= timeCpu) {
                    fcfsEspera.pop();
                    fcfs.push(procesoEspera);
                } else {
                    break;
                }
            }
        }else
        {
            string procesoF = fcfs.front();
            int bursTime = stoi(procesos[procesoF]["BursTime"]);
            timeCpu+=1;
            bursTime-=1;
            if (bursTime == 0){
                fcfs.pop();
                printf("Proceso %s finalizó en FCFS en tiempo %d\n", procesoF.c_str(), timeCpu);
            }else{
                printf("Proceso %s no finalizó en FCFS en tiempo %d\n", procesoF.c_str(), timeCpu);
                // cout << "burstTime " << bursTime << endl;
                procesos[procesoF]["BursTime"] = to_string(bursTime);
                while (!rrEspera.empty()) {
                    string procesoEspera = rrEspera.front();
                    int arrivalTime = stoi(procesos[procesoEspera]["ArrivalTime"]);

                    if (arrivalTime <= timeCpu) {
                        rrEspera.pop();
                        rr.push(procesoEspera);
                        cout << "se agrego proceso " << procesoEspera << " a la cola rr en tiempo " << timeCpu << endl;
                    } else {
                        break;
                    }
                }
                if (!rr.empty()){
                    roundRobin();
                }
                cout << endl <<"_____EN EJECUCION FCFS_____" << endl << endl;
            }
        }
    } 

    // firstComeFirstServed();

    cout<< endl << "-------Tiempo de cpu: " << timeCpu << "-------" <<endl;

}

void procesamientosDatos(const vector<vector<string> >& instrucciones) {
    string proceso;
    int arrivalTime;
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
        arrivalTime = stoi(instrucciones[j][1]);
        bursTime = stoi(instrucciones[j][2]);
        algoritmo = stoi(instrucciones[j][3]);

        // Guardar en cola correspondiente
        if (algoritmo == 1) {
            if (arrivalTime == 0) {
                rr.push(proceso);
            } else {
                rrEspera.push(proceso);
            }
        } else if (algoritmo == 2) {
            if (arrivalTime == 0) {
                fcfs.push(proceso);
            } else {
                fcfsEspera.push(proceso);
            }
        }

        // Guardar en el mapa
        procesos[proceso]["ArrivalTime"] = to_string(arrivalTime);
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
    imprimirTiemposDeEspera("wt");
    imprimirTiemposRespuesta();
    // imprimirTiemposDeEspera("wt");
    return 0;
}