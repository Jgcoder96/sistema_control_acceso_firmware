#ifndef WIFI_MESH_SETUP_H
#define WIFI_MESH_SETUP_H

/**
 * @file wifi_mesh_setup.h
 * @brief Setup a nivel de hardware (LwIP) de la red Mesh.
 */

/**
 * @brief Inicializa el driver WiFi, pila TCP/IP, y arranca el Mesh (MDF).
 *
 * Se encarga de hacer el scan para buscar el router, definir los parámetros 
 * de red y crear el stack.
 *
 * @par Ejemplo de uso:
 * @code
 * wifi_mesh_setup(); // Llamar antes de wifi_mesh_init()
 * @endcode
 */
void wifi_mesh_setup(void);

#endif