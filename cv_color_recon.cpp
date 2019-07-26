// cv_color_recon.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

/*
Atenção: o escopo desse projeto utiliza apenas o espaço de cores RGB, para fazer a modificação para outros
espaços de cor é necessário converter a matriz de cores usando o nome_da_matriz.convert_to(ENUM_CORRESPONDENTE).
O enumerator para a matriz possui o formato COLOR_TIPO_DA_MATRIZ, por exemplo COLOR_GRAYSCALE para escala de cinza.
HUE - MATIZ -> varia no intervalo 0 a 360;
SATURATION - SATURAÇÃO -> varia de 0 a 1;
VALUE - VALOR -> varia de 0 a1;
*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <fstream>
#include <string>

using namespace std;
using namespace cv;

Scalar vermelho = Scalar(0  ,0  ,255);
Scalar azul     = Scalar(255,0  ,0  );
Scalar verde    = Scalar(0  ,255,0  );
Scalar laranja  = Scalar(0  ,165,255);
Scalar branco   = Scalar(255,255,255);
Scalar amarelo  = Scalar(0  ,255,255);
Scalar cinza    = Scalar(169, 169, 169);
Scalar preto    = Scalar(0  ,0   ,0);

void proc(Mat& matrix,Scalar cor) {
	flip(matrix, matrix, 1);
	matrix.convertTo(matrix, -1, 2, 0);

	rectangle(matrix, Rect(matrix.cols/2-30, matrix.rows/2-30, 60, 60), cor, 3);
	rectangle(matrix, Rect((matrix.cols/2-70)-30, (matrix.rows/2)-30, 60, 60), cor, 3);
	rectangle(matrix, Rect((matrix.cols/2+70)-30,(matrix.rows/2)-30,60,60),cor,3);
	rectangle(matrix, Rect(matrix.cols/2-30,(matrix.rows/2+70)-30,60,60), cor,3);
	rectangle(matrix, Rect((matrix.cols/2-70)-30,(matrix.rows/2+70)-30,60,60),cor,3);
	rectangle(matrix, Rect((matrix.cols/2+70)-30,(matrix.rows/2+70)-30,60,60),cor,3);
	rectangle(matrix, Rect(matrix.cols/2-30,(matrix.rows/2-70)-30,60,60),cor,3);
	rectangle(matrix, Rect((matrix.cols/2-70)-30,(matrix.rows/2-70)-30,60,60),cor,3);
	rectangle(matrix, Rect((matrix.cols/2+70)-30,(matrix.rows/2-70)-30,60,60),cor,3);
}
void rgbScan(Mat source, int (*cor)[3], int dx, int dy) {
	for (size_t i = 0; i < 9; i++){
		for (size_t j = 0; j < 3; j++){
			cor[i][j] = source.at<Vec3b>(source.rows / 2 + (i/3-1)*dy, source.cols / 2 + (i%3-1)*dx)[j];
		}
	}
}

void colorPatternRecon(Mat matrix, int (*cores)[3], int n, Scalar *cor) {
	rgbScan(matrix, cores, 70, 70);
	
	double r = 0;
	double g = 0;
	double b = 0;
	double mag = 0;

	//cout << fixed << setprecision(2) << "\n";
	system("cls");
	for (int i = 0; i < 9; i++)
	{
		//cout << to_string(cores[0][0])+' ' + to_string(cores[0][1])+ ' ' + to_string(cores[0][2]) + "\n";
		cor[i] = Scalar(cores[i][0], cores[i][1], cores[i][2]);

		b = cor[i].val[0];
		g = cor[i].val[1];
		r = cor[i].val[2];

		mag = sqrt(pow(b, 2) + pow(g, 2) + pow(r, 2));

		b = b/mag; 
		g = g/mag; 
		r = r/mag;
		
		if (b >= 0.60 && b <= 0.69 &&  g >= 0.51 && g <= 0.60 && r >= 0.49 && r <= 0.62) {
			cor[i] = branco;
		}else if (b >= 0.00 && b <= 0.28 && g >= 0.67 && g <= 0.85 && r >= 0.52 && r <= 0.74) {
			cor[i] = verde;
		}else if (b >= 0.00 && b <= 0.24 && g >= 0.49 && g <= 0.65 && r >= 0.74 && r <= 0.87) {
			cor[i] = amarelo;
		}else if (b >= 0.08 && b <= 0.36 && g >= 0.05 && g <= 0.19 && r >= 0.92 && r <= 1.00) {
			cor[i] = vermelho;
		}else if (b >= 0.00 && b <= 0.27 && g >= 0.08 && g <= 0.35 && r >= 0.90 && r <= 1.00) {
			cor[i] = laranja;
		}else if (b >= 0.94 && b <= 0.98 && g >= 0.24 && g <= 0.33  && r >= 0.00 && r <= 0.02) {
			cor[i] = azul;
		}
		else {
			cor[i] = cinza;
		}
	}
}

void colorPatternPlot(Mat frame,Scalar *cor) {
	int margin[2], gap[2];
	int width = 10;
	int height = 10;
	margin[0] = 5;
	margin[1] = 5;
	gap[0] = 3; gap[1] = 3;

	//criação de uma imagem no canto superior esquedo da tela mostrando o resultado da leitura do algoritmo.

	for (int i = 1; i <= 3; i++) {
		for (int j = 1; j <= 3; j++) {
			rectangle(frame, Point(margin[0] + (i - 1) * width + gap[0] * (i - 1),
				margin[1] + (j - 1) * height + gap[1] * (j - 1)),
				Point(margin[0] + i * width + gap[0] * (i - 1),
				margin[1] + j * height + gap[1] * (j - 1)), 
				cor[(j - 1) * 3 + (i - 1)], -1);
			rectangle(frame, Point(margin[0] + (i - 1) * width + gap[0] * (i - 1),
				margin[1] + (j - 1) * height + gap[1] * (j - 1)),
				Point(margin[0] + i * width + gap[0] * (i - 1),
				margin[1] + j * height + gap[1] * (j - 1)),
				preto, 0);
		}
	}
}

int main()
{ 
	Scalar cor[9];
	Mat frame, matrix;
	ofstream myfile;
	int cores[9][3];
	int count = 1;

	VideoCapture capt(0);

	myfile.open("dados.txt");

	for (int i = 0; i < count; i++)
	{
		while (capt.read(frame))
		{
			proc(frame, verde);
			blur(frame, matrix, Size(7, 7), Point(-1, -1));
			colorPatternRecon(matrix, cores, 9, cor);
			colorPatternPlot(frame, cor);

			imshow("Stream", frame);

			if (waitKey(1000 / 20) >= 0) {

				imwrite(to_string(i) + ".jpeg", frame);
				break;

			}
		}

	}
	
	imshow("resultado", frame);
	waitKey();
	return 0;
}

// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln
