#include "mainwindow.h"
#include "ui_main.h"

#include <QFile>
#include <QObject>
#include <QIcon>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsView>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QTimer>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QEvent>
namespace Settings {
bool ENABLE_CUSTOM_TITLE_BAR = true;
const int MENU_WIDTH = 240;
const int LEFT_BOX_WIDTH = 240;
const int RIGHT_BOX_WIDTH = 240;
const int TIME_ANIMATION = 500;

 QString BTN_LEFT_BOX_COLOR = "background-color: rgb(44, 49, 58);";
 QString BTN_RIGHT_BOX_COLOR = "background-color: #ff79c6;";

 QString MENU_SELECTED_STYLESHEET = R"(
        border-left: 22px solid qlineargradient(spread:pad, x1:0.034, y1:0, x2:0.216, y2:0, stop:0.499 rgba(255, 121, 198, 255), stop:0.5 rgba(85, 170, 255, 0));
        background-color: rgb(40, 44, 52);
    )";

//# GLOBALS
//# ///////////////////////////////////////////////////////////////
bool GLOBAL_STATE = false;
bool GLOBAL_TITLE_BAR = true;

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //CustomWidgets::widgets = &ui;

    // USE CUSTOM TITLE BAR | USE AS "False" FOR MAC OR LINUX
    // ///////////////////////////////////////////////////////////////
    Settings::ENABLE_CUSTOM_TITLE_BAR = true;

    // APP NAME
    // ///////////////////////////////////////////////////////////////
    QString title = "PyDracula - Modern GUI";
    QString description = "PyDracula APP - Theme with colors based on Dracula for Python.";
    // APPLY TEXTS
    setWindowTitle(title);
    ui->titleRightInfo->setText(description);

    // TOGGLE MENU
    // ///////////////////////////////////////////////////////////////
    connect(ui->toggleButton, &QPushButton::clicked, [=]() { toggleMenu(true); });

    // SET UI DEFINITIONS
    // ///////////////////////////////////////////////////////////////
    uiDefinitions();

    // QTableWidget PARAMETERS
    // ///////////////////////////////////////////////////////////////
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // BUTTONS CLICK
    // ///////////////////////////////////////////////////////////////

    // LEFT MENUS
    connect(ui->btn_home, &QPushButton::clicked, this, &MainWindow::buttonClick);
    connect(ui->btn_widgets, &QPushButton::clicked, this, &MainWindow::buttonClick);
    connect(ui->btn_new, &QPushButton::clicked, this, &MainWindow::buttonClick);
    connect(ui->btn_save, &QPushButton::clicked, this, &MainWindow::buttonClick);

    // EXTRA LEFT BOX
    connect(ui->toggleLeftBox, &QPushButton::clicked, this, &MainWindow::openCloseLeftBox);
    connect(ui->extraCloseColumnBtn, &QPushButton::clicked, this, &MainWindow::openCloseLeftBox);

    // EXTRA RIGHT BOX
    connect(ui->settingsTopBtn, &QPushButton::clicked, this, &MainWindow::openCloseRightBox);

    // SHOW APP
    // ///////////////////////////////////////////////////////////////
    show();

    // SET CUSTOM THEME
    // ///////////////////////////////////////////////////////////////
    bool useCustomTheme = true;
    QString themeFile = ":themes/py_dracula_light.qss";

    // SET THEME AND HACKS
    if (useCustomTheme) {
        // LOAD AND APPLY STYLE
        theme(themeFile, true);

        // SET HACKS
        setThemeHack();
    }

    // SET HOME PAGE AND SELECT MENU
    // ///////////////////////////////////////////////////////////////
    ui->stackedWidget->setCurrentWidget(ui->home);
    ui->btn_home->setStyleSheet(selectMenu(ui->btn_home->styleSheet()));

    ui->titleRightInfo->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    //qDebug() << "test " << obj;
    if(obj == ui->titleRightInfo) {

        if(event->type() == QEvent::MouseButtonDblClick) {
            // IF DOUBLE CLICK CHANGE STATUS
            if (event->type() == QEvent::MouseButtonDblClick) {
                QTimer::singleShot(250, [&]() { maximize_restore(); });
            }
        }
        else if(event->type() == QEvent::MouseMove)
        {
            // IF MAXIMIZED CHANGE TO NORMAL
            if (returStatus()) {
                maximize_restore();
            }
            QMouseEvent* evt = static_cast<QMouseEvent*>(event);
            // MOVE WINDOW
            if (evt->buttons() == Qt::LeftButton) {
                auto npos = pos() + evt->globalPosition() - this->dragPos;
                this->move(npos.rx(), npos.ry());
                this->dragPos = evt->globalPosition();
                event->accept();
            }
        }
    }

    // false means it should be send to target also. as in , we dont remove it.
    // if you return true , you will take the event and widget never sees it so be carefull with that.
    return false;
}

void MainWindow::buttonClick() {
    // GET BUTTON CLICKED
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    QString btnName = btn->objectName();

    // SHOW HOME PAGE
    if (btnName == "btn_home") {
        ui->stackedWidget->setCurrentWidget(ui->home);
        resetStyle(btnName);
        btn->setStyleSheet(selectMenu(btn->styleSheet()));
    }

    // SHOW WIDGETS PAGE
    if (btnName == "btn_widgets") {
        ui->stackedWidget->setCurrentWidget(ui->widgets);
        resetStyle(btnName);
        btn->setStyleSheet(selectMenu(btn->styleSheet()));
    }

    // SHOW NEW PAGE
    if (btnName == "btn_new") {
        ui->stackedWidget->setCurrentWidget(ui->new_page); // SET PAGE
        resetStyle(btnName); // RESET ANOTHERS BUTTONS SELECTED
        btn->setStyleSheet(selectMenu(btn->styleSheet())); // SELECT MENU
    }

    if (btnName == "btn_save") {
        qDebug() << "Save BTN clicked!";
    }

    // PRINT BTN NAME
    qDebug() << "Button \"" << btnName << "\" pressed!";
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    // Update Size Grips
    resize_grips();
}

bool MainWindow::maximize_restore()
{
    bool status = isMaximized();
    if (status) {
        showNormal();
        resize(width() + 1, height() + 1);
        ui->appMargins->setContentsMargins(10, 10, 10, 10);
        ui->maximizeRestoreAppBtn->setToolTip("Maximize");
        ui->maximizeRestoreAppBtn->setIcon(QIcon(":/icons/images/icons/icon_maximize.png"));
        ui->frame_size_grip->show();
        left_grip->show();
        right_grip->show();
        top_grip->show();
        bottom_grip->show();
    } else {
        showMaximized();
        ui->appMargins->setContentsMargins(0, 0, 0, 0);
        ui->maximizeRestoreAppBtn->setToolTip("Restore");
        ui->maximizeRestoreAppBtn->setIcon(QIcon(":/icons/images/icons/icon_restore.png"));
        ui->frame_size_grip->hide();
        left_grip->hide();
        right_grip->hide();
        top_grip->hide();
        bottom_grip->hide();
    }
    //Settings::GLOBAL_STATE = !status;
    return !status;
}

bool MainWindow::returStatus()
{
    return Settings::GLOBAL_STATE;
}

void MainWindow::setStatus(bool status)
{
    Settings::GLOBAL_STATE = status;
}

void MainWindow::toggleMenu(bool enable)
{
    if (enable) {
        // GET WIDTH
        int width = ui->leftMenuBg->width();
        int maxExtend = Settings::MENU_WIDTH;
        int standard = 60;

        // SET MAX WIDTH
        int widthExtended = (width == 60) ? maxExtend : standard;

        // ANIMATION
        QPropertyAnimation* animation = new QPropertyAnimation(ui->leftMenuBg, "minimumWidth");
        animation->setDuration(Settings::TIME_ANIMATION);
        animation->setStartValue(width);
        animation->setEndValue(widthExtended);
        animation->setEasingCurve(QEasingCurve::InOutQuart);
        animation->start();
    }
}

void MainWindow::toggleLeftBox(bool enable)
{
    if (enable) {
        // GET WIDTH
        int width = ui->extraLeftBox->width();
        int widthRightBox = ui->extraRightBox->width();
        int maxExtend = Settings::LEFT_BOX_WIDTH;
        QString color = Settings::BTN_LEFT_BOX_COLOR;
        int standard = 0;

        // GET BTN STYLE
        QString style = ui->toggleLeftBox->styleSheet();

        // SET MAX WIDTH
        int widthExtended = (width == 0) ? maxExtend : standard;

        // Set style based on width
        if (width == 0) {
            ui->toggleLeftBox->setStyleSheet(style + color);
            if (widthRightBox != 0) {
                style = ui->settingsTopBtn->styleSheet();
                ui->settingsTopBtn->setStyleSheet(style.replace(Settings::BTN_RIGHT_BOX_COLOR, ""));
            }
        } else {
            ui->toggleLeftBox->setStyleSheet(style.replace(color, ""));
        }

        start_box_animation(width, widthRightBox, "left");
    }
}

void MainWindow::toggleRightBox(bool enable)
{
    if (enable) {
        // GET WIDTH
        int width = ui->extraRightBox->width();
        int widthLeftBox = ui->extraLeftBox->width();
        int maxExtend = Settings::RIGHT_BOX_WIDTH;
        QString color = Settings::BTN_RIGHT_BOX_COLOR;
        int standard = 0;

        // GET BTN STYLE
        QString style = ui->settingsTopBtn->styleSheet();

        // SET MAX WIDTH
        int widthExtended = (width == 0) ? maxExtend : standard;

        // Set style based on width
        if (width == 0) {
            ui->settingsTopBtn->setStyleSheet(style + color);
            if (widthLeftBox != 0) {
                style = ui->toggleLeftBox->styleSheet();
                ui->toggleLeftBox->setStyleSheet(style.replace(Settings::BTN_LEFT_BOX_COLOR, ""));
            }
        } else {
            ui->settingsTopBtn->setStyleSheet(style.replace(color, ""));
        }

        start_box_animation(widthLeftBox, width, "right");
    }
}

void MainWindow::start_box_animation(int left_box_width, int right_box_width, const QString &direction)
{
    int right_width = 0;
    int left_width = 0;

    // Check values
    if (left_box_width == 0 && direction == "left") {
        left_width = 240;
    } else {
        left_width = 0;
    }
    // Check values
    if (right_box_width == 0 && direction == "right") {
        right_width = 240;
    } else {
        right_width = 0;
    }

    // ANIMATION LEFT BOX
    QPropertyAnimation* left_box = new QPropertyAnimation(ui->extraLeftBox, "minimumWidth");
    left_box->setDuration(Settings::TIME_ANIMATION);
    left_box->setStartValue(left_box_width);
    left_box->setEndValue(left_width);
    left_box->setEasingCurve(QEasingCurve::InOutQuart);

    // ANIMATION RIGHT BOX
    QPropertyAnimation* right_box = new QPropertyAnimation(ui->extraRightBox, "minimumWidth");
    right_box->setDuration(Settings::TIME_ANIMATION);
    right_box->setStartValue(right_box_width);
    right_box->setEndValue(right_width);
    right_box->setEasingCurve(QEasingCurve::InOutQuart);

    // GROUP ANIMATION
    QParallelAnimationGroup* group = new QParallelAnimationGroup();
    group->addAnimation(left_box);
    group->addAnimation(right_box);
    group->start();
}

QString MainWindow::selectMenu(const QString &getStyle)
{
    return getStyle + Settings::MENU_SELECTED_STYLESHEET;
}

QString MainWindow::deselectMenu(const QString &getStyle)
{
    return QString(getStyle).replace(Settings::MENU_SELECTED_STYLESHEET, "");
}

void MainWindow::selectStandardMenu(const QString &widget)
{
    QList<QPushButton*> buttons = ui->topMenu->findChildren<QPushButton*>();
    for (QPushButton* w : buttons) {
        if (w->objectName() == widget) {
            w->setStyleSheet(selectMenu(w->styleSheet()));
        }
    }
}

void MainWindow::resetStyle(const QString &widget)
{
    QList<QPushButton*> buttons = ui->topMenu->findChildren<QPushButton*>();
    for (QPushButton* w : buttons) {
        if (w->objectName() != widget) {
            w->setStyleSheet(deselectMenu(w->styleSheet()));
        }
    }
}

void MainWindow::theme(const QString &file, bool useCustomTheme)
{
    if (useCustomTheme) {
        QFile styleFile(file);
        if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString styleSheet = styleFile.readAll();
            ui->styleSheet->setStyleSheet(styleSheet);
            styleFile.close();
        }
        else {
            qDebug() << "File not found";
        }
    }
}

void MainWindow::resize_grips() {
    if (Settings::ENABLE_CUSTOM_TITLE_BAR) {
        this->left_grip->setGeometry(0, 10, 10, this->height());
        this->right_grip->setGeometry(this->width() - 10, 10, 10, this->height());
        this->top_grip->setGeometry(0, 0, this->width(), 10);
        this->bottom_grip->setGeometry(0, this->height() - 10, this->width(), 10);
    }
}

void MainWindow::uiDefinitions() {
//    auto doubleClickMaximizeRestore = [&](QMouseEvent* event) {
//        // IF DOUBLE CLICK CHANGE STATUS
//        if (event->type() == QEvent::MouseButtonDblClick) {
//            QTimer::singleShot(250, [&]() { maximize_restore(); });
//        }
//    };
    //connect(ui->titleRightInfo, &QWidget::mouseDoubleClickEvent, doubleClickMaximizeRestore);
    //ui->titleRightInfo->mouseDoubleClickEvent = doubleClickMaximizeRestore;

    if (Settings::ENABLE_CUSTOM_TITLE_BAR) {
        // STANDARD TITLE BAR
        setWindowFlags(Qt::FramelessWindowHint);
        setAttribute(Qt::WA_TranslucentBackground);

        // MOVE WINDOW / MAXIMIZE / RESTORE
//        auto moveWindow = [&](QMouseEvent* event) {
//            // IF MAXIMIZED CHANGE TO NORMAL
//            if (returStatus()) {
//                maximize_restore();
//            }
//            // MOVE WINDOW
//            if (event->buttons() == Qt::LeftButton) {
//                auto npos = pos() + event->globalPosition() - this->dragPos;
//                this->move(npos.x(), npos.y());
//                this->dragPos = event->globalPosition();
//                event->accept();
//            }
//        };
        //ui->titleRightInfo->mouseMoveEvent = moveWindow;

        // CUSTOM GRIPS
        this->left_grip = new CustomGrip(this, Qt::LeftEdge, true);
        this->right_grip = new CustomGrip(this, Qt::RightEdge, true);
        this->top_grip = new CustomGrip(this, Qt::TopEdge, true);
        this->bottom_grip = new CustomGrip(this, Qt::BottomEdge, true);

    } else {
        ui->appMargins->setContentsMargins(0, 0, 0, 0);
        ui->minimizeAppBtn->hide();
        ui->maximizeRestoreAppBtn->hide();
        ui->closeAppBtn->hide();
        ui->frame_size_grip->hide();
    }

    // DROP SHADOW
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(17);
    shadow->setXOffset(0);
    shadow->setYOffset(0);
    shadow->setColor(QColor(0, 0, 0, 150));
    ui->bgApp->setGraphicsEffect(shadow);

    // RESIZE WINDOW
    QSizeGrip* sizegrip = new QSizeGrip(ui->frame_size_grip);
    sizegrip->setStyleSheet("width: 20px; height: 20px; margin 0px; padding: 0px;");

    // MINIMIZE
    QObject::connect(ui->minimizeAppBtn, &QPushButton::clicked, [&]() { showMinimized(); });

    // MAXIMIZE/RESTORE
    QObject::connect(ui->maximizeRestoreAppBtn, &QPushButton::clicked, [&]() { maximize_restore(); });

    // CLOSE APPLICATION
    QObject::connect(ui->closeAppBtn, &QPushButton::clicked, [&]() { close(); });
}

void MainWindow::setThemeHack()
{
    Settings::BTN_LEFT_BOX_COLOR = "background-color: #495474;";
    Settings::BTN_RIGHT_BOX_COLOR = "background-color: #495474;";
    Settings::MENU_SELECTED_STYLESHEET = R"(
        border-left: 22px solid qlineargradient(spread:pad, x1:0.034, y1:0, x2:0.216, y2:0, stop:0.499 rgba(255, 121, 198, 255), stop:0.5 rgba(85, 170, 255, 0));
        background-color: #566388;
    )";

    // SET MANUAL STYLES
    ui->lineEdit->setStyleSheet("background-color: #6272a4;");
    ui->pushButton->setStyleSheet("background-color: #6272a4;");
    ui->plainTextEdit->setStyleSheet("background-color: #6272a4;");
    ui->tableWidget->setStyleSheet("QScrollBar:vertical { background: #6272a4; } QScrollBar:horizontal { background: #6272a4; }");
    ui->scrollArea->setStyleSheet("QScrollBar:vertical { background: #6272a4; } QScrollBar:horizontal { background: #6272a4; }");
    ui->comboBox->setStyleSheet("background-color: #6272a4;");
    ui->horizontalScrollBar->setStyleSheet("background-color: #6272a4;");
    ui->verticalScrollBar->setStyleSheet("background-color: #6272a4;");
    ui->commandLinkButton->setStyleSheet("color: #ff79c6;");
}

