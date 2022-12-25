

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

void print(int i,int mas[], string masName) {
	cout << masName << "[" << i << "]: " << mas[i] << '\n';
}



int main() {
	ifstream in("file.txt"); // Открытие потока для чтения файла
	string line; // Переменная для хранения содержимого файла
	getline(in, line, '\0'); // Считывается почстрочно файл из потока в переменную line
	while (line.find('\n') != -1) {
		line.replace(line.find('\n'), 1, " "); // Изменение перенос строки на пробел
	}
	line.append(" ");

	int NumberMas[100] = { 0 }; // массив для найденных чисел 
	int j = 0; // количество цифр
	char src[512]; //массив для хранения данных из файла
	strcpy(src, line.c_str()); // Перевод строки в массив
	__asm {
		xor eax, eax // eax - аккумулятор числа, зануление.
		mov ebx, 1 // ebx - множитель знака 
		lea esi, src // esi - адрес src
		lea edi, NumberMas // edi - адрес NumberMas

		cmp[esi], 0 // проверка массива на пустоту
		je empty_line // если путо, то пропуск алгоритма

		// основной цикл - чтение символа
		loop_start :
		imul eax, 10 // умножили аккумулятор на 10
			movzx ecx, byte ptr[esi] // считывание символа по адресу esi в ecx
			cmp ecx, '-' // проверка на минус
			jne digit // если нет, то digit

			mov ebx, -1 // если  минус, то множитель знака - -1 
			jmp sign // переход к sign

			digit :
		sub ecx, '0' // преобразование символ к числу
			add eax, ecx // добавление к аккумулятору

			sign :
		inc esi // увеличение esi (т.е. переходим к следующему символу)
			cmp byte ptr[esi], ' ' // проверка не пробел
			je loop_end // если пробел, то прыгаем в конец цикла, число закончилось

			cmp byte ptr[esi], 0 // если не пробел, то проверка, что строка не закончилась
			jne loop_start // если не закончилась, то читаем следующий символ, прыгая в начало цикла

			loop_end :
		imul eax, ebx // увеличение аккумулятор на множитель знака (1 или -1)
			add j, 1 // добавлние найденных чисел
			mov[edi], eax // записываем спаршенное число в массив с найденными числами
			add edi, 4 // увеличение адрес NumberMas на 4 (т.к. пишем int - 4 байта)
			xor eax, eax // зануляем eax (сбрасываем аккумулятор)
			mov ebx, 1 // сбрасываем множитель знака
			inc esi // увеличение адрес src (пропускаем пробел)
			cmp byte ptr[esi], 0 // проверка, не закончилась ли строка
			jne loop_start // если не закончилась, то считывание следующий символ, прыгая в начало цикла

			// метка для выхода из ассемблерной вставки 
			empty_line :


	}

	// вывод найденные числа(для проверки)

	cout << "--------------" << endl;
	cout << "Mass: " << endl;
	for (int i = 0; i < j; i++) {
		print(i, NumberMas, "DataMas");
	}
	cout << "--------------" << endl;

	__asm {
		lea edi, NumberMas
		mov ecx, j

		outerloop : lea     ebx, [edi + ecx * 4]
		mov     eax, [edi]
		cmploop : sub     ebx, 4
		cmp     eax, [ebx]
		jle     notyet
		xchg    eax, [ebx]
		notyet : cmp     ebx, edi
		jnz     cmploop
		stosd
		loop    outerloop

	}

	cout << "--------------" << endl;
	cout << "SortMass: " << endl;
	for (int i = 0; i < j; i++) {
		print(i, NumberMas, "DataMas");
	}
	cout << "--------------" << endl;


	int results[3] = {}; // Массив для хранения результата
	
			__asm {
				lea edi, NumberMas
				mov ecx, 0
				mov eax, [edi + ecx * 4]
				mov ecx, j
				sub ecx,1
				mov ebx, [edi + ecx * 4]
				sub ebx, eax
				lea esi, results
				mov [results], ebx

				mov ecx, 1
				mov eax, [edi + ecx * 4]
				mov ecx, j
				sub ecx, 1
				mov ebx, [edi + ecx * 4]
				sub ebx, eax
				mov[results+4], ebx

				mov ecx, 0
				mov eax, [edi + ecx * 4]
				mov ecx, j
				sub ecx, 2
				mov ebx, [edi + ecx * 4]
				sub ebx, eax
				mov[results + 8], ebx
			}

			cout << "--------------" << endl;
			for (int i = 0; i < 3; i++) {
				print(i, results, "Result");
			}
			cout << "--------------" << endl;
	std::system("pause");
	return 0;
}