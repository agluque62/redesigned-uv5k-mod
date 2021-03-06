// DspAudioQualityTesting.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include "pch.h"
#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	__declspec(dllimport) int DAQOpenInstance();
	__declspec(dllimport) void DAQSetData(int instance, int count, float *pfData);
	__declspec(dllimport) int DAQGetQuality(int instance);
	__declspec(dllimport) void DAQCloseInstance(int instance);
#ifdef __cplusplus
}
#endif // __cplusplus

int main()
{
	float audio[64];
	std::cout << "Inicio ...";

	int instance = DAQOpenInstance();
	int quality = -1;

	if (instance >= 0) {
		for (int i = 0; i < 1000; i++)
		{
			DAQSetData(instance, 64, audio);
		}
		quality = DAQGetQuality(instance);
	}

	DAQCloseInstance(instance);

	std::cout << "Fin ...";
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
