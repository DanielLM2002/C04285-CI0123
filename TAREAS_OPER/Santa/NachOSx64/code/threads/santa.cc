#include "santa.h"
/*
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
} */

Santa::Santa() {
    dp = new Lock("dp");
    for (int i = 0; i < 4; i++) {
        duendes[i] = new Condition("duende");
        estadoDuende[i] = Durmiendo;
    }
}

Santa::~Santa() {
    delete dp;
    for (int i = 0; i < 4; i++) {
        delete duendes[i];        
    }
}

int Santa::inciarTrabajo(int duende) {
    dp->Acquire();
    estadoDuende[duende] = Esperando;
    std::cout<< "El duende " << duende << " esta esperando" << std::endl;
    if (sinAsignar.size() == 4) {
        casoGrunon();
    } else if (sinAsignar.size() == 3) {
        casoJerry();
    } else if (sinAsignar.size() == 2) {
        casoTimido();
    } else if (sinAsignar.size() == 1) {
        casoTom();
    }
    if (estadoDuende[duende] == Esperando) {
        duendes[duende]->Wait(dp);
        std::cout<< "Esperando por espacio de taller" << duende + 1;
    }
    dp->Release();
    return duende;
}

void Santa::terminarTrabajo(int duende, int taller) {
    dp->Acquire();
    if (taller == 0) {
        tallerL.pop_front();
    } else if (taller == 1) {
        tallerH.pop_front();
    }
    if (tallerLleno(taller)) {
        estadoTaller[taller] = Disponible;
    }
    if (sinAsignar.size() > 0) {
        if (taller == 0) {
            tallerL.push_back(sinAsignar.front());
            sinAsignar.pop_front();
        } else if (taller == 1) {
            tallerH.push_back(sinAsignar.front());
            sinAsignar.pop_front();
        }
    }
    estadoDuende[duende] = Durmiendo;
    dp->Release();
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

bool Santa::duendeTrabajando(int duende) {
    if (estadoDuende[duende] == Trabajando) {
        return true;
    }
    return false;
}

void Santa::casoGrunon() {
    if (this->tallerL.empty()) {
        tallerL.push_back(1);
        estadoDuende[0] = Trabajando;
        sinAsignar.remove(0);
        taller[0] = 0;
        duendes[0]->Signal(dp);
        std::cout<< "El duende " << 1 << " esta trabajando en el taller de baja tecnologia" << std::endl;
    } else if (this->tallerH.empty()) {
        tallerH.push_back(1);
        estadoDuende[0] = Trabajando;
        sinAsignar.remove(0);
        taller[0] = 1;
        duendes[0]->Signal(dp);
        std::cout<< "El duende " << 1 << " esta trabajando en el taller de alta tecnologia" << std::endl;
    }
}

void Santa::casoTom() {
    if (tallerL.size() <=1 && *tallerL.begin() != 1) {
        if (*tallerL.begin() != 3) {
            tallerL.push_back(2);
            estadoDuende[1] = Trabajando;
            sinAsignar.remove(1);
            taller[1] = 0;
            duendes[1]->Signal(dp);
            std::cout<< "El duende " << 2 << " esta trabajando en el taller de baja tecnologia" << std::endl;
        }
    } else if (tallerH.size() <=1 && *tallerH.begin() != 1) {
        if (*tallerH.begin() != 3) {
            tallerH.push_back(2);
            estadoDuende[1] = Trabajando;
            sinAsignar.remove(1);
            taller[1] = 1;
            duendes[1]->Signal(dp);
            std::cout<< "El duende " << 2 << " esta trabajando en el taller de alta tecnologia" << std::endl;
        }
    }
}

void Santa::casoJerry() {
    if (tallerL.size()<= 1 && *tallerL.begin()!= 1) {
        if (*tallerL.begin() != 2) {
            tallerL.push_back(3);
            estadoDuende[2] = Trabajando;
            sinAsignar.remove(2);
            taller[2] = 0;
            duendes[2]->Signal(dp);
            std::cout<< "El duende " << 3 << " esta trabajando en el taller de baja tecnologia" << std::endl;
        }
    } else if (tallerH.size() <= 1 && *tallerH.begin() != 1) {
        if (*tallerH.begin() != 2) {
            tallerH.push_back(3);
            estadoDuende[2] = Trabajando;
            sinAsignar.remove(2);
            taller[2] = 1;
            duendes[2]->Signal(dp);
            std::cout<< "El duende " << 3 << " esta trabajando en el taller de alta tecnologia" << std::endl;
        }
    }
}

void Santa::casoTimido() {
    if (tallerL.size() == 1 && *tallerL.begin() != 1) {
        tallerL.push_back(4);
        estadoDuende[3] = Trabajando;
        sinAsignar.remove(3);
        taller[3] = 0;
        duendes[3]->Signal(dp);
        std::cout<< "El duende " << 4 << " esta trabajando en el taller de baja tecnologia" << std::endl;
    } else if (tallerH.size() == 1 && *tallerH.begin() != 1) {
        tallerH.push_back(4);
        estadoDuende[3] = Trabajando;
        sinAsignar.remove(3);
        taller[3] = 1;
        duendes[3]->Signal(dp);
        std::cout<< "El duende " << 4 << " esta trabajando en el taller de alta tecnologia" << std::endl;
    }
}

void print() {
    std::cout<< "Taller de baja tecnologia: " << std::endl;
    for (std::list<int>::iterator it = tallerL.begin(); it != tallerL.end(); ++it) {
        std::cout<< *it << " ";
    }
    std::cout<< std::endl;
    std::cout<< "Taller de alta tecnologia: " << std::endl;
    for (std::list<int>::iterator it = tallerH.begin(); it != tallerH.end(); ++it) {
        std::cout<< *it << " ";
    }
    std::cout<< std::endl;
}

