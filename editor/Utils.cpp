#include "Utils.h"
#include <QString>
#include <QListWidget>
#include <QPrintDialog>
#include <QTextDocument>
#include <QTableWidget>
#include <QPrinter>

QString SchedulePrinter::getUnionListOfValues(QListWidget *lst)
{
    QString union_values;
    for (int i=0; i < lst->count(); ++i)
        union_values += ((CommandButton *)lst->itemWidget(lst->item(i)))->text() + "<br>";
    return union_values;
}

QString SchedulePrinter::getTextFromCellWidget(const QWidget *cellWidget, int column)
{
    QString text;
    if (column == 0)
        text = "+";
    else if (column == 1 || column == 6 || column == 7)
        text = ((CommandButton *)cellWidget)->text();
    else
        text = getUnionListOfValues((QListWidget *)cellWidget);
    return text;
}

void SchedulePrinter::print(const QTableView *view)
{
    Q_UNUSED(view)
}

void SchedulePrinter::printToPDF(const QTableView *view)
{
    if (!view)
        return;

    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printdialog(&printer);

    if (printdialog.exec() == QDialog::Accepted) {
        QString html = "<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN'"
                "'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'>"
                "<html xmlns='http://www.w3.org/1999/xhtml' xml:lang='en' lang='en'>"
                "<head>"
                "<title>Schedule</title>"
                "<meta http-equiv='content-type' content='text/html;charset=utf-8' />"
                "</head>"
                "<body>"
                "<br><p align='center'><strong>Table:</strong></p><br><br>"
                "<table border=3 bordercolor='red' cellpadding=5 width='100%'>";
        QTextDocument *doc = new QTextDocument();

        html += "<tr>";
        for (int i = 1; i < view->model()->columnCount(); i++) {
            html += "<td align='center'>";
            html += view->model()->headerData(i, Qt::Horizontal).toString();
            html += "</td>";
        }
        html += "</tr>";

        for (int i = 0; i < view->model()->rowCount(); i++) {
            html += "<tr>";
            for (int j = 1; j < view->model()->columnCount(); j++) {
                if (!view->isColumnHidden(j)) {
                    html += "<td align='center'>";
                    html += view->model()->index(i, j).data().toString();
                    html += "</td>";
                }
            }
            html += "</tr>";
        }
        html += "</table></body></html>";

        doc->setHtml(html);

        doc->print(&printer);

        delete doc;
    }

}
