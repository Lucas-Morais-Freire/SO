#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>

void gradx (cv::Mat* m, cv::Mat* gx) {
    // cantos:

    // caso i = 0 e j = 0:
    gx->at<int>(0, 0) = (int)((m->at<uchar>(1, 0) << 1) +
                               m->at<uchar>(1, 1)
                             ) -
                        (int)((m->at<uchar>(0, 0) << 1) +
                               m->at<uchar>(0, 1));

    // caso i = M - 1 e j = 0:
    gx->at<int>(m->rows - 1, 0) = (int)((m->at<uchar>(m->rows - 1, 0) << 1) +
                                         m->at<uchar>(m->rows - 1, 1)
                                       ) -
                                  (int)((m->at<uchar>(m->rows - 2, 0) << 1) +
                                         m->at<uchar>(m->rows - 2, 1));

    // caso i = 0 e j = N - 1:
    gx->at<int>(0, m->cols - 1) = (int)((m->at<uchar>(1, m->cols - 1) << 1) +
                                         m->at<uchar>(1, m->cols - 2)
                                       ) -
                                  (int)((m->at<uchar>(0, m->cols - 1) << 1) +
                                         m->at<uchar>(0, m->cols - 2));

    // caso i = M - 1 e j = N - 1:
    gx->at<int>(m->rows - 1, m->cols - 1) = (int)((m->at<uchar>(m->rows - 1, m->cols - 1) << 1) +
                                                   m->at<uchar>(m->rows - 1, m->cols - 2)
                                                 ) -
                                            (int)((m->at<uchar>(m->rows - 2, m->cols - 1) << 1) +
                                                   m->at<uchar>(m->rows - 2, m->cols - 2));

    // arestas:

    // caso i = 0 e 1 <= j <= N - 2:
    for (int j = 1; j < m->cols - 1; j++) {
        gx->at<int>(0, j) = (int)(m->at<uchar>(1, j - 1) +
                                  m->at<uchar>(1, j) +
                                  m->at<uchar>(1, j + 1)
                                 ) -
                            (int)(m->at<uchar>(0, j - 1) +
                                  m->at<uchar>(0, j) +
                                  m->at<uchar>(0, j + 1)
                                 );
    }

    // caso i = M - 1 e 1 <= j <= N - 2:
    for (int j = 1; j < m->cols - 1; j++) {
        gx->at<int>(m->rows - 1, j) = (int)(m->at<uchar>(m->rows - 1, j - 1) +
                                            m->at<uchar>(m->rows - 1, j) +
                                            m->at<uchar>(m->rows - 1, j + 1)
                                           ) -
                                      (int)(m->at<uchar>(m->rows - 2, j - 1) +
                                            m->at<uchar>(m->rows - 2, j) +
                                            m->at<uchar>(m->rows - 2, j + 1)
                                           );
    }

    // caso 1 <= i <= M - 1 e j = 0:
    for (int i = 1; i < m->rows - 1; i++) {
        gx->at<int>(i, 0) = (int)((m->at<uchar>(i + 1, 0) << 1) +
                                   m->at<uchar>(i + 1, 1)
                                 ) -
                            (int)((m->at<uchar>(i - 1, 0) << 1) +
                                   m->at<uchar>(i - 1, 1)
                                 );
    }

    // caso 1 <= i <= M - 1 e j = N - 1:
    for (int i = 1; i < m->rows - 1; i++) {
        gx->at<int>(i, m->cols - 1) = (int)((m->at<uchar>(i + 1, m->cols - 1) << 1) +
                                             m->at<uchar>(i + 1, m->cols - 2)
                                           ) -
                                      (int)((m->at<uchar>(i - 1, m->cols - 1) << 1) +
                                             m->at<uchar>(i - 1, m->cols - 2)
                                           );
    }


    // caso 1 <= i <= M - 1 e 1 <= j <= N - 2
    for (int i = 1; i < m->rows - 1; i++) {
        for (int j = 1; j < m->cols - 1; j++) {
            gx->at<int>(i, j) = (int)(
                                        m->at<uchar>(i + 1, j - 1) +
                                        m->at<uchar>(i + 1, j) +
                                        m->at<uchar>(i + 1, j + 1)
                                     ) -
                                (int)(
                                        m->at<uchar>(i - 1, j - 1) +
                                        m->at<uchar>(i - 1, j) +
                                        m->at<uchar>(i - 1, j + 1)
                                     );
        }
    }

    *gx = cv::abs(*gx);
}

void grady (cv::Mat* m, cv::Mat* gy) {
    // cantos:

    // caso i = 0 e j = 0:
    gy->at<int>(0, 0) = (int)((m->at<uchar>(0, 1) << 1) +
                               m->at<uchar>(1, 1)
                             ) -
                        (int)((m->at<uchar>(0, 0) << 1) +
                               m->at<uchar>(1, 0));

    // caso i = M - 1 e j = 0:
    gy->at<int>(m->rows - 1, 0) = (int)((m->at<uchar>(m->rows - 1, 1) << 1) +
                                         m->at<uchar>(m->rows - 2, 1)
                                       ) -
                                  (int)((m->at<uchar>(m->rows - 1, 0) << 1) +
                                         m->at<uchar>(m->rows - 2, 0));

    // caso i = 0 e j = N - 1:
    gy->at<int>(0, m->cols - 1) = (int)((m->at<uchar>(0, m->cols - 1) << 1) +
                                         m->at<uchar>(1, m->cols - 1)
                                       ) -
                                  (int)((m->at<uchar>(0, m->cols - 2) << 1) +
                                         m->at<uchar>(1, m->cols - 2));

    // caso i = M - 1 e j = N - 1:
    gy->at<int>(m->rows - 1, m->cols - 1) = (int)((m->at<uchar>(m->rows - 1, m->cols - 1) << 1) +
                                                   m->at<uchar>(m->rows - 2, m->cols - 1)
                                                 ) -
                                            (int)((m->at<uchar>(m->rows - 1, m->cols - 2) << 1) +
                                                   m->at<uchar>(m->rows - 2, m->cols - 2));

    // arestas:

    // caso i = 0 e 1 <= j <= N - 2:
    for (int j = 1; j < m->cols - 1; j++) {
        gy->at<int>(0, j) = (int)((m->at<uchar>(0, j + 1) << 1) +
                                  m->at<uchar>(1, j + 1)
                                 ) -
                            (int)((m->at<uchar>(0, j - 1) << 1) +
                                  m->at<uchar>(1, j - 1)
                                 );
    }

    // caso i = M - 1 e 1 <= j <= N - 2:
    for (int j = 1; j < m->cols - 1; j++) {
        gy->at<int>(m->rows - 1, j) = (int)((m->at<uchar>(m->rows - 1, j + 1) << 1) +
                                             m->at<uchar>(m->rows - 2, j + 1)
                                           ) -
                                      (int)((m->at<uchar>(m->rows - 1, j - 1) << 1) +
                                             m->at<uchar>(m->rows - 2, j - 1)
                                           );
    }

    // caso 1 <= i <= M - 1 e j = 0:
    for (int i = 1; i < m->rows - 1; i++) {
        gy->at<int>(i, 0) = (int)(m->at<uchar>(i - 1, 1) +
                                  m->at<uchar>(i, 1) +
                                  m->at<uchar>(i + 1, 1)
                                 ) -
                            (int)(m->at<uchar>(i - 1, 0) +
                                  m->at<uchar>(i, 0) +
                                  m->at<uchar>(i + 1, 0)
                                 );
    }

    // caso 1 <= i <= M - 1 e j = N - 1:
    for (int i = 1; i < m->rows - 1; i++) {
        gy->at<int>(i, m->cols - 1) = (int)(m->at<uchar>(i - 1, m->cols - 1) +
                                            m->at<uchar>(i, m->cols - 1) +
                                            m->at<uchar>(i + 1, m->cols - 1)
                                           ) -
                                      (int)(m->at<uchar>(i - 1, m->cols - 2) +
                                            m->at<uchar>(i, m->cols - 2) +
                                            m->at<uchar>(i + 1, m->cols - 2)
                                           );
    }


    // caso 1 <= i <= M - 1 e 1 <= j <= N - 2
    for (int i = 1; i < m->rows - 1; i++) {
        for (int j = 1; j < m->cols - 1; j++) {
            gy->at<int>(i, j) = (int)(
                                        m->at<uchar>(i - 1, j + 1) +
                                        m->at<uchar>(i, j + 1) +
                                        m->at<uchar>(i + 1, j + 1)
                                     ) -
                                (int)(
                                        m->at<uchar>(i - 1, j - 1) +
                                        m->at<uchar>(i, j - 1) +
                                        m->at<uchar>(i + 1, j - 1)
                                     );
        }
    }

    *gy = cv::abs(*gy);
}

int main(int argc, const char** argv) {

    // checar se foi passada a quantidade correta de argumentos.
    if (argc != 2) {
        std::cout << "quantidade ilegal de argumentos passados. (" << argc - 1 << ")\n";
        exit(0);
    }
    
    // criar um array para ser codificada a imagem.
    cv::Mat image;
    
    // tentar abrir a imagem.
    try {
        image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
    } catch (std::runtime_error err) { // se um erro ocorrer, abortar o programa.
        std::cerr << err.what() << std::endl;
        exit(-1);
    }

    // criar as matrizes que irao conter os gradientes em x, y e o total.
    cv::Mat gx(image.rows, image.cols, CV_32S);
    cv::Mat gy(image.rows, image.cols, CV_32S);
    cv::Mat g(image.rows, image.cols, CV_32S);

    // criar as threads e chamar as funcoes apropriadas para os dois calculos
    std::thread t1(gradx, &image, &gx), t2(grady, &image, &gy);

    // sincronizar as duas threads com a thread mae.
    t1.join(); t2.join();

    // somar as duas matrizes
    g = gx + gy;

    // matriz de saida
    cv::Mat g_out;

    // converter a matriz de numeros inteiros para matriz de unsigned char com valores de 0 ate 255
    cv::normalize(g, g_out, 0, 255, cv::NORM_MINMAX, CV_8U);

    // escrever a imagem em um arquivo.
    cv::imwrite("out.png", g_out);

    return 0;
}