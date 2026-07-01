/**
 * @file PROYECTO_FINAL_NoeliaArtés_MariaJoseAzcárate.cpp
 *
 * @brief 
 * Programa para el procesamiento de imágenes.
 *
 * @version 1.0
 * @author Noelia Artés 
 * @author María José Azcárate
 * @date 02-01-2024
 */

#include <iostream>
#include <time.h>
#include <fstream>

#include "CImg-3.3.3_pre11242318/CImg.h"

using namespace cimg_library;
using namespace std;

const unsigned int N_ROWS = 512;
const unsigned int N_COLS = 512;
const unsigned int TAM = 9;

typedef unsigned char Matrix[N_ROWS][N_COLS];
typedef unsigned char MatrixRGB[N_ROWS][N_COLS][3];
typedef unsigned char Vector [TAM];


void ReadImage(string, Matrix);
void ReadRGBImage(string, MatrixRGB);
void DisplayImage(const Matrix);
void DisplayRGBImage(const MatrixRGB);
void WriteImage(string, const Matrix);
void ThresholdImage(const Matrix, Matrix, unsigned int);
void NegativeImage(const Matrix, Matrix);
void AddNoiseImage(const Matrix, Matrix, unsigned int);
void MedianFilterImage(const Matrix, Matrix);
void HistogramImage(const Matrix, ofstream&);
void SeparateChannelsRGB(const MatrixRGB, Matrix, Matrix, Matrix);

unsigned int Menu();

void InsertionSort(Vector, unsigned int);
unsigned char ObteinMedian(Vector);

int main()
{
    static Matrix m, mTh, mNeg, mNoise, mMedian, mR, mG, mB;
    static MatrixRGB mColor;
    unsigned int th, pNoise, sel, opt;
    string fileName;
    ofstream salida;
	string in;
    do
    {
        sel = Menu();
        switch (sel)
        {
        case 1:
            cout << "Input file name? " << endl;
            fileName = "Images/";
            cin >> in;
            fileName += in;
            ReadImage(fileName, m);
            cout << "\n Close the image window to continue..." << endl;
            DisplayImage(m);
            break;
        case 2:
            cout << "Input file name? " << endl;
            fileName = "Images/";
            cin >> in;
            fileName += in;
            ReadRGBImage(fileName, mColor);
            cout << "\n Close the image window to continue..." << endl;
            DisplayRGBImage(mColor);
            break;
        case 3:
        	cout << "Input file name? " << endl;
            fileName = "Images/";
            cin >> in;
            fileName += in;
            ReadImage(fileName, m);
            cout << "Input the threshold level? " << endl;
            cin >> th;
            ThresholdImage(m, mTh, th);
            cout << "\n Close the image window to continue..." << endl;
            DisplayImage(mTh);
            break;
        case 4:
        	cout << "Input file name? " << endl;
            fileName = "Images/";
            cin >> in;
            fileName += in;
            ReadImage(fileName, m);
            NegativeImage(m, mNeg);
            cout << "\n Close the image window to continue..." << endl;
            DisplayImage(mNeg);
            break;
        case 5:
        	cout << "Input file name? " << endl;
            fileName = "Images/";
            cin >> in;
            fileName += in;
            ReadImage(fileName, m);
            cout << "Input the percentage of noise salt-and-paper? " << endl;
            cin >> pNoise;
            AddNoiseImage(m, mNoise, pNoise);
            cout << "\n Close the image window to continue..." << endl;
            DisplayImage(mNoise);
            break;
        case 6:
        	cout << "Input file name? " << endl;
            fileName = "Images/";
            cin >> in;
            fileName += in;
            ReadImage(fileName, m);
            MedianFilterImage(m, mMedian);
            cout << "\n Close the image window to continue..." << endl;
            DisplayImage(mMedian);
            break;

        case 7: 
            cout << "Input file name? " << endl;
            cin >> fileName;
            ReadImage(fileName, m);
            salida.open(fileName + ".txt");
            if (salida.is_open())
			{
				HistogramImage(m, salida);
			} else
			{
				cout<< "Error al abrir el fichero";
			}
            salida.close();
            break;
        case 8:
            cout << "1) Threshold; 2) Negative; 3) Noise; 4) Median Filter? " << endl;
            cin >> opt;
            cout << "Input file name? " << endl;
            cin >> fileName;
            switch (opt)
            {
            case 1:
                WriteImage(fileName, mTh);
                break;
            case 2:
                WriteImage(fileName, mNeg);
                break;
            case 3:
                WriteImage(fileName, mNoise);
                break;
            case 4:
                WriteImage(fileName, mMedian);
                break;
            }
            break;

        case 9:
            SeparateChannelsRGB(mColor, mR, mG, mB);
            break;
        case 10:
            cout << "Input file name? " << endl;
            cin >> fileName;
            WriteImage("R-" + fileName, mR);
            WriteImage("G-" + fileName, mG);
            WriteImage("B-" + fileName, mB);
            break;
        case 11:
            cout << "Fin";
        }
    } while (sel != 11);

    return 0;
}

unsigned int Menu()
{
    unsigned int sel;

    cout << "1. Read grey-level image." << endl;
    cout << "2. Read color image." << endl;
    cout << "3. Threshold grey-level image." << endl;
    cout << "4. Negative grey-level image." << endl;
    cout << "5. Add grey-level noise." << endl;
    cout << "6. Median grey-level filter." << endl;
    cout << "7. Histogram grey-level image." << endl;
    cout << "8. Save grey-level image." << endl;
    cout << "9. Separate Color Channels." << endl;
    cout << "10.Save color channels." << endl;
    cout << "11.Exit." << endl;
    cout << "Select an option: ";
    cin >> sel;

    return sel;
}
/**
 * @brief Aplica un umbral a una imagen en escala de grises.
 * @param[in] m Matriz que representa la imagen en escala de grises.
 * @param[out] mres Matriz para almacenar la imagen umbralizada.
 * @param[in] t Valor de umbral.
 */
void ThresholdImage(const Matrix m, Matrix mres, unsigned int t)
{
    for (unsigned int i = 0; i < N_ROWS; i++)
    {
        for (unsigned int j = 0; j < N_COLS; j++)
        {
            if (m[i][j] < t)
                mres[i][j] = 0;
            else
                mres[i][j] = 255;
        }
    }
}
/**
 * @brief Obtiene la imagen negativa de una imagen en escala de grises.
 * @param[in] m Matriz que representa la imagen en escala de grises.
 * @param[out] mres Matriz para almacenar la imagen negativa.
 */
void NegativeImage(const Matrix m, Matrix mres)
{
    for (unsigned int i = 0; i < N_ROWS; i++)
    {
        for (unsigned int j = 0; j < N_COLS; j++)
        {
            mres[i][j] = 255 - m[i][j];
        }
    }
}
/**
 * @brief Agrega ruido a una imagen en escala de grises.
 * @param[in] m Matriz que representa la imagen en escala de grises.
 * @param[out] mres Matriz para almacenar la imagen con ruido.
 * @param[in] p Porcentaje de píxeles con ruido (valor entre 0 y 100).
 */
void AddNoiseImage(const Matrix m, Matrix mres, unsigned int p)
{
    unsigned int pixelesTotales = N_ROWS * N_COLS;
    unsigned int pixelesRuido = (pixelesTotales * p) / 100;

    unsigned int sal = pixelesRuido / 2;
    unsigned int pimienta = pixelesRuido - sal;

    srand(time(NULL));

    for (unsigned int i = 0; i < N_ROWS; i++)
        for (unsigned int j = 0; j < N_COLS; j++)
            mres[i][j] = m[i][j];

    for (unsigned int i = 0; i < sal; i++)
    {
        int fila = rand() % N_ROWS;
        int columna = rand() % N_COLS;
        mres[fila][columna] = 0;
    }

    for (unsigned int i = 0; i < pimienta; i++)
    {
        int fila = rand() % N_ROWS;
        int columna = rand() % N_COLS;
        mres[fila][columna] = 255;
    }
}
/**
 * @brief Aplica un filtro de mediana a una imagen en escala de grises.
 * @param[in] m Matriz que representa la imagen en escala de grises.
 * @param[out] mres Matriz para almacenar la imagen filtrada.
 */
void MedianFilterImage(const Matrix m, Matrix mres)
{
    Vector v;

    for (unsigned int i = 1; i < N_ROWS - 1; i++)
    {
        for (unsigned int j = 1; j < N_COLS - 1; j++)
        {
            v[0] = m[i - 1][j - 1];
            v[1] = m[i - 1][j];
            v[2] = m[i - 1][j + 1];
            v[3] = m[i][j - 1];
            v[4] = m[i][j];
            v[5] = m[i][j + 1];
            v[6] = m[i + 1][j - 1];
            v[7] = m[i + 1][j];
            v[8] = m[i + 1][j + 1];
            InsertionSort(v, 9);
            mres[i][j] = ObteinMedian(v);
        }
    }
}
// funciones auxiliares

/**
 * @brief Implementa el algoritmo de ordenamiento de inserción.
 * @param[out] v Vector a ordenar.
 * @param[in] n Tamaño del vector.
 */
void InsertionSort(Vector v, unsigned int n)
{
    unsigned int i;
    int j;
    unsigned char aux;

    for (i = 1; i < n; i++)
    {
        aux = v[i];
        j = i - 1;
        while (j >= 0 && v[j] > aux)
        {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = aux;

    }
}
/**
 * @brief Obtiene el valor medio de un vector ordenado.
 * @param[out] v Vector ordenado.
 * @return Valor medio del vector.
 */
unsigned char ObteinMedian(Vector v)
{
    return v[4];
}
/**
 * @brief Calcula el histograma de una imagen en escala de grises y lo guarda en un archivo.
 * @param[in] m Matriz que representa la imagen en escala de grises.
 * @param[out] f Flujo de salida para escribir el histograma.
 */
void HistogramImage(const Matrix m, ofstream& f)
{
    int histograma[256] = { 0 };

    for (unsigned int i = 0; i < N_ROWS; i++)
        for (unsigned int j = 0; j < N_COLS; j++)
            histograma[m[i][j]]++;

    for (int i = 0; i < 256; i++)
        f << i << " " << histograma[i] << endl;
}
/**
 * @brief Separa los canales de una imagen en formato RGB.
 * @param[in] m Matriz que representa la imagen en formato RGB.
 * @param[out] r Matriz para almacenar el canal rojo.
 * @param[out] g Matriz para almacenar el canal verde.
 * @param[out] b Matriz para almacenar el canal azul.
 */
void SeparateChannelsRGB(const MatrixRGB m, Matrix r, Matrix g, Matrix b)
{
    for (unsigned int i = 0; i < N_ROWS; i++)
        for (unsigned int j = 0; j < N_COLS; j++)
        {
            r[i][j] = m[i][j][0];
            g[i][j] = m[i][j][1];
            b[i][j] = m[i][j][2];
        }

    DisplayImage(r);
    DisplayImage(g);
    DisplayImage(b);
}

/************************************/
/* FUNCIONES AUXILIARES             */
/************************************/

/**
 * @brief Lee una imagen en escala de grises desde un archivo.
 * @param[in] nameFile Nombre del archivo de la imagen.
 * @param[out] m Matriz para almacenar la imagen.
 */
void ReadImage(string nameFile, Matrix m)
{
    CImg<unsigned char> image(nameFile.c_str());

    for (unsigned int i = 0; i < N_ROWS; i++)
        for (unsigned int j = 0; j < N_COLS; j++)
            m[i][j] = image(i, j);
}
/**
 * @brief Lee una imagen en formato RGB desde un archivo.
 * @param[in] nameFile Nombre del archivo de la imagen.
 * @param[out] m Matriz para almacenar la imagen en formato RGB.
 */
void ReadRGBImage(string nameFile, MatrixRGB m)
{
    CImg<unsigned char> image(nameFile.c_str());

    for (unsigned int i = 0; i < N_ROWS; i++)
        for (unsigned int j = 0; j < N_COLS; j++)
        {
            m[i][j][0] = image(i, j, 0, 0);
            m[i][j][1] = image(i, j, 0, 1);
            m[i][j][2] = image(i, j, 0, 2);
        }
}
/**
 * @brief Muestra una imagen en escala de grises.
 * @param[in] m Matriz que representa la imagen en escala de grises.
 */
void DisplayImage(const Matrix m)
{
    CImg<unsigned char> image(N_ROWS, N_COLS);

    for (unsigned int i = 0; i < N_ROWS; i++)
        for (unsigned int j = 0; j < N_COLS; j++)
            image(i, j) = m[i][j];

    CImgDisplay disp(image, "Image");
    while (!disp.is_closed())
        disp.wait();
}
/**
 * @brief Muestra una imagen en formato RGB.
 * @param[in] m Matriz que representa la imagen en formato RGB.
 */
void DisplayRGBImage(const MatrixRGB m)
{
    CImg<unsigned char> image(N_ROWS, N_COLS, 1, 3, 0);

    for (unsigned int i = 0; i < N_ROWS; i++)
        for (unsigned int j = 0; j < N_COLS; j++)
        {
            image(i, j, 0, 0) = m[i][j][0];
            image(i, j, 0, 1) = m[i][j][1];
            image(i, j, 0, 2) = m[i][j][2];
        }

    CImgDisplay disp(image, "Image");
    while (!disp.is_closed())
        disp.wait();
}
/**
 * @brief Escribe una imagen en escala de grises en un archivo.
 * @param[in] name Nombre del archivo de salida.
 * @param[in] m Matriz que representa la imagen en escala de grises.
 */
void WriteImage(string name, const Matrix m)
{
    CImg<unsigned char> image(N_ROWS, N_COLS);

    for (unsigned int i = 0; i < N_ROWS; i++)
        for (unsigned int j = 0; j < N_COLS; j++)
            image(i, j) = m[i][j];

    image.save(name.c_str());
}
