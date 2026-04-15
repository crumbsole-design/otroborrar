
# Ejercicio 4

Escriba un programa en **C++** que realice transformaciones (escalados y rotaciones) sobre un array unidimensional de dos componentes `x` e `y` de tipo real, el cual es introducido por el usuario a través de la consola. Estas transformaciones se realizan de la siguiente forma:

## Tipos de transformaciones

- **Escalado**: Consiste en realizar el producto del factor de escala, que es un número real $a$, por el array $(x, y)$. El resultado es el array unidimensional de dos componentes $(a \cdot x, a \cdot y)$.

- **Rotación**: Consiste en realizar el producto de la matriz:

	$$
	\begin{bmatrix}
	\cos \alpha & -\sin \alpha \\
	\sin \alpha & \cos \alpha
	\end{bmatrix}
	$$

	por el array $(x, y)$. El ángulo de rotación $\alpha$ pasado como argumento a las funciones trigonométricas debe expresarse en **radianes**, mientras que el ángulo introducido por el usuario a través de la consola está expresado en **grados**. El programa debe realizar el cambio de unidades.

## Entrada de transformaciones

El usuario indica al programa las transformaciones a realizar mediante el siguiente procedimiento:

1. Especificar el factor de escala a continuación de la palabra `ESCALA`, separando ambos por uno o más espacios en blanco.
2. Especificar el ángulo de rotación a continuación de la palabra `ROT_DEG`, separando ambos por uno o más espacios en blanco.
3. Es posible describir varias transformaciones sucesivas, escribiéndolas una a continuación de otra, separadas por uno o más espacios en blanco.
4. El final de la secuencia de transformaciones se indica mediante el carácter punto y coma (`;`).

**Ejemplo de entrada:**

```
ESCALA 1.5 ROT_DEG 30.5 ROT_DEG -10 ESCALA -0.5;
```
o, indistintamente (incluyendo más espacios en blanco):
```
ESCALA 1.5   ROT_DEG 30.5   ROT_DEG -10   ESCALA -0.5 ;
```

Esto indica que se debe realizar sobre el array $(x, y)$:

1. Escala con factor 1.5
2. Rotación de 30.5°
3. Rotación de -10°
4. Escala con factor -0.5

## Requisitos del programa en C++

1. Mostrar un mensaje en la consola indicando al usuario que introduzca los dos componentes, `x` e `y`, del array unidimensional que desea transformar. Leer los valores y almacenarlos en un array unidimensional de dos componentes llamado `v`.
2. Mostrar un mensaje en la consola indicando al usuario que introduzca el texto que describe las transformaciones a realizar sobre el array `v`.
3. Comprobar si la línea de texto leída describe correctamente una o varias transformaciones. En caso afirmativo, realizar las transformaciones sobre el array `v` y mostrar en la consola el resultado obtenido al realizar cada una de las sucesivas transformaciones, escribiendo los componentes del array en formato fijo con **tres dígitos decimales**. En caso negativo, escribir un mensaje de error en la consola.
4. Terminar.

## Ejemplo de salida

Como resultado de las transformaciones:

```
ESCALA 1.5 ROT_DEG 30.5 ROT_DEG -10 ESCALA -0.5;
```

Sobre el vector $(1, 2)$, el programa debe escribir en la consola:

```
v = (1.000, 2.000)
Escala a = 1.500, v = (1.500, 3.000)
Rotacion alfa = 0.532 rad, v = (-0.230, 3.346)
Rotacion alfa = -0.175 rad, v = (0.354, 3.335)
Escala a = -0.500, v = (-0.177, -1.668)
```

## Casos de prueba

Muestre en la memoria capturas de pantalla del resultado obtenido al ejecutar su programa para cada uno de los siguientes casos de prueba:

### Caso de Prueba 1

- **Vector:** $(4, -3)$
- **Transformaciones:**
	```
	ESCALA 2.5 ESCALA -2 ;
	```

### Caso de Prueba 2

- **Vector:** $(4, 2.5)$
- **Transformaciones:**
	```
	ROT_DEG -90 ;
	```

### Caso de Prueba 3

- **Vector:** $(1, 2)$
- **Transformaciones:**
	```
	ESCALA 1.5 ROT_DEG 30.5 ROT_DEG -10 ESCALA -0.5;
	```

### Caso de Prueba 4

- **Vector:** $(-1, 1)$
- **Transformaciones:**
	```
	ROT_DEG 60 ROT_DEG -40 ESCALA 2 ROT_DEG -20 ESCALA 0.5;
	```
