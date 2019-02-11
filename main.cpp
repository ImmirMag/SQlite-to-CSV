#include <iostream>
#include <fstream>

#include "Poco/File.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/RecordSet.h"

using namespace std;
using namespace Poco::Data::Keywords;

using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::RecordSet;
using Poco::File;

int main() {

    cout << "--- SQlite to CSV Convertor ---" << endl;

    string check_table; // потребуется для проверки на наличие таблицы в базе данных
    string db_name;
    File db_file;

    do // Проверка на существование файла базы данных
    {
        cout << "Enter filename of SQlite Database (*.db file): ";
        cin >> db_name;
        db_file = db_name;

        if (!db_file.exists())
        {
            cout << "Not exist" << endl;
        }


    }while (!db_file.exists());


    Poco::Data::SQLite::Connector::registerConnector();
    Session session("SQLite", db_name);
    Statement select(session);


    string table;

    do // Проверка на существование таблицы в базе данных
    {
        cout << "Enter name of table: ";
        cin >> table;

        select << "SELECT name FROM sqlite_master WHERE type='table' AND name='" + table + "'",into(check_table); //Если таблица существует в БД, то запросы вернёт название таблицы, которое запишется в check_table
        select.execute(); // выполнить запрос
        select.reset(session); // для корректной работы следующего запроса требуется "сброс" предыдущего

            if (table != check_table)
            {
                cout << "Table isn't exist" << endl;
            }

    }while (table != check_table);



    select << "SELECT * FROM " + table, range(0, 1); //  range() - шаг прохождения строк в таблице

    ofstream output_csv_file;
    output_csv_file.open("out.csv",ios_base::out);

    while (!select.done()) // извлечь все строки из таблицы, пока она не закончится
    {
        select.execute(); // при выполнении запросы данные извлекаются не полностью, а построчно
        RecordSet row(select);

        for (int index = 0; index != row.columnCount(); index++)
        {
            output_csv_file << row.value(index).toString() << "\t"; //записать в файл значение из таблицы string-типа и перейти на следующую ячейку в csv-файле
        }

        output_csv_file << "\n"; //перейти на следующую строку в csv-файле
    }

    output_csv_file.close();

    cout << "Convert is successful" << endl;

    return 0;
}
