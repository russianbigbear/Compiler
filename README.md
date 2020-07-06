## Часть компилятора по предмету "Теория языков программирования"
### Задание:
Программа: главная программа языка С++. Допускается описание функций с параметрами, допустимых в программе типов.<br/>
Функции возвращают void.<br/><br/>
**Типы данных:** double, char.<br/>
**Операции:** арифметические и сравнения.<br/>
**Операторы:** присваивания и while.<br/>
**Операнды:** простые переменные, элементы одномерных массивов и константы.<br/>
**Константы:** строковые, символьные и целые в 10 с/с.<br/>

### Пример кода из файла Input.txt, работающего в написанном компиляторе:
    char key = 'k', c_str = 'h';
    char locale = 'e';
    double dob, h_doub = 0;
    char a[19];
    double b[20];

    void setlocale(char lang)
    {
      locale = lang;
    }

    void printHello()
    {
      char c1 = 'h',
        c2 = 'e',
        c3 = 'l',
        c4 = 'l',
        c5 = 'o';
    }

    void print()
    {				
      printHello();	
    }

    void T2(char cmp, double d1){}

    void main()
    {
      setlocale('r');
      double chislo1, chislo2;
      b[6 + 7 - 12] = 89;

      while (key != '9')
      {
         chislo1 = (100 + 200 + 300 + 66) / 2;
         chislo2 = 154;
      } 

      chislo1 = chislo1 + chislo2 + 400;		
      chislo2 = b[11];	
    }
