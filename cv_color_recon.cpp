// cv_color_recon.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Scalar vermelho = Scalar(0  ,0  ,255);
Scalar azul     = Scalar(255,0  ,0  );
Scalar verde    = Scalar(0  ,255,0  );
Scalar laranja  = Scalar(0  ,165,255);
Scalar branco   = Scalar(255,255,255);
Scalar amarelo  = Scalar(0  ,255,255);
Scalar cinza    = Scalar(169, 169, 169);
int    I_max    = 765;

void proc(Mat& matrix) {
	flip(matrix, matrix, 1);
	matrix.convertTo(matrix, -1, 2, 0);

	rectangle(matrix, Rect(matrix.cols/2-30, matrix.rows/2-30, 60, 60), Scalar(0, 255, 0), 3);
	rectangle(matrix, Rect((matrix.cols/2-70)-30, (matrix.rows/2)-30, 60, 60), Scalar(0, 255, 0), 3);
	rectangle(matrix, Rect((matrix.cols/2+70)-30,(matrix.rows/2)-30,60,60),Scalar(0,255,0),3);
	rectangle(matrix, Rect(matrix.cols/2-30,(matrix.rows/2+70)-30,60,60), Scalar(0,255,0),3);
	rectangle(matrix, Rect((matrix.cols/2-70)-30,(matrix.rows/2+70)-30,60,60),Scalar(0,255,0),3);
	rectangle(matrix, Rect((matrix.cols/2+70)-30,(matrix.rows/2+70)-30,60,60),Scalar(0,255,0),3);
	rectangle(matrix, Rect(matrix.cols/2-30,(matrix.rows/2-70)-30,60,60),Scalar(0,255,0),3);
	rectangle(matrix, Rect((matrix.cols/2-70)-30,(matrix.rows/2-70)-30,60,60),Scalar(0,255,0),3);
	rectangle(matrix, Rect((matrix.cols/2+70)-30,(matrix.rows/2-70)-30,60,60),Scalar(0,255,0),3);
}
void rgbScan(Mat source,Mat& R,Mat& G, Mat& B, int dx, int dy) {
	int a = 0, b = 0;
	for (int i = (source.cols /2+dx) - 10; i < (source.cols/2+dx) + 10; i += 5) {
		for (int j = (source.rows/2+dy) - 10; j < (source.rows/2+dy) + 10; j += 5) {
			B.at<uchar>(a, b) = source.at<Vec3b>(j, i)[0];
			source.at<Vec3b>(j, i)[0] = 255;
			G.at<uchar>(a, b) = source.at<Vec3b>(j, i)[1];
			source.at<Vec3b>(j, i)[1] = 0;
			R.at<uchar>(a, b) = source.at<Vec3b>(j, i)[2];
			source.at<Vec3b>(j, i)[2] = 0;
			b++;
		}
		b = 0;
		a++;
	}
}
int somador(Mat matrix) {
	int soma = 0;
	for (int i = 0; i < matrix.cols; i++) {
		for (int j = 0; j < matrix.rows; j++) {
			soma += (int)matrix.at<uchar>(i,j);
		}
	}
	return soma;
}
Scalar averageColor(Mat R, Mat G, Mat B){
	int r_media = somador(R)/(R.cols*R.rows);
	int g_media = somador(G)/(G.cols*G.rows);
	int b_media = somador(B)/(B.cols*B.rows);

	return Scalar(b_media,g_media,r_media);
}

int maior(Scalar cor) {
	if (cor.val[0] > cor.val[1] && cor.val[0] > cor.val[2])
		return cor.val[0];
	else if (cor.val[1] > cor.val[2])
		return cor.val[1];
	else
		return cor.val[2];
}
/*
void fit(Scalar cor) {
	float I = cor.val[0] + cor.val[1] + cor.val[2];
	float correcao = I_max/I;
	for (int i = 0; i < 3; i++)
		cor.val[i] = cor.val[i]*correcao;
}
*/
int main()
{ 
	Mat frame;
	Mat blue[9];
	//Mat::zeros(4,4,CV_8U);
	Mat green[9];
	Mat red[9];
	for (int i = 0; i < 9; i++) {
		blue[i] = Mat::zeros(4, 4, CV_8U);
		green[i] = Mat::zeros(4, 4, CV_8U);
		red[i] = Mat::zeros(4, 4, CV_8U);
	}
	VideoCapture capt(0);

	//Coleta de imagem por captura

	while (capt.read(frame))
	{
		
		proc(frame);
		imshow("Stream", frame);
		if (waitKey(1000/20) >= 0) {

			//Coleta das cores na área especificada
			/*
			int a = 0, b = 0;
			for (int i = (frame.cols/2)-10; i<(frame.cols/2)+10;i+=5) {
				for (int j = (frame.rows/2)-10; j < (frame.rows/2)+10; j+=5) {
					blue.at<uchar>(a,b)  = frame.at<Vec3b>(j,i)[0];
					frame.at<Vec3b>(j,i)[0] = 255;
					green.at<uchar>(a,b) = frame.at<Vec3b>(j,i)[1];
					frame.at<Vec3b>(j,i)[1] = 0;
					red.at<uchar>(a, b)  = frame.at<Vec3b>(j,i)[2];
					frame.at<Vec3b>(j,i)[2] = 0;
					b++;
				}
				b = 0;
				a++;
			}
			a = 0;
			int a = 0;
			int b = 0;
			for (int i = (frame.cols/2-70) - 10; i < (frame.cols/2-70) + 10; i += 5) {
				for (int j = (frame.rows/2)-10; j < (frame.rows/2) + 10; j += 5) {
					blue2.at<uchar>(a,b) = frame.at<Vec3b>(j,i)[0];
					frame.at<Vec3b>(j,i)[0] = 255;
					green2.at<uchar>(a,b) = frame.at<Vec3b>(j, i)[1];
					frame.at<Vec3b>(j,i)[1] = 0;
					red2.at<uchar>(a,b) = frame.at<Vec3b>(j, i)[2];
					frame.at<Vec3b>(j,i)[2] = 0;
					b++;
				}
				b = 0;
				a++;
			}
			a = 0;
			*/

			blur(frame, frame, Size(11, 11), Point(10,10));

			rgbScan(frame, red[0], green[0], blue[0], -70, -70);
			rgbScan(frame, red[1], green[1], blue[1],   0, -70);
			rgbScan(frame, red[2], green[2], blue[2],  70, -70);

			rgbScan(frame, red[3], green[3], blue[3], -70, 0);
			rgbScan(frame, red[4], green[4], blue[4],   0, 0);
			rgbScan(frame, red[5], green[5], blue[5],  70, 0);
			
			rgbScan(frame, red[6], green[6], blue[6], -70, 70);
			rgbScan(frame, red[7], green[7], blue[7],   0, 70);
			rgbScan(frame, red[8], green[8], blue[8],  70, 70);
			break;
		}
	}

	//Média das Matrizes de intensidade RGB
	/*
	int soma  = 0;
	int media_blue = 0, media_green = 0, media_red = 0;
	for (int i = 0; i < blue[0].cols; i++) {
		for (int j = 0; j < blue[0].rows; j++) {
			cout << (int)blue[0].at<uchar>(i, j) << " ";
			soma += (int)blue[0].at<uchar>(i, j);
		}
		cout << endl;
	}

	cout << endl;
	media_blue = soma / (blue[0].cols * blue[0].rows);
	soma = 0;
	for (int i = 0; i < green[0].cols; i++) {
		for (int j = 0; j < green[0].rows; j++) {
			cout << (int)green[0].at<uchar>(i, j) << " ";
			soma += (int)green[0].at<uchar>(i, j);
		}
		cout << endl;
	}
	cout << endl;
	media_green = soma / (green[0].cols * green[0].rows);
	soma = 0;
	for (int i = 0; i < red[0].cols; i++) {
		for (int j = 0; j < red[0].rows; j++) {
			cout << (int)red[0].at<uchar>(i, j) << " ";
			soma += (int)red[0].at<uchar>(i, j);
		}
		cout << endl;
	}
	cout << endl;
	media_red = soma/(red[0].cols * red[0].rows);
	soma = 0;

	int media_blue2 = 0, media_green2 = 0, media_red2 = 0;

	for (int i = 0; i < blue[1].cols; i++) {
		for (int j = 0; j < blue[1].rows; j++) {
			soma += (int)blue[1].at<uchar>(i, j);
		}
	}
	media_blue2 = soma / (blue[1].cols * blue[1].rows);
	soma = 0;
	for (int i = 0; i < green[1].cols; i++) {
		for (int j = 0; j < green[1].rows; j++) {
			soma += (int)green[1].at<uchar>(i, j);
		}
	}
	media_green2 = soma / (green[1].cols * green[1].rows);
	soma = 0;
	for (int i = 0; i < red[1].cols; i++) {
		for (int j = 0; j < red[1].rows; j++) {
			soma += (int)red[1].at<uchar>(i, j);
		}
		cout << endl;
	}

	media_red2 = soma / (red[1].cols * red[1].rows);
	soma = 0;
	*/

	Scalar cor[9];
	for(int i=0;i<9;i++)
		cor[i]=averageColor(red[i],green[i],blue[i]);
	
	int r = 0;
	int g = 0;
	int b = 0;
	for (int i = 0; i < 9; i++) {
		
		//fit(cor[i]);
		
		b = ((int)(cor[i].val[0]*255/maior(cor[i])+1)/16)*16;
		g = ((int)(cor[i].val[1]*255/maior(cor[i])+1)/16)*16;
		r = ((int)(cor[i].val[2]*255/maior(cor[i])+1)/16)*16;
		

		cout <<"[" << b << ", ";
		cout << g << ", ";
		cout << r << "] ";
		if (i % 3 == 2) {
			cout << endl;
		}

		if (r > g && r > b) {
			if (g > 128) {
				cor[i] = amarelo;
			}else if (b > 128) {
				cor[i] = branco;
			}
			else if (g - b >= 0) {
				cor[i] = laranja;
			}else {
				cor[i] = vermelho;
			}
		}
		else if (b > g) {
			if (r > 128) {
				cor[i] = branco;
			}
			else {
				cor[i] = azul;
			}
		}
		else{
			if (b > 128) {
				cor[i] = branco;
			}
			else {
				cor[i] = verde;
			}
		}
		
		
	}

	rectangle(frame, Point(5, 5), Point(15, 15), cor[0], -1);
	rectangle(frame, Point(18, 5), Point(28, 15), cor[1], -1);
	rectangle(frame, Point(31, 5), Point(41, 15), cor[2], -1);
	rectangle(frame, Point(5, 18), Point(15, 28), cor[3], -1);
	rectangle(frame, Point(18, 18), Point(28, 28), cor[4], -1);
	rectangle(frame, Point(31, 18), Point(41, 28), cor[5], -1);
	rectangle(frame, Point(5, 31), Point(15, 41), cor[6], -1);
	rectangle(frame, Point(18, 31), Point(28, 41), cor[7], -1);
	rectangle(frame, Point(31, 31), Point(41, 41), cor[8], -1);

	imshow("frame", frame);
	imwrite("resultado.jpeg", frame);
	waitKey(0);

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
