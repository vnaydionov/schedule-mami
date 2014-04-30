#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QStringList>
#include <QHeaderView>
#include <QPointer>

class HeaderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HeaderWidget(QWidget *parent = 0);
    
signals:
    
public slots:
    
};

class CustomHeader : public QHeaderView{
public:
    CustomHeader(QWidget *parent = 0):QHeaderView(Qt::Horizontal, parent){

        for(int i = 0; i<6; i++){
            headerSections.insert(i,new HeaderWidget(this));
            headerSections[i]->hide();
        }
        setMinimumSectionSize(headerSections[0]->minimumSizeHint().width());
        setDefaultSectionSize(headerSections[0]->minimumSizeHint().width());
    }
protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const {
        if(headerSections[logicalIndex]->geometry() != rect){
            for(int i = 0; i < model()->columnCount(); ++i){
                QRect newRect(sectionViewportPosition(i), rect.y(), sectionSize(i), rect.height());
                headerSections[i]->setGeometry(newRect);
                if(!headerSections[i]->isVisible()) headerSections[i]->show();
            }
        }
    }
    QSize sizeHint() const {
        QSize s = size();
        s.setHeight(headerSections[0]->minimumSizeHint().height());
        return s;
    }
private:
    mutable QVector< QPointer <HeaderWidget> > headerSections;
};


#endif // WIDGET_H
