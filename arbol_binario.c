#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

struct Nodo {
    int valor;
    struct Nodo* izquierda;
    struct Nodo* derecha;
};

struct Nodo* crearNodo(int valor) {
    struct Nodo* nuevoNodo = (struct Nodo*)malloc(sizeof(struct Nodo));
    nuevoNodo->valor = valor;
    nuevoNodo->izquierda = nuevoNodo->derecha = NULL;
    return nuevoNodo;
}

struct Nodo* insertar(struct Nodo* raiz, int nuevoValor, int valorExistente, char direccion) {
    if (raiz == NULL) {
        printf("Árbol vacío. Insertando el primer nodo con valor %d.\n", nuevoValor);
        return crearNodo(nuevoValor);
    }

    if (valorExistente == raiz->valor) {
        if (tolower(direccion) == 'i' && raiz->izquierda == NULL) {
            raiz->izquierda = crearNodo(nuevoValor);
            printf("Insertado %d a la izquierda de %d.\n", nuevoValor, valorExistente);
        } else if (tolower(direccion) == 'd' && raiz->derecha == NULL) {
            raiz->derecha = crearNodo(nuevoValor);
            printf("Insertado %d a la derecha de %d.\n", nuevoValor, valorExistente);
        } else {
            printf("Dirección no válida o nodo ya ocupado.\n");
        }
    } else if (valorExistente < raiz->valor) {
        raiz->izquierda = insertar(raiz->izquierda, nuevoValor, valorExistente, direccion);
    } else {
        raiz->derecha = insertar(raiz->derecha, nuevoValor, valorExistente, direccion);
    }

    return raiz;
}

void preorden(struct Nodo* raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->valor);
        preorden(raiz->izquierda);
        preorden(raiz->derecha);
    }
}

void inorden(struct Nodo* raiz) {
    if (raiz != NULL) {
        inorden(raiz->izquierda);
        printf("%d ", raiz->valor);
        inorden(raiz->derecha);
    }
}

void postorden(struct Nodo* raiz) {
    if (raiz != NULL) {
        postorden(raiz->izquierda);
        postorden(raiz->derecha);
        printf("%d ", raiz->valor);
    }
}

bool validarSubarbol(struct Nodo* raiz, int* min, int* max) {
    if (raiz == NULL) {
        return true;  // Árbol vacío es un ABB válido
    }

    if ((min != NULL && raiz->valor <= *min) || (max != NULL && raiz->valor >= *max)) {
        return false;  // Valor fuera de rango
    }

    // Verificar los subárboles izquierdo y derecho
    return validarSubarbol(raiz->izquierda, min, &raiz->valor) && validarSubarbol(raiz->derecha, &raiz->valor, max);
}


bool esArbolValido(struct Nodo* raiz) {
    return validarSubarbol(raiz, NULL, NULL);
}

struct Nodo* encontrarNodo(struct Nodo* raiz, int valor) {
    if (raiz == NULL || raiz->valor == valor) {
        return raiz;
    }

    if (valor < raiz->valor) {
        return encontrarNodo(raiz->izquierda, valor);
    } else {
        return encontrarNodo(raiz->derecha, valor);
    }
}

struct Nodo* encontrarMinimo(struct Nodo* raiz) {
    while (raiz->izquierda != NULL) {
        raiz = raiz->izquierda;
    }
    return raiz;
}

struct Nodo* borrarNodo(struct Nodo* raiz, int valor) {
    if (raiz == NULL) {
        return raiz;
    }

    if (valor < raiz->valor) {
        raiz->izquierda = borrarNodo(raiz->izquierda, valor);
    } else if (valor > raiz->valor) {
        raiz->derecha = borrarNodo(raiz->derecha, valor);
    } else {
        // Nodo con un hijo o sin hijo
        if (raiz->izquierda == NULL) {
            struct Nodo* temp = raiz->derecha;
            free(raiz);
            return temp;
        } else if (raiz->derecha == NULL) {
            struct Nodo* temp = raiz->izquierda;
            free(raiz);
            return temp;
        }

        // Nodo con dos hijos: obtener el sucesor inorden (el más pequeño en el subárbol derecho)
        struct Nodo* temp = encontrarMinimo(raiz->derecha);

        // Copiar el valor del sucesor inorden al nodo actual
        raiz->valor = temp->valor;

        // Borrar el sucesor inorden
        raiz->derecha = borrarNodo(raiz->derecha, temp->valor);
    }

    return raiz;
}

int main() {
    struct Nodo* raiz = NULL;
    int opcion, nuevoValor, valorExistente;
    char direccion;

    do {
        printf("\nMenu de Opciones:\n");
        printf("1. Insertar nodo\n");
        printf("2. Mostrar en preorden\n");
        printf("3. Mostrar en inorden\n");
        printf("4. Mostrar en postorden\n");
        printf("5. Encontrar nodo\n");
        printf("6. Borrar nodo\n");
        printf("7. Validar arbol\n");
        printf("8. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                if (raiz == NULL) {
                    printf("Ingrese el valor del primer nodo: ");
                    scanf("%d", &nuevoValor);
                    raiz = crearNodo(nuevoValor);
                    printf("Árbol creado con el primer nodo de valor %d.\n", nuevoValor);
                } else {
                    printf("Ingrese la descripción del nuevo nodo (ejemplo: 9-I-10): "); // '9' es el valor del nuevo nodo, 'I' ó 'D' la direccíon con respecto a '10' que sera el nodo padre del nuevo nodo
                    scanf("%d-%c-%d", &nuevoValor, &direccion, &valorExistente);        // I es de Izquierda y D de derecha
                    direccion = (char)toupper(direccion);
                    raiz = insertar(raiz, nuevoValor, valorExistente, direccion);
                }
                break;
            case 2:
                printf("Árbol en preorden: ");
                preorden(raiz);
                printf("\n");
                break;
            case 3:
                printf("Árbol en inorden: ");
                inorden(raiz);
                printf("\n");
                break;
            case 4:
                printf("Árbol en postorden: ");
                postorden(raiz);
                printf("\n");
                break;
            case 5:
                printf("Ingrese el valor del nodo a buscar: ");
                scanf("%d", &valorExistente);
                struct Nodo* nodo_encontrado = encontrarNodo(raiz, valorExistente);
                if (nodo_encontrado != NULL) {
                    printf("Nodo encontrado: ID=%p, Valor=%d, Hijo Izquierdo=%d, Hijo Derecho=%d\n", (void*)nodo_encontrado, nodo_encontrado->valor, (nodo_encontrado->izquierda != NULL) ? nodo_encontrado->izquierda->valor : -1, (nodo_encontrado->derecha != NULL) ? nodo_encontrado->derecha->valor : -1);
                } else {
                    printf("Nodo no encontrado.\n");
                }
                break;
            case 6:
                printf("Ingrese el valor del nodo a borrar: ");
                scanf("%d", &valorExistente);
                raiz = borrarNodo(raiz, valorExistente);
                printf("Árbol después de borrar el nodo con valor %d:\n", valorExistente);
                printf("Árbol en inorden: ");
                inorden(raiz);
                printf("\n");
                break;
            case 7:
                if (esArbolValido(raiz)) {
                    printf("El árbol es un árbol binario de búsqueda válido.\n");
                } else {
                    printf("El árbol NO es un árbol binario de búsqueda válido.\n");
                }
                break;
            case 8:
                printf("Saliendo del programa.\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    } while (opcion != 8);

    return 0;
}
