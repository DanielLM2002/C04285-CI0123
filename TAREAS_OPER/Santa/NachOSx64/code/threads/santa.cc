#include "santa.h"

Santa::Santa() {
    dp = new Lock("dp");
    duendes = new Condition("duendes");
    duendesEnTaller = 0;
    for (int i = 0; i < 4; i++) {
        estadoDuende[i] = Durmiendo;
        taller[i] = -1;
    }
    for (int i = 0; i < 2; i++) {
        estadoTaller[i] = Disponible;
    }
}
Santa::~Santa() {
    delete dp;
    delete duendes;
}

int Santa::inciarTrabajo(int duende) {
    dp->Acquire();
    estadoDuende[duende] = Esperando;
    sinAsignar.push_back(duende);
    if (sinAsignar.size() == 4) {
        casoGrunon();
    } else if (sinAsignar.size() == 3) {
        casoTimido();
    } else if (sinAsignar.size() == 2) {
        casoJerry();
    } else if (sinAsignar.size() == 1) {
        casoTom();
    }
    dp->Release();
    return 0;
}
void Santa::terminarTrabajo(int duende, int taller) {
    dp->Acquire();
    estadoDuende[duende] = Durmiendo;
    estadoTaller[taller] = Disponible;
    tallerL.push_back(duende);
    tallerH.push_back(taller);
    duendesEnTaller--;
    if (duendesEnTaller == 0) {
        duendes->Broadcast(dp);
    }
    dp->Release();
}

void Santa::print() {
    printf("Estado de los duendes: ");
    for (int i = 0; i < 4; i++) {
        if (estadoDuende[i] == Durmiendo) {
            printf("D ");
        } else if (estadoDuende[i] == Esperando) {
            printf("E ");
        } else if (estadoDuende[i] == Trabajando) {
            printf("T ");
        }
    }
    printf("\n");
    printf("Estado de los talleres: ");
    for (int i = 0; i < 2; i++) {
        if (estadoTaller[i] == Disponible) {
            printf("D ");
        } else if (estadoTaller[i] == Ocupado) {
            printf("O ");
        }
    }
    printf("\n");
    printf("Duendes en taller: ");
    for (int i = 0; i < 4; i++) {
        if (taller[i] != -1) {
            printf("%d ", taller[i]);
        }
    }
    printf("\n");
    printf("Duendes en taller L: ");
    for (std::list<char>::iterator it = tallerL.begin(); it != tallerL.end(); ++it) {
        printf("%d ", *it);
    }
    printf("\n");
    printf("Duendes en taller H: ");
    for (std::list<char>::iterator it = tallerH.begin(); it != tallerH.end(); ++it) {
        printf("%d ", *it);
    }
    printf("\n");
    printf("Duendes sin asignar: ");
    for (std::list<char>::iterator it = sinAsignar.begin(); it != sinAsignar.end(); ++it) {
        printf("%d ", *it);
    }
    printf("\n");
}

void Santa::casoGrunon() {
    tallerL.push_back(sinAsignar.front());
    sinAsignar.pop_front();
    tallerH.push_back(sinAsignar.front());
    sinAsignar.pop_front();
    tallerL.push_back(sinAsignar.front());
    sinAsignar.pop_front();
    tallerH.push_back(sinAsignar.front());
    sinAsignar.pop_front();
    for (int i = 0; i < 4; i++) {
        if (estadoDuende[i] == Esperando) {
            estadoDuende[i] = Trabajando;
            duendes->Signal(dp);
        }
    }
}

void Santa::casoJerry() {
    if (estadoTaller[0] == Disponible) {
        tallerL.push_back(sinAsignar.front());
        sinAsignar.pop_front();
        estadoTaller[0] = Ocupado;
    } else if (estadoTaller[1] == Disponible) {
        tallerH.push_back(sinAsignar.front());
        sinAsignar.pop_front();
        estadoTaller[1] = Ocupado;
    }
}

void Santa::casoTimido() {
    if (estadoTaller[0] == Disponible) {
        tallerL.push_back(sinAsignar.front());
        sinAsignar.pop_front();
        estadoTaller[0] = Ocupado;
    } else if (estadoTaller[1] == Disponible) {
        tallerH.push_back(sinAsignar.front());
        sinAsignar.pop_front();
        estadoTaller[1] = Ocupado;
    }
}

void Santa::casoTom() {
    if (estadoTaller[0] == Disponible) {
        tallerL.push_back(sinAsignar.front());
        sinAsignar.pop_front();
        estadoTaller[0] = Ocupado;
    } else if (estadoTaller[1] == Disponible) {
        tallerH.push_back(sinAsignar.front());
        sinAsignar.pop_front();
        estadoTaller[1] = Ocupado;
    }
}

bool Santa::tallerLleno(int taller) {
    if (taller == 0) {
        if (tallerL.size() == 2) {
            return true;
        }
    } else if (taller == 1) {
        if (tallerH.size() == 2) {
            return true;
        }
    }
    return false;
}