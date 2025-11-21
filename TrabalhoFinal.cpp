#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <limits>

using namespace std;

struct Paciente {
    string senha;
    char prioridade;
    int horaChegada;
    int minutoChegada;
    int tempoEspera;
};

void limparBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool leituraValida() {
    if (cin.fail()) {
        cout << " " << endl;
        cout << "Erro: Entrada inválida. Tente novamente." << endl;
        cout << " " << endl;
        limparBuffer();
        return false;
    }
    return true;
}

void adicionarPaciente(vector<queue<Paciente>>& filas, const string& senha, char prioridade, int hora, int minuto) {
    Paciente novoPaciente;
    novoPaciente.senha = senha;
    novoPaciente.prioridade = prioridade;
    novoPaciente.horaChegada = hora;
    novoPaciente.minutoChegada = minuto;
    novoPaciente.tempoEspera = 0;
    
    int indice;
    switch(prioridade) {
        case 'V':
            indice = 0;
            break;
        case 'A':
            indice = 1;
            break; 
        case 'D':
            indice = 2;
            break; 
        case 'B':
            indice = 3;
            break; 
        default: return; 
    }
    
    filas[indice].push(novoPaciente);
}

bool atenderPaciente(vector<queue<Paciente>>& filas, int horaAtual, int minutoAtual, int& totalAtendidos, vector<int>& atendidosPorPrioridade, int& tempoMaximoEspera) {
    for(int i = 0; i < 4; i++) {
        if(!filas[i].empty()) {
            Paciente paciente = filas[i].front();
            filas[i].pop();
            
            int tempoEspera =(horaAtual - paciente.horaChegada) * 60 + (minutoAtual - paciente.minutoChegada);
            if(tempoEspera > tempoMaximoEspera) {
                tempoMaximoEspera = tempoEspera;
            }
            
            totalAtendidos++;
            
            switch(i) {
                case 0:
                    atendidosPorPrioridade[0]++;
                    break;
                case 1:
                    atendidosPorPrioridade[1]++;
                    break;
                case 2:
                    atendidosPorPrioridade[2]++;
                    break;
                case 3:
                    atendidosPorPrioridade[3]++;
                    break;
            }
            
            return true;
        }
    }
    return false;
}

void exibirEstadoFilas(const vector<queue<Paciente>>& filas, int totalAtendidos) {
    cout << "V:" << filas[0].size() 
         << " A:" << filas[1].size() 
         << " D:" << filas[2].size() 
         << " B:" << filas[3].size() 
         << " | Atendidos:" << totalAtendidos << endl;
}

void exibirRelatorioFinal(int totalAtendidos, const vector<int>& atendidosPorPrioridade, int picoLotacao, int tempoMaximoEspera) {
    cout << " " << endl;
    cout << "--- RELATÓRIO FINAL ---" << endl;
    cout << " " << endl;
    cout << "Total atendidos: " << totalAtendidos << endl;
    cout << "Por prioridade: V=" << atendidosPorPrioridade[0] 
         << " A=" << atendidosPorPrioridade[1] 
         << " D=" << atendidosPorPrioridade[2] 
         << " B=" << atendidosPorPrioridade[3] << endl;
    cout << "Pico de lotação: " << picoLotacao << endl;
    cout << "Espera máxima: " << tempoMaximoEspera << " min" << endl;
    cout << " " << endl;
}

int calcularTotalPacientes(const vector<queue<Paciente>>& filas) {
    int total = 0;
    for(int i = 0; i < 4; i++) {
        total += filas[i].size();
    }
    return total;
}

void formatarTempo(int tempo) {
    if (tempo < 10) {
        cout << "0" << tempo;
    } else {
        cout << tempo;
    }
}

int main() {
    vector<queue<Paciente>> filas(4);
    int totalAtendidos = 0;
    vector<int> atendidosPorPrioridade(4, 0);
    int picoLotacao = 0;
    int tempoMaximoEspera = 0;
    
    string comando;
    
    cout << " " << endl;
    cout << "Fila de Triagem Hospitalar" << endl;
    cout << " " << endl;
    cout << "C <senha> <prior> <hh> <mm> | Chegada de um paciente." << endl;
    cout << "A <hh> <mm> | Atendimento (um paciente é chamado)." << endl;
    cout << "D | Exibe informações sobre o estado atual das filas." << endl;
    cout << "Q | Encerra a simulação." << endl;
    cout << " " << endl;
    
    while(true) {
        cout << "> ";
        cin >> comando;
        
        if (!leituraValida()) {
            continue;
        }
        
        if(comando == "C") {
            string senha;
            char prioridade;
            int hora, minuto;
            
            cin >> senha >> prioridade >> hora >> minuto;
            
            if (!leituraValida()) {
                continue;
            }
            
            if(prioridade == 'V' || prioridade == 'A' || prioridade == 'D' || prioridade == 'B') {
                adicionarPaciente(filas, senha, prioridade, hora, minuto);

                int totalAtual = calcularTotalPacientes(filas);
                if(totalAtual > picoLotacao) {
                    picoLotacao = totalAtual;
                }
            } else {
                cout << " " << endl;
                cout << "Prioridade inválida. Use V, A, D ou B." << endl;
                cout << " " << endl;
                limparBuffer();
            }
        }
        else if(comando == "A") {
            int hora, minuto;
            cin >> hora >> minuto;
            
            if (!leituraValida()) {
                continue;
            }
            
            if(calcularTotalPacientes(filas) == 0) {
                formatarTempo(hora);
                cout << " ";
                formatarTempo(minuto);
                cout << " " << endl;
                cout << " Sem pacientes aguardando atendimento" << endl;
                cout << " " << endl;
            } else {
                atenderPaciente(filas, hora, minuto, totalAtendidos, atendidosPorPrioridade, tempoMaximoEspera);
            }
        }
        else if(comando == "D") {
            exibirEstadoFilas(filas, totalAtendidos);
        }
        else if(comando == "Q") {
            exibirRelatorioFinal(totalAtendidos, atendidosPorPrioridade, picoLotacao, tempoMaximoEspera);
            break;
        }
        else {
            cout << " " << endl;
            cout << "Comando inválido. Use C, A, D ou Q." << endl;
            cout << " " << endl;
            limparBuffer();
        }
    }
    
    return 0;
}