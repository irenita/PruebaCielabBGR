

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "Def_controles.h"

#ifndef _DEF_IOSTREAM_ 
#include <iostream>
using namespace std;
#endif

using namespace cv;


int main()
{
	char entrada[100] = "Flor.jpg";
	Mat imagen;										// new image - nueva imagen
	imagen = imread(entrada, IMREAD_COLOR);			// read the file - leer el archivo
	
	if (!imagen.data)								// Check for invalid input - chequea entrada invalida
	{
		cout << "No se pudo abrir o encontrar la imagen \n Could not open or find the image";
		ControlImprimir();
		//CargarimagenBGR();
		cin.get();
		return -1;
	}

	//**********************************************************************************//
	//**********************************************************************************//
	// ***** BGR ***** //
	Mat imagenBGR[3];						//	Matriz de 3 canales
	imagen.convertTo(imagen, CV_8U);		//	8U es de 0 a 255
	split(imagen, imagenBGR);				//	Separa la imagen original en 3 canales BGR (BlueGreenRed)
											//cout << imagenBGR[0];

											// ***** BGR to CIELab ***** //
	Mat imagenLab_temp;
	//imagen.convertTo(imagenLab_temp, CV_32F);
	cvtColor(imagen, imagenLab_temp, CV_BGR2Lab);	// Conversion de BGR a CIELab //cvtColor(imagenLab_temp, imagenLab_temp, CV_BGR2Lab);	
	Mat imagenLab[3];
	split(imagenLab_temp, imagenLab);				// Separa la imagen CIELab en 3 canales L a b
													//cout << imagenLab[1];

													//imagenLab[1] = Mat::zeros(imagen.rows, imagen.cols, CV_8UC1);
													//imagenLab[2] = Mat::zeros(imagen.rows, imagen.cols, CV_8UC1);
	Mat imagenLab_NEW(imagen.rows, imagen.cols, CV_8UC3); //CV_8UC3   // U porque va de 0 a 255
	merge(imagenLab, 3, imagenLab_NEW);
	//	cvtColor(imagenLab_NEW, imagenLab_temp, CV_Lab2BGR);
	//	split(imagenLab_temp, imagenLab);
	namedWindow("L_merge", CV_WINDOW_NORMAL);
	imshow("L_merge", imagenLab_NEW);

	namedWindow("L", CV_WINDOW_NORMAL);
	namedWindow("a", CV_WINDOW_NORMAL);
	namedWindow("b", CV_WINDOW_NORMAL);
	imshow("L", imagenLab[0]);
	imshow("a", imagenLab[1]);
	imshow("b", imagenLab[2]);

	//imwrite("blue.png", bgr[0]); //blue channel
	//imwrite("green.png", bgr[1]); //green channel
	//imwrite("red.png", bgr[2]); //red channel
	namedWindow("BLUE", CV_WINDOW_AUTOSIZE);
	namedWindow("GREEN", CV_WINDOW_NORMAL);
	namedWindow("RED", CV_WINDOW_NORMAL);
	imshow("BLUE", imagenBGR[0]);
	imshow("GREEN", imagenBGR[1]);
	imshow("RED", imagenBGR[2]);

	namedWindow("BGR", CV_WINDOW_NORMAL);			// create a window for display.CV_WINDOW_AUTOSIZE
	imshow("BGR", imagen);							// show our image inside it.
													//resizeWindow("BGR", 500 , 500);

													// **** VAMOS A PROBAR LA CONVERSION DE LAB A BGR
	Mat imagenLab2BGR;
	cvtColor(imagenLab_temp, imagenLab2BGR, CV_Lab2BGR);	// Conversion de CIELab a BGR  
	namedWindow("BGR desde Lab", CV_WINDOW_NORMAL);			// create a window for display
	imshow("BGR desde Lab", imagenLab2BGR);

	// ****************************************************************************************//
	// ****************************  FFT  en  BGR y en CLIEAB  ********************************//

	// ****************************   BGR   ********************************//

	//	Mat imagenTempo; //(Mat::zeros(imagen.rows, imagen.cols, CV_32F));
	Mat tempBlue, tempGreen, tempRed;
	tempBlue = imagenBGR[0];
	tempGreen = imagenBGR[1];
	tempRed = imagenBGR[2];
	tempBlue.convertTo(tempBlue, CV_32F);
	tempGreen.convertTo(tempGreen, CV_32F);
	tempRed.convertTo(tempRed, CV_32F);
	//	Mat planes[] = { temp, imagenTempo, CV_32F };
	//Mat planes[] = { Mat_<float>(imagen), Mat::zeros(imagen.size(), CV_32F) };
	Mat complexBlue, complexGreen, complexRed;    //Complex plane to contain the DFT coefficients {[0]-Real,[1]-Img}
												  //	merge(planes, 2, complexI);
	dft(tempBlue, complexBlue, DFT_COMPLEX_OUTPUT);  // Applying DFT dft(dftInput1, dftImage1, DFT_COMPLEX_OUTPUT); 
	dft(tempGreen, complexGreen, DFT_COMPLEX_OUTPUT);  // Applying DFT dft(dftInput1, dftImage1, DFT_COMPLEX_OUTPUT); 
	dft(tempRed, complexRed, DFT_COMPLEX_OUTPUT);  // Applying DFT dft(dftInput1, dftImage1, DFT_COMPLEX_OUTPUT); 


												   // Reconstructing original image from the DFT coefficients
	Mat invDFTBlue, invDFTcvtBlue, invDFTGreen, invDFTcvtGreen, invDFTRed, invDFTcvtRed;

	idft(complexBlue, invDFTBlue, DFT_SCALE | DFT_REAL_OUTPUT); // Applying IDFT
	idft(complexGreen, invDFTGreen, DFT_SCALE | DFT_REAL_OUTPUT); // Applying IDFT
	idft(complexRed, invDFTRed, DFT_SCALE | DFT_REAL_OUTPUT); // Applying IDFT

	invDFTBlue.convertTo(invDFTcvtBlue, CV_8U);
	invDFTGreen.convertTo(invDFTcvtGreen, CV_8U);
	invDFTRed.convertTo(invDFTcvtRed, CV_8U);

	namedWindow("invDFT Blue", CV_WINDOW_AUTOSIZE);
	imshow("invDFT Blue", invDFTcvtBlue);
	namedWindow("invDFT Green", CV_WINDOW_AUTOSIZE);
	imshow("invDFT Green", invDFTcvtGreen);
	namedWindow("invDFT Red", CV_WINDOW_AUTOSIZE);
	imshow("invDFT Red", invDFTcvtRed);

	// ****************************   CIELAB   ********************************//

	//	Mat imagenTempo; //(Mat::zeros(imagen.rows, imagen.cols, CV_32F));
	Mat tempL, tempa, tempb;
	tempL = imagenLab[0];
	tempa = imagenLab[1];
	tempb = imagenLab[2];
	tempL.convertTo(tempL, CV_32F);
	tempa.convertTo(tempa, CV_32F);
	tempb.convertTo(tempb, CV_32F);
	//	Mat planes[] = { temp, imagenTempo, CV_32F };
	//Mat planes[] = { Mat_<float>(imagen), Mat::zeros(imagen.size(), CV_32F) };
	Mat complexL, complexa, complexb;    //Complex plane to contain the DFT coefficients {[0]-Real,[1]-Img}
										 //	merge(planes, 2, complexI);
	dft(tempL, complexL, DFT_COMPLEX_OUTPUT);  // Applying DFT dft(dftInput1, dftImage1, DFT_COMPLEX_OUTPUT); 
	dft(tempa, complexa, DFT_COMPLEX_OUTPUT);  // Applying DFT dft(dftInput1, dftImage1, DFT_COMPLEX_OUTPUT); 
	dft(tempb, complexb, DFT_COMPLEX_OUTPUT);  // Applying DFT dft(dftInput1, dftImage1, DFT_COMPLEX_OUTPUT); 


											   // Reconstructing original image from the DFT coefficients
	Mat invDFTL, invDFTcvtL, invDFTa, invDFTcvta, invDFTb, invDFTcvtb;

	idft(complexL, invDFTL, DFT_SCALE | DFT_REAL_OUTPUT); // Applying IDFT
	idft(complexa, invDFTa, DFT_SCALE | DFT_REAL_OUTPUT); // Applying IDFT
	idft(complexb, invDFTb, DFT_SCALE | DFT_REAL_OUTPUT); // Applying IDFT

	invDFTL.convertTo(invDFTcvtL, CV_8U);
	invDFTa.convertTo(invDFTcvta, CV_8U);
	invDFTb.convertTo(invDFTcvtb, CV_8U);

	namedWindow("invDFT L", CV_WINDOW_AUTOSIZE);
	imshow("invDFT L", invDFTcvtL);
	namedWindow("invDFT a", CV_WINDOW_AUTOSIZE);
	imshow("invDFT a", invDFTcvta);
	namedWindow("invDFT b", CV_WINDOW_AUTOSIZE);
	imshow("invDFT b", invDFTcvtb);


	//	Mat FFMatrix(imagen.rows, imagen.cols, CV_32F);
	//cvFFT(FFMatrix, imagen,0,0 );
	//	dft(imagenBGR[0], FFMatrix, DFT_COMPLEX_OUTPUT);
	//	Mat FFMatrix_temp[3];
	//	FFMatrix_temp(imagen.rows, imagen.cols, CV_32F);
	//FFMatrix_temp(imagen.rows, imagen.cols, CV_32F);
	//	split(FFMatrix,FFMatrix_temp);
	//FFMatrix.convertTo(FFMatrix, CV_8UC1);
	//	namedWindow("FFT", CV_WINDOW_NORMAL);			// create a window for display
	//	imshow("FFT", FFMatrix_temp[0]);


	// ****************************  FIN   FFT  en  BGR y en CLIEAB  ********************************//
	// **********************************************************************************************//

	// **********************************************************************************************//
	// ************************  CALCULO DE MAGNITUDES EN FFT DE RGB Y CIELAB  **********************//

	// complexBlue, complexGreen, complexRed, complexL, complexa, complexb

	Mat magnitudBlue;     // magnitudBlue3
	Mat complexBlueSplit[2];
	split(complexBlue, complexBlueSplit);
	magnitude(complexBlueSplit[0], complexBlueSplit[1], magnitudBlue); //magnitudBlue |B^| es sqrt(R^2 +I^2)
																	   //magnitudBlue = abs(complexBlue); //valor absoluto

																	   //magnitudBlue3 = magnitudBlue2 - magnitudBlue;
																	   //subtract(magnitudBlue2, magnitudBlue, magnitudBlue3);  //absdiff
																	   //magnitudBlue3.convertTo(magnitudBlue3, CV_8U);
																	   //subtract(magnitudBlue, magnitudBlue2, magnitudBlue3);
																	   //cvSub(magnitudBlue, magnitudBlue2, magnitudBlue3);

	Mat magnitudGreen;
	Mat complexGreenSplit[2];
	split(complexGreen, complexGreenSplit);
	magnitude(complexGreenSplit[0], complexGreenSplit[1], magnitudGreen); //magnitudGreen |G^| es sqrt(R^2 +I^2)

	Mat magnitudRed;
	Mat complexRedSplit[2];
	split(complexRed, complexRedSplit);
	magnitude(complexRedSplit[0], complexRedSplit[1], magnitudRed); //magnitudRed |R^| es sqrt(R^2 +I^2)

	Mat magnitudL;
	Mat complexLSplit[2];
	split(complexL, complexLSplit);
	magnitude(complexLSplit[0], complexLSplit[1], magnitudL); //magnitudL |L^| es sqrt(R^2 +I^2)

	Mat magnituda;
	Mat complexaSplit[2];
	split(complexa, complexaSplit);
	magnitude(complexaSplit[0], complexaSplit[1], magnituda); //magnituda |a^| es sqrt(R^2 +I^2)

	Mat magnitudb;
	Mat complexbSplit[2];
	split(complexb, complexbSplit);
	magnitude(complexbSplit[0], complexbSplit[1], magnitudb); //magnitudb |b^| es sqrt(R^2 +I^2)

															  //cout << magnitudBlue;

															  // ***************** NOTA:
															  //					LAS MAGNITUDES ESTAN ALMACENADAS EN LAS VARIABLES
															  //					magnitudBlue, magnitudGreen, magnitudRed, magnitudL, magnituda, magnitudb


															  // ********************  FIN DE CALCULO DE MAGNITUDES EN FFT DE RGB Y CIELAB  *******************//
															  // **********************************************************************************************//

															  // **********************************************************************************************//
															  // ***********************************  CALCULO DE TITA Y PHI  **********************************//

	Mat TITA;
	TITA = (magnitudBlue + magnitudGreen + magnitudRed - magnitudL) / (magnitudBlue + magnitudGreen + magnitudRed);

	/* // Para TITA promedio>:
	int m, n;
	m = imagen.rows;		// Filas = alto
	n = imagen.cols;		// Columnas = ancho

	double TITA2 = sum(TITA).val[0] / (m*n);
	//	double TITA3 = sum(TITA).val[0];
	cout << "\nTITA:" << TITA2;
	*/

	//	cout << "\nMAGNITUD TITA:" << TITA ; //sum(magnitudb).val[0];
	//	cout << "\Valores de Lab:" << imagenLab[0];

	//cout << "\nAlto:" << m << "\nAncho: " << n;


	Mat PHI;
	PHI = magnituda / (magnituda + magnitudb);
	//	cout << "\nMAGNITUD PHI:" << PHI; //sum(magnitudb).val[0];
	/* // Para TITA promedio>:
	int mm, nn;
	mm = imagen.rows;		// Filas = alto
	nn = imagen.cols;		// Columnas = ancho
	double PHI2 = sum(PHI).val[0] / (mm*nn);
	cout << "\nPHI:" << PHI2;
	*/

	// *********************************** FIN CALCULO DE TITA Y PHI  *******************************//
	// **********************************************************************************************//

	// **********************************************************************************************//
	// *********************************  APLICACION DE LOS VALORES  ********************************//

	//IMAGEN_MODIFICADA = ((1 - TITA).*FFT_L_Shift) + (TITA).*(PHI.* (FFT_a_Shift)+((1 - PHI).*FFT_b_Shift));

	// *** SE DIVIDEN LAS OPERACIONES PARA TRABAJAR CON LA PARTE REAL Y LA IMAGINARIA DE LOS NUMEROS COMPLEJOS
	// *** ((1 - TITA).*FFT_L_Shift) ---------------------------------------- A
	Mat complexLSplit_temp[2];
	complexLSplit_temp[0] = complexLSplit[0];
	complexLSplit_temp[1] = complexLSplit[1];
	Mat unoMenosTita = 1 - TITA;
	multiply(complexLSplit[0], unoMenosTita, complexLSplit_temp[0]);
	multiply(complexLSplit[1], unoMenosTita, complexLSplit_temp[1]);

	// *** PHI.* (FFT_a_Shift)	--------------------------------------------- B
	Mat complexaSplit_temp[2];
	complexaSplit_temp[0] = complexaSplit[0];
	complexaSplit_temp[1] = complexaSplit[1];
	multiply(complexaSplit[0], PHI, complexaSplit_temp[0]);
	multiply(complexaSplit[1], PHI, complexaSplit_temp[1]);

	// *** ((1 - PHI).*FFT_b_Shift)	----------------------------------------- C
	Mat complexbSplit_temp[2];
	complexbSplit_temp[0] = complexbSplit[0];
	complexbSplit_temp[1] = complexbSplit[1];
	Mat unoMenosPhi = 1 - PHI;
	multiply(complexbSplit[0], unoMenosPhi, complexbSplit_temp[0]);
	multiply(complexbSplit[1], unoMenosPhi, complexbSplit_temp[1]);

	// *** B + C = D	----------------------------------------------------- D
	Mat sumaAmasB[2];
	add(complexaSplit_temp[0], complexbSplit_temp[0], sumaAmasB[0]);
	add(complexaSplit_temp[1], complexbSplit_temp[1], sumaAmasB[1]);

	// *** (TITA)*D = E	----------------------------------------------------- E
	Mat titaPorD[2];
	multiply(sumaAmasB[0], TITA, titaPorD[0]);
	multiply(sumaAmasB[1], TITA, titaPorD[1]);

	// *** A + E
	Mat sumaAmasE[2];
	add(titaPorD[0], complexLSplit_temp[0], sumaAmasE[0]);
	add(titaPorD[1], complexLSplit_temp[1], sumaAmasE[1]);


	// ****************************** FIN  APLICACION DE LOS VALORES   ******************************//
	// **********************************************************************************************//

	// **********************************************************************************************//
	// ******************************  IFFT DE LA IMAGEN MODIFICADA  ********************************//

	Mat invDFTFinal, imagenFinal;
	Mat mergeAE; // (imagen.rows, imagen.cols, CV_8UC2);
	merge(sumaAmasE, 2, mergeAE);
	idft(mergeAE, invDFTFinal, DFT_SCALE | DFT_REAL_OUTPUT); // Applying IDFT
	invDFTFinal.convertTo(imagenFinal, CV_8UC1);
	namedWindow("Imagen Final", CV_WINDOW_AUTOSIZE);
	imshow("Imagen Final", imagenFinal);


	//cout << max(imagenFinal, 210);

	// ****************************** CIELAB2BGR *****************************//

	// imagenFinal es de un canal, representado en el espacio Lab
	// ahora, ese canal, lo vamos a llevar al especio BGR
	// el canal L sera el indicado en imagenFinal, el canal a y b seran ceros

	Mat matrizLCerosCeros[3], imagenLabBGR_final;
	//imagenFinal.convertTo(imagenFinal, CV_32FC1);
	matrizLCerosCeros[0] = imagenFinal;
	//matrizLCerosCeros[1] = Mat::zeros(imagen.rows, imagen.cols, CV_32FC1);
	matrizLCerosCeros[1] = Mat::ones(imagen.rows, imagen.cols, CV_8UC1) * 128;
	//matrizLCerosCeros[2] = Mat::zeros(imagen.rows, imagen.cols, CV_32FC1);
	matrizLCerosCeros[2] = Mat::ones(imagen.rows, imagen.cols, CV_8UC1) * 128;
	Mat imagenLabBGR(imagen.rows, imagen.cols, CV_8UC3);		//32FC3			//CV_8UC3   // U porque va de 0 a 255
	merge(matrizLCerosCeros, 3, imagenLabBGR);
	cvtColor(imagenLabBGR, imagenLabBGR_final, CV_Lab2BGR);	// Conversion de CIELab a BGR 

															//	cvtColor(imagenLab_NEW, imagenLab_temp, CV_Lab2BGR);
															//	split(imagenLab_temp, imagenLab);
	namedWindow("L FINAL FINAL", CV_WINDOW_AUTOSIZE);
	imshow("L FINAL FINAL", imagenLabBGR_final);


	cout << "\nPrograma en ejecuci\242n... \n...\n.. \n.";
	waitKey(0);								// wait for a keystroke in the window
	return 0;
}