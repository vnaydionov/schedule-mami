#include "documentparser.h"
#include <QMessageBox>

using namespace std;


DocumentParser::DocumentParser(const QByteArray &content, const QString &doctype)
{
    QDomDocument doc;
    doc.setContent(content, false);
    if (doctype == "docx")
    {
       QDomNodeList tables = doc.elementsByTagName("w:tbl");
       if (tables.size() == 0)
           throw ParserEx("DocumentParser", "no one table found in document");
       table_ = tables.at(0);
       // tr обозначает строку
       rows_ = table_.toElement().elementsByTagName("w:tr");
    }

}

QVector<Row>
DocumentParser::getRows()
{

    QVector<Row> rows_vector;
    for (int i = 0; i < rows_.count(); ++i)
    {
        // tc - столбец
        QDomNodeList cells = rows_.at(i).toElement().elementsByTagName("w:tc");
        Row row;
        for (int j = 0; j < cells.count(); ++j)
        {
            // p - блок с записью (предположительно до перевода строки)
            QDomNodeList blocks = cells.at(j).toElement().elementsByTagName("w:p");
            Cell cell;
            for (int k = 0; k < blocks.count(); ++k){
                QString content = parseBlock(blocks.at(k));
                cell.addBlock(content);
            }
            row.push_back(cell);
        }
        rows_vector.push_back(row);
    }
    return rows_vector;
}

QString
DocumentParser::parseBlock(QDomNode block)
{
    QString textInBlock = "";
    // В t текстовое содержимое
    QDomNodeList contents = block.toElement().elementsByTagName("w:t");
    for (int i = 0; i < contents.count(); ++i){
        textInBlock += contents.at(i).toElement().text();
    }
    return textInBlock;
}
