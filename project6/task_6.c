// код приложения на языке C, устанавливающий соединение с БД SQLite
 и выполняющий запросы типа SELECT, INSERT, DELETE, параметризованные запросы, фотографию объекта выводит в файл:

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

// функция для вывода сообщения об ошибке
void error_message(const char* message) {
printf("Error: %s\n", message);
exit(1);
}
// функция для выполнения запроса SELECT и вывода результатов в консоль
int select_query(sqlite3* db, const char* query) {
sqlite3_stmt* stmt;
int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
if (rc != SQLITE_OK) {
error_message("Can't prepare SELECT statement");
}
while (sqlite3_step(stmt) == SQLITE_ROW) {
int id = sqlite3_column_int(stmt, 0);
const unsigned char* name = sqlite3_column_text(stmt, 1);
const unsigned char* capital = sqlite3_column_text(stmt, 2);
int population = sqlite3_column_int(stmt, 3);
int area = sqlite3_column_int(stmt, 4);
const unsigned char* currency = sqlite3_column_text(stmt, 5);
const unsigned char* government = sqlite3_column_text(stmt, 6);
int leader_id = sqlite3_column_int(stmt, 7);
printf("%d | %s | %s | %d | %d | %s | %s | %d\n", id, name, capital, population, area, currency, government, leader_id);
}
sqlite3_finalize(stmt);
return 0;
}

// функция для выполнения запроса INSERT
int insert_query(sqlite3* db, const char* query) {
char* error_message = NULL;
int rc = sqlite3_exec(db, query, NULL, NULL, &error_message);
if (rc != SQLITE_OK) {
error_message("Can't execute INSERT statement");
}
return 0;
}
// функция для выполнения параметризованного запроса SELECT и вывода результатов в консоль
int param_select_query(sqlite3* db, const char* query, const char* param) {
sqlite3_stmt* stmt;
int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
if (rc != SQLITE_OK) {
error_message("Can't prepare SELECT statement");
}
sqlite3_bind_text(stmt, 1, param, -1, SQLITE_STATIC);
while (sqlite3_step(stmt) == SQLITE_ROW) {
int id = sqlite3_column_int(stmt, 0);
const unsigned char* name = sqlite3_column_text(stmt, 1);
const unsigned char* capital = sqlite3_column_text(stmt, 2);
int population = sqlite3_column_int(stmt, 3);
int area = sqlite3_column_int(stmt, 4);
const unsigned char* currency = sqlite3_column_text(stmt, 5);
const unsigned char* government = sqlite3_column_text(stmt, 6);
int leader_id = sqlite3_column_int(stmt, 7);
printf("%d | %s | %s | %d | %d | %s | %s | %d\n", id, name, capital, population, area, currency, government, leader_id);
}
sqlite3_finalize(stmt);
return 0;
}

// функция для выполнения запроса DELETE
int delete_query(sqlite3* db, const char* query) {
char* error_message = NULL;
int rc = sqlite3_exec(db, query, NULL, NULL, &error_message);
if (rc != SQLITE_OK) {
error_message("Can't execute DELETE statement");
}
return 0;
}
// функция для вывода фотографии объекта в файлint get_photo(sqlite3* db, const char* query, const char* filename) {sqlite3_stmt* stmt;int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);if (rc != SQLITE_OK) {error_message("Can't prepare SELECT statement");}if (sqlite3_step(stmt) == SQLITE_ROW) {const void* photo = sqlite3_column_blob(stmt, 0);int size = sqlite3_column_bytes(stmt, 0);FILE* file = fopen(filename, "wb");fwrite(photo, size, 1, file);fclose(file);}sqlite3_finalize(stmt);return 0;}int main() {sqlite3* db;int rc = sqlite3_open("test.db", &db);if (rc != SQLITE_OK) {error_message("Can't open database");}// выполнение запроса SELECT
printf("SELECT:\n");
select_query(db, "SELECT * FROM countries");

// выполнение запроса INSERT
printf("INSERT:\n");
insert_query(db, "INSERT INTO countries(name, capital, population, area, currency, government, leader_id) VALUES('Russia', 'Moscow', 144500000, 17098242, 'rubles', 'federal semi-presidential constitutional republic', 1)");

// выполнение параметризованного запроса SELECT
printf("PARAM SELECT:\n");
param_select_query(db, "SELECT * FROM countries WHERE name=?", "Russia");

// выполнение запроса DELETE
printf("DELETE:\n");
delete_query(db, "DELETE FROM countries WHERE name='Russia'");

// вывод фотографии объекта в файл
get_photo(db, "SELECT photo FROM objects WHERE id=1", "object1_photo.jpg");

sqlite3_close(db);
return 0;
}
