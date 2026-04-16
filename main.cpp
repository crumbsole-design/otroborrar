#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
#include "transformations.h"

using namespace std;
using namespace tf;

/**
 * Ejercicio 4 - Transformaciones Geométricas
 * Lenguajes de Programación - UNED 2026
 */

int main() {
    double x, y;
    
    // "1.  Mostrar un mensaje en la consola indicando al usuario que introduzca los dos componentes, x e y" 
    cout << "Introduzca los dos componentes, x e y, del array unidimensional que desea transformar: ";
    if (!(cin >> x >> y)) {
        cerr << "Error: entrada de vector invalida." << endl;
        return 1;
    }

    // "del array unidimensional que desea transformar. Leer los
    // valores introducidos por el usuario y almacenarlos en un array unidimensional
    // de dos componentes llamado v"
    Vector2D v{x, y};
   

    // Limpiar el buffer de entrada para leer la línea de texto posterior
    cin.ignore(1000, '\n');

    // "2. Mostrar un mensaje en la consola indicando al usuario que introduzca por
    // consola el texto que describe las transformaciones a realizar sobre el array v."

    cout << "Introduzca por consola el texto que describe las transformaciones a realizar sobre el array v: ";
    
    cout << "v=" << formatVector(v) << endl;
    string linea;
    getline(cin, linea);

    // Procesar la secuencia de comandos , ya nos traemos el resultado como un vector de comandos separados
    vector<CommandToken> tokens = parseCommandSequence(linea);

    // "3. Comprobar si la l´ınea de texto le´ıda describe correctamente una o varias
    // transformaciones"
    if (tokens.empty() && linea.find(';') == string::npos) {
        cout << "Error: La linea de texto no describe correctamente las transformaciones." << endl;
        return 1;
    }

    //  "En caso afirmativo, realizar las transformaciones sobre el array v  "
    for (const auto& token : tokens) {
        string comando = token.key;
        
        // Normalizar a mayúsculas para la comparación aqui los comandos que no estan bien formados simplemente se saltan con return 1 al final del programa, no se procesan
        for (auto &c : comando) c = toupper(c);

        if (comando == "ESCALA") {
            if (!token.hasValue) {
                cout << "Error: El comando ESCALA requiere un valor numerico." << endl;
                return 1;
            }
            v = scaleVector(v, token.value);
            // "mostrar en la consola el resultado obtenido al realizar cada una
            // de las sucesivas transformaciones, escribiendo los componentes del array en
            // formato fijo con tres d´ıgitos decimales. En caso negativo, escribir un mensaje
            // de error en la consola
            cout << "Escala a=" << formatFixed3(token.value) 
                 << " v=" << formatVector(v) << endl;
        } 
        else if (comando == "ROT_DEG") {
            if (!token.hasValue) {
                cout << "Error: El comando ROT_DEG requiere un valor numerico." << endl;
                return 1;
            }
            // Calcular radianes para el mensaje de salida según el ejemplo
            const double PI = std::acos(-1.0);
            double radianes = token.value * (PI / 180.0);
            
            v = rotateVectorDeg(v, token.value);
            
            // Mostrar resultado: Rotacion alfa = X.XXX rad, v=(X.XXX, Y.YYY)
            cout << "Rotacion alfa = " << formatFixed3(radianes) 
                 << " rad, v=" << formatVector(v) << endl;
        } 
        else {
            // En caso negativo, escribir un mensaje de error en la consola
            cout << "Error: Transformacion '" << token.key << "' no reconocida." << endl;
            return 1;
        }
    }

    return 0; // 4. Terminar 
}