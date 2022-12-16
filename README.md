# AVSHomework4
Кузьмин Максим БПИ212 Вариант 26

Условие: Задача про экзамен. Преподаватель проводит экзамен у группы студентов. Каждый студент получает свой билет, сообщает его номер и готовит
письменный ответ. Подготовив ответ, он передает его преподавателю. Преподаватель просматривает ответ и сообщает студенту оценку. Требуется создать многопоточное приложение, моделирующее действия преподавателя и студентов. При решении использовать парадигму «клиент-сервер».

Ожидаемая оценка: 6

Модель параллельных вычислений: Студенты параллельно пишут ответ в соответствие с их вариантами. Реализовано посредством клиент-сервер, где клиенты - студенты, а сервер - преподаватель. Сервер (преподаватель) обрабатывает запросы студентов. Парадигма использована верно.

Входные данные: Количество студентов в группе и их имена. Вся остальная информация генерируется рандомно.

Ввод данных с консоли.

Программа лежит в файле main.cpp

Пример входных данных:

Enter number of students in group:
5
Enter full name of 1 student:
Max
Enter full name of 2 student:
Andrey
Enter full name of 3 student:
Victor
Enter full name of 4 student:
Nikita
Enter full name of 5 student:
Olga

Пример выходных данных:

Olga

    Variant number: 20
    Students answer: 4
    Students mark: 3

Max

    Variant number: 24
    Students answer: 1
    Students mark: 1

Victor

    Variant number: 13
    Students answer: 4
    Students mark: 4

Nikita

    Variant number: 50
    Students answer: 4
    Students mark: 5

Andrey

    Variant number: 6
    Students answer: 4
    Students mark: 5

Были добавлены комментарии к программе описывающие полные действия.

Поведение объектов:
Студенты поочереди получают варианты и сразу садятся писать ответ. (StudentRequest())
По мере того, как студент допишет ответ и будет готов отвечать, он подходит к преподавателю (TeacherAnswer())
Преподаватель по очереди выслушивает студентов, если нужно, задаёт доп. вопрос и записывает результаты в журнал.

Запуск из комнадной строки:
Первым делом компилируем: g++ -pthread -lm -o main main.cpp
Далее запускаем файл: ./main
Работает корректно.
