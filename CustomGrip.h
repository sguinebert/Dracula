#ifndef CUSTOMGRIP_H
#define CUSTOMGRIP_H

#include <QWidget>
#include <QFrame>
#include <QHBoxLayout>
#include <QSizeGrip>
#include <QEvent>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>

class CustomGrip : public QWidget {
    Q_OBJECT

public:
    CustomGrip(QWidget *parent, Qt::Edge position, bool disable_color = false);

    void top(QWidget *form);
    void bottom(QWidget *form);
    void left(QWidget *form);
    void right(QWidget *form);

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
//    void mouseMoveEvent(QMouseEvent *event) override {

//    }
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if(event->type() == QEvent::MouseMove) {
            if(obj == m_top) {
                resizeTop(static_cast<QMouseEvent *>(event));
            }
            else if(obj == m_bottom){
                resizeBottom(static_cast<QMouseEvent *>(event));
            }
            else if(obj == leftGrip){
                resizeLeft(static_cast<QMouseEvent *>(event));
            }
            else if(obj == rightGrip){
                resizeRight(static_cast<QMouseEvent *>(event));
            }
            return true;
        }

        // false means it should be send to target also. as in , we dont remove it.
        // if you return true , you will take the event and widget never sees it so be carefull with that.
        return false;
    }
private:
    QWidget *parentWidget;
    QWidget *gripContainer;
    QSizeGrip *gripTopLeft;
    QSizeGrip *gripTopRight;
    QSizeGrip *gripBottomLeft;
    QSizeGrip *gripBottomRight;
    QSizeGrip *gripLeft;
    QSizeGrip *gripRight;
    Qt::Edge gripPosition;

    void setupUI();
    void resizeTop(QMouseEvent *event);
    void resizeBottom(QMouseEvent *event);
    void resizeLeft(QMouseEvent *event);
    void resizeRight(QMouseEvent *event);


    QFrame *containerTop;
    QFrame *containerBottom;
    QFrame *topLeft;
    QFrame *m_top;
    QFrame *topRight;
    QFrame *bottomLeft;
    QFrame *m_bottom;
    QFrame *bottomRight;
    QFrame *leftGrip;
    QFrame *rightGrip;
};

#endif // CUSTOMGRIP_H
