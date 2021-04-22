struct filtro {
    int *nivel;
    int *victimas;
    int n;
};

typedef struct filtro filtro_t;
/* Crea un filtro para _n_ hilos */
filtro_t filtro_init(unsigned int n);

/* El hilo _id_ está intentando tomar el lock */
void filtro_lock(filtro_t filtro, int id);

/* El hilo _id_ libera el lock */
void filtro_unlock(filtro_t filtro, int id);

/* Destruye un filtro */
void filtro_destroy(filtro_t filtro);