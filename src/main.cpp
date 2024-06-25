#include "definitions.h"


int main(int argc, char **argv)
{
    glutInit(&argc, argv);//начальная инициализация окна
    glutInitDisplayMode(GLUT_DOUBLE);//установка режима отображения
    glutInitWindowSize(WINX,WINY);//размер окна
    glutInitWindowPosition(200, 200);//начальная позиция на экране
    //вы заметили что вначале идут функции с приставкой glutInit...?, так вот они должны быть первыми, а потом уже все остальные ф-ии.
    glutCreateWindow("Window");//заголовок окна
    glClearColor(1, 0.5, 1, 0);//цвет фона

    // настройка проекции, с этими двумя ф-ми познакомимся поближе чуть позже.
    glMatrixMode(GL_PROJECTION);//режим матрицы
    glLoadIdentity();//отчищает матрицу
    
    glOrtho(0, WINX, 0, WINY, 0, 1);//cоздаем пространство нашей сцены, в данном случае 3D пространство с высотой, шириной и глубиной в 200 едениц.
    glutDisplayFunc(update);//функция которой мы передаем имя функции для отрисовки окна. 
    glutReshapeFunc(force_reshape);

    prepare();
    glutMainLoop();//запускаем всё проинициализированное, проще говоря та же ф-я main, только в данном случае glut'овская ф-я main.
    return 0;
}