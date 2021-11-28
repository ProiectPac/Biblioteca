#include "sqldatabase.h"

SQLDataBase::SQLDataBase()
{
    const QString driver ="QSQLITE";
    if(QSqlDatabase::isDriverAvailable(driver))
    {
        QSqlDatabase dataBase = QSqlDatabase::addDatabase(driver);
        dataBase.setDatabaseName("../dataBase.db");
        if(!dataBase.open())
        {
            qWarning() << "ERROR: " << dataBase.lastError();
        }
        else
        {
            QSqlQuery query("INSERT INTO users VALUES(\"nae\", 4001284951)");
            if(!query.isActive())
                qWarning() << "ERROR: " << query.lastError().text();
        }
    }
}
