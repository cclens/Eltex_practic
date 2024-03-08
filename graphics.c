#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "navigation.h" // подгружаем интерфейс навигации
#define attributs_upper \
    attron(COLOR_PAIR(3)); \
    mvprintw(0, 1, "Left       File        Command     Option      Right"); \
    for (int k = 53; k <= 100; k++) { \
        mvprintw(0, k, "  "); \
    }; \
    for (int j = 1; j <= 100; j++) { \
        mvprintw(1, j, "  "); \
    }; \
    attroff(COLOR_PAIR(3)); // отрисовка верхней панели

#define attributs_fenc \
    attron(COLOR_PAIR(1)); \
    for (int j = 3; j <= 45; j++) { \
        mvprintw(j, 25, "||"); \
    };// отрисовка заборчиков 


void clear_left(){ //функция очистки левой панели
    int y0 = 2;
    int y1 = 70;
    int x0 = 0;
    int x1 = 45;
    for (int i = y0; i <= y1; i++) {
        for (int j = x0; j <= x1; j++) {
            mvaddch(i, j, ' '); // Записываем пробелы в определенную область экрана
        }
        refresh();
    }
}

void clear_right(){ //функция очистки правой панели
    int y0 = 2;
    int y1 = 70;
    int x0 = 50;
    int x1 = 100;
    for (int i = y0; i <= y1; i++) {
        for (int j = x0; j <= x1; j++) {
            mvaddch(i, j, ' '); // Записываем пробелы в определенную область экрана
        }
        refresh();
    }

}

void draw_graphics(int *count, char **filenames, int *flag) { //главная функция графики
    initscr();
    start_color();
    keypad(stdscr, TRUE);
    noecho();
    
    init_color(100, 100, 0, 0);
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_CYAN, COLOR_BLACK); 
    init_pair(3, COLOR_BLACK, 50);  
    init_pair(4, COLOR_BLUE, COLOR_BLUE); // инициализация всех палитр

    bkgd(COLOR_PAIR(1)); // задний фон
    attributs_upper;
    attributs_fenc;

    char **filenames_left = (char**)malloc(100 * sizeof(char*)); // динамические массивы хранящие элементы правой и левой панели
    char **filenames_right = (char**)malloc(100 * sizeof(char*));
    
           
    for (int i = 0; i < *count; i++) {
     	filenames_left[i] = (char *)malloc((strlen(filenames[i]) +1) * sizeof(char));
     	filenames_right[i] = (char *)malloc((strlen(filenames[i]) +1) * sizeof(char));
		strcpy(filenames_left[i], filenames[i]);
    	strcpy(filenames_right[i], filenames[i]); // заполняем оба массива изначальными файлами в каталоге откуда запущенна программа
}

    int count_files = (*count)-1; // переменная для работы с файлами в выбранной панели
    int x_offset = 4; 
    int y_offset = 2;
    int input_chr = 0;
    int trigger = 0; // отвечает за то, работает правая панель или левая
    int tmp_count = *count; // *count отвечает за левую панель, tmp_count за правую

    for (int i = 0; i <= count_files; i++) {
        if (trigger == i) {
            attron(COLOR_PAIR(2));
            mvprintw(y_offset, x_offset, filenames_left[i]);
            attroff(COLOR_PAIR(2));
        } else {
            mvprintw(y_offset, x_offset, filenames_left[i]);
        }
        y_offset += 2;
    } // отрисовка левой панели
    
    y_offset = 2;
    x_offset = 50;
    
    for (int j = 0; j <= count_files; j++) {
        mvprintw(y_offset, x_offset, filenames_right[j]);
        y_offset += 2; 
    } // отрисовка правой панели

    refresh();

    do { // реализовал ожидание ввода через do while
        input_chr = getch();
        if (input_chr == KEY_DOWN) { // Если нажата клавиша down
            trigger = (trigger + 1); // Обновляем значение trigger для переключения на следующий объект
            if (trigger == count_files+1) {
                trigger = 0;
            }
        }
        if (input_chr == KEY_UP) {
            trigger = (trigger - 1);
            if (trigger < 0) {
                trigger = count_files;
            }
        }
        if (input_chr == 0x09){
            *flag = (*flag)*(-1); // если нажат TAB то рабочая панель меняется
        }        

        if (input_chr == '\n'){ // если нажат ENTER, и в зависимости от панели обновляется содержимое, в filenames_ и очищается панель от прошлого содержимого
            if (*flag == 1){
                current_directory(count, filenames_left, &trigger);
                count_files = *count -1;
                clear_left();
     
            }
            if (*flag == (-1)){
                count_files = tmp_count -1;
                current_directory(&tmp_count, filenames_right, &trigger);
                clear_right();
            } 
            refresh();

          }
       
        attributs_upper;
        attributs_fenc; // отрисовка всех заборчиков и панелей заного, так как до этого был clear
        
        x_offset = 4;
        y_offset = 2;
        if ((*flag) == 1) {
            clear_left();
            attributs_fenc;
            count_files = tmp_count - 1;
            for (int i = 0; i <= count_files; i++) {
                if (trigger == i){
                attron(COLOR_PAIR(2));
                mvprintw(y_offset, x_offset, filenames_left[i]);
                attroff(COLOR_PAIR(2));
            } else {
                mvprintw(y_offset, x_offset, filenames_left[i]);
            }
            y_offset += 2;
            refresh();
            }// вывод содержимого левой панели
        }
        
        y_offset = 2;
        x_offset = 50;
        if ((*flag) == (-1)) {
            clear_right();
            attributs_fenc;
            count_files = tmp_count - 1;
            for (int j = 0; j <= count_files; j++) {
                if (trigger ==j){
                attron(COLOR_PAIR(2));
                mvprintw(y_offset, x_offset, filenames_right[j]);
                attroff(COLOR_PAIR(2));
            } else {
                mvprintw(y_offset, x_offset, filenames_right[j]);
            }
            y_offset += 2; // Увеличиваем y_offset для следующей строки
            refresh();
            }// вывод содержимого правой панели
        }
        
    
        refresh();
    } while (input_chr != 27);

    endwin();
    return;
}


