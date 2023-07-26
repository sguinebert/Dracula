#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QMainWindow>
#include <QHeaderView>
#include <QMouseEvent>
#include <QPushButton>

#include "CustomGrip.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
private slots:
    // BUTTONS CLICK
    // Post here your functions for clicked buttons
    // ///////////////////////////////////////////////////////////////
    void buttonClick();

    // RESIZE EVENTS
    // ///////////////////////////////////////////////////////////////
    void resizeEvent(QResizeEvent* event) override;

    // MOUSE CLICK EVENTS
    // ///////////////////////////////////////////////////////////////
    void mousePressEvent(QMouseEvent* event) override {
        // SET DRAG POS WINDOW
        this->dragPos = event->globalPosition();

        // PRINT MOUSE EVENTS
        if (event->buttons() == Qt::LeftButton) {
            qDebug() << "Mouse click: LEFT CLICK";
        }
        if (event->buttons() == Qt::RightButton) {
            qDebug() << "Mouse click: RIGHT CLICK";
        }
    }

    void openCloseLeftBox() {
        toggleLeftBox(true);
    }

    void openCloseRightBox() {
        toggleRightBox(true);
    }

private:
    bool maximize_restore();
    bool returStatus();
    void setStatus(bool status);
    void toggleMenu(bool enable);
    void toggleLeftBox(bool enable);
    void toggleRightBox(bool enable);
    void start_box_animation(int left_box_width, int right_box_width, const QString& direction);
    QString selectMenu(const QString& getStyle);
    QString deselectMenu(const QString& getStyle);
    void selectStandardMenu(const QString& widget);
    void resetStyle(const QString& widget);
    void theme(const QString& file, bool useCustomTheme);
    void resize_grips();

    void uiDefinitions();

    void setThemeHack();

    //bool eventFilter(QObject *object, QEvent *event);

private:
    Ui::MainWindow *ui;
    QPointF dragPos;

    CustomGrip* left_grip, *right_grip, *top_grip, *bottom_grip;
};
#endif // MAINWINDOW_H
