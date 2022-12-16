#include <iostream>
#include <pthread.h>
#include <random>
#include <ctime>
#include <semaphore.h>

int kVariantCount = 50;

enum Answer{
    kAnswerMark1,
    kAnswerMark2,
    kAnswerMark3,
    kAnswerMark4,
    kAnswerMark5
};

struct Student {
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

void TeacherAnswer(Student& student) {
    student.mark = student.answer;
    bool teacher_ask_additional_question = static_cast<bool>(random() % 2);
    if (teacher_ask_additional_question) {
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
    std::cout << student.full_name << "\n" << "    Variant number: " << student.variant_number << '\n'
              << "    Student`s answer: " << student.answer << '\n' << "    Student`s mark: " << student.mark << "\n\n";
}

void* StudentRequest(void* vptr_args) {
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
    student.additional_question = static_cast<bool>(random() % 2);
    TeacherAnswer(student);
    return nullptr;
}

int main() {
    srand(time(nullptr));
    std::cout << "Enter number of students in group:\n";
    int n;
    std::cin >> n;
    std::vector <Student> students(n);
    for (int i = 1; i <= n; ++i) {
        Student student;
        std::cout << "Enter full name of " << i << " student:\n";
        std::string full_name;
        getline(std::cin >> std::ws, full_name);
        student.full_name = full_name;
        student.variant_number = static_cast<int>(random()) % kVariantCount + 1;
        std::cout << student.full_name << "`s variant: " << student.variant_number << '\n';
        students[i - 1] = student;
    }
    std::vector <pthread_t> threads(n);
    for (int i = 0; i < n; ++i) {
        pthread_t thread;
        if (pthread_create(&thread, nullptr, StudentRequest, &students[i])) {
            std::cout << "Error: creation of thread was failed! Student name: " << students[i].full_name << "\n\n";
            continue;
        }
        threads[i] = thread;
    }
    std::vector <bool> student_answered(n);
    for (int i = 0; i < n; ++i) {
        student_answered[i] = false;
    }
    for (int i = 0; i < n; ++i) {
        int rand_student_num = static_cast<int>(random()) % n;
        while (student_answered[rand_student_num]) {
            rand_student_num = static_cast<int>(random()) % n;
        }
        student_answered[rand_student_num] = true;
        pthread_join(threads[rand_student_num], nullptr);
    }
    return 0;
}
