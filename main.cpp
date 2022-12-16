#include <iostream>
#include <pthread.h>
#include <random>
#include <ctime>
#include <semaphore.h>

int kVariantCount = 50;    // количество вариантов на экзамене

enum Answer{               // енам для определения на какую оценку ответит студент
    kAnswerMark1,
    kAnswerMark2,
    kAnswerMark3,
    kAnswerMark4,
    kAnswerMark5
};

struct Student {           // класс студента
    Student() {
        full_name = "";
        variant_number = 0;
        answer = kAnswerMark1;
        mark = 0;
        additional_question = true;
    }
    std::string full_name;
    int variant_number;
    Answer answer;
    int mark;
    bool additional_question;
};

struct StudentInJournal {   // описание студента в журнале
    std::string full_name;
    int variant_number;
    Answer answer;
    int mark;
};

struct MarksList {          // журнал преподавателя (одновременно можно записывать инфу об одном студенте, что логично
    std::vector<StudentInJournal> list;
    pthread_mutex_t mutex;
};

MarksList marks_list;

void TeacherAnswer(Student& student) {  // ответ преподавателя
    student.mark = student.answer;      // ставится оценка с помощью enum`a
    bool teacher_ask_additional_question = static_cast<bool>(random() % 2); // будет ли задан доп вопрос
    if (teacher_ask_additional_question) {          // обработка доп вопроса
        if (student.additional_question) {
            ++student.mark;
            if (student.mark > 5) {
                student.mark = 5;
            }
        } else {
            --student.mark;
            if (student.mark < 0) {
                student.mark = 0;
            }
        }
    }
    // ограничивается запись в журнал (для текущего студента) и вывод его результатов
    pthread_mutex_lock(&marks_list.mutex);

    StudentInJournal new_write_in_list_mark;
    new_write_in_list_mark.full_name = student.full_name;
    new_write_in_list_mark.variant_number = student.variant_number;
    new_write_in_list_mark.answer = student.answer;
    new_write_in_list_mark.mark = student.mark;
    marks_list.list.push_back(new_write_in_list_mark);
    std::cout << student.full_name << "\n" << "    Variant number: " << student.variant_number << '\n'
              << "    Student`s answer: " << student.answer << '\n' << "    Student`s mark: " << student.mark << "\n\n";

    pthread_mutex_unlock(&marks_list.mutex);
    // снятие ограничения
}

void* StudentRequest(void* vptr_args) {                 // студенту выдаётся вариант, на какую оценку он ответит
                                                        // и как он ответит на доп. вопрос
    Student& student = *(Student *)vptr_args;
    int ans_for_mark = static_cast<int>(random()) % kVariantCount + 1;
    if (ans_for_mark == 1) {
        student.answer = kAnswerMark1;
    } else if (ans_for_mark == 2) {
        student.answer = kAnswerMark2;
    } else if (ans_for_mark == 3) {
        student.answer = kAnswerMark3;
    } else if (ans_for_mark == 4) {
        student.answer = kAnswerMark4;
    } else {
        student.answer = kAnswerMark5;
    }
    student.variant_number = static_cast<int>(random()) % kVariantCount + 1;
    student.additional_question = static_cast<bool>(random() % 2);
    TeacherAnswer(student);                                 // студент подходит к преподавателю
    return nullptr;
}

int main() {
    srand(time(nullptr));

    std::cout << "Enter number of students in group:\n";        // ввод количества студентов
    int n;
    std::cin >> n;

    std::vector <Student> students(n);                          // ввод имён студентов,
    for (int i = 1; i <= n; ++i) {
        Student student;
        std::cout << "Enter full name of " << i << " student:\n";
        std::string full_name;
        getline(std::cin >> std::ws, full_name);
        student.full_name = full_name;
        students[i - 1] = student;
    }

    std::vector <pthread_t> threads(n);                         // массив потоков, который хранит потоки студентов
    std::vector <bool> student_get_task(n);                     // получил ли студент задание
    for (int i = 0; i < n; ++i) {
        student_get_task[i] = false;
    }

    for (int i = 0; i < n; ++i) {                               // создание потоков для студентов в рандомном порядке
        int rand_student_num = static_cast<int>(random()) % n;
        while (student_get_task[rand_student_num]) {
            rand_student_num = static_cast<int>(random()) % n;
        }
        pthread_t thread;
        if (pthread_create(&thread, nullptr, StudentRequest, &students[rand_student_num])) {
            std::cout << "Error: creation of thread was failed! Student name: " << students[rand_student_num].full_name << "\n\n";
            continue;
        }
        threads[rand_student_num] = thread;
        student_get_task[rand_student_num] = true;
    }

    std::vector <bool> student_answered(n);             // массив, отвечал ли студент
    for (int i = 0; i < n; ++i) {
        student_answered[i] = false;
    }
    for (int i = 0; i < n; ++i) {                       // студенты подходят к преподователю по мере того,
                                                        // как они решили (рандомно) и преподаватель его проверяет
        int rand_student_num = static_cast<int>(random()) % n;
        while (student_answered[rand_student_num]) {
            rand_student_num = static_cast<int>(random()) % n;
        }
        student_answered[rand_student_num] = true;
        pthread_join(threads[rand_student_num], nullptr);
    }
    return 0;
}
