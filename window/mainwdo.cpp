#include <QDir>
#include <QFile>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QCryptographicHash>
#include <QGraphicsDropShadowEffect>

#include "window/mainwdo.h"
#include "appsetting.h"
#include "ui_mainwdo.h"

MainWdo::MainWdo(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWdo)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    // 设置窗口阴影
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setColor(Qt::black);
    shadow->setBlurRadius(5);
    shadow->setOffset(0);
    ui->styleSheet->setGraphicsEffect(shadow);

    setWindowTitle("QtDracula - Modern GUI");
    ui->titleRightInfo->setText("QtDracula APP -  Based on PyDracula modify for QT/C++");

    // TOGGLE MENU
    connect(ui->toggleButton, &QPushButton::clicked, this, &MainWdo::toggleMenu);

    // LEFT MENUS
    connect(ui->btn_home, &QPushButton::clicked, this, &MainWdo::buttonClick);
    connect(ui->btn_widgets, &QPushButton::clicked, this, &MainWdo::buttonClick);
    connect(ui->btn_new, &QPushButton::clicked, this, &MainWdo::buttonClick);
    connect(ui->btn_save, &QPushButton::clicked, this, &MainWdo::buttonClick);

    // EXTRA LEFT BOX
    connect(ui->toggleLeftBox, &QPushButton::clicked, this, &MainWdo::toggleLeftBox);
    connect(ui->extraCloseColumnBtn,&QPushButton::clicked, this, &MainWdo::toggleLeftBox);

    // EXTRA RIGHT BOX
    connect(ui->settingsTopBtn, &QPushButton::clicked, this, &MainWdo::toggleRightBox);
}

MainWdo::~MainWdo()
{
    delete ui;
}

void MainWdo::showMaximize()
{
    showMaximized();
    ui->appMargins->setContentsMargins(0, 0, 0, 0);
    ui->maximizeRestoreAppBtn->setToolTip("Restore");
    ui->maximizeRestoreAppBtn->setIcon(QIcon(":/icons/images/icons/icon_restore.png"));
    ui->frame_size_grip->hide();
}

void MainWdo::showNormalize()
{
    showNormal();
    ui->maximizeRestoreAppBtn->setToolTip("Maximize");
    ui->maximizeRestoreAppBtn->setIcon(QIcon(":/icons/images/icons/icon_maximize.png"));
    ui->frame_size_grip->show();
}

void MainWdo::moveWindow(QMouseEvent *event)
{
    if (Qt::WindowMaximized){
        showNormalize();
        if (event->buttons() == Qt::LeftButton) {
            move(pos() + event->globalPosition().toPoint());
            QPoint dragPos = event->globalPos();
        }
    }
}

QString MainWdo::selectMenu(QString getStyle)
{
    return getStyle + MENU_SELECTED_STYLESHEET;
}

QString MainWdo::deselectMenu(QString getStyle)
{
    return getStyle.replace(MENU_SELECTED_STYLESHEET, "");
}

void MainWdo::selectStandardMenu(QString widget)
{
    foreach ( QPushButton *w, ui->topMenu->findChildren<QPushButton *>()){
        if (w->objectName() == widget)
            w->setStyleSheet(selectMenu(w->styleSheet()));
    }
}

void MainWdo::resetStyle(QString widget)
{
    foreach ( QPushButton *w, ui->topMenu->findChildren<QPushButton *>()){
        if (w->objectName() != widget)
            w->setStyleSheet(deselectMenu(w->styleSheet()));
    }
}

void MainWdo::theme(QString filename, bool useCustomTheme)
{
    if (useCustomTheme) {
        QFile file(filename.arg(QDir::currentPath()));
            file.open(QFile::ReadOnly);
            ui->styleSheet->setStyleSheet(file.readAll());
            file.close();
    }
}

void MainWdo::toggleMenu()
{
    int width = ui->leftMenuBg->width();
    int maxExtend = MENU_WIDTH;
    int widthExtended;

    // SET MAX WIDTH
    if (width < maxExtend) widthExtended = maxExtend;
    else widthExtended = MENU_STANDARD;

    // ANIMATION
    QPropertyAnimation *animation = new QPropertyAnimation(ui->leftMenuBg, "minimumWidth");
    animation->setDuration(TIME_ANIMATION);
    animation->setStartValue(width);
    animation->setEndValue(widthExtended);
    animation->setEasingCurve(QEasingCurve::InOutQuart);
    animation->start();

}

void MainWdo::start_box_animation(int left_box_width, int right_box_width, QString direction)
{
    int right_width = 0;
    int left_width  = 0 ;

    if (left_box_width == 0 and direction == "left") left_width = 240;

    if (right_box_width == 0 and direction == "right") right_width = 240;

    // ANIMATION LEFT BOX
    QPropertyAnimation *animation1 = new QPropertyAnimation(ui->extraLeftBox, "minimumWidth");
    animation1->setDuration(TIME_ANIMATION);
    animation1->setStartValue(left_box_width);
    animation1->setEndValue(left_width);
    animation1->setEasingCurve(QEasingCurve::InOutQuart);

    // ANIMATION RIGHT BOX
    QPropertyAnimation *animation2 = new QPropertyAnimation(ui->extraRightBox, "minimumWidth");
    animation2->setDuration(TIME_ANIMATION);
    animation2->setStartValue(right_box_width);
    animation2->setEndValue(right_width);
    animation2->setEasingCurve(QEasingCurve::InOutQuart);

    // GROUP ANIMATION
    QParallelAnimationGroup *animgroup = new QParallelAnimationGroup;
    animgroup->addAnimation(animation1);
    animgroup->addAnimation(animation2);
    animgroup->start();
}

void MainWdo::toggleLeftBox()
{
    // GET WIDTH
    int leftBoxWidth = ui->extraLeftBox->width();
    int righBoxWidth = ui->extraRightBox->width();

    // GET BTN STYLE
    QString color = BTN_LEFT_BOX_COLOR;
    QString style = ui->toggleLeftBox->styleSheet();

    // SET MAX WIDTH
    if (leftBoxWidth < LEFT_BOX_WIDTH) {
        ui->toggleLeftBox->setStyleSheet(style + color);
        if (righBoxWidth > RIGHT_BOX_MIN) {
            QString style = ui->settingsTopBtn->styleSheet();
            ui->settingsTopBtn->setStyleSheet(style.replace(BTN_RIGHT_BOX_COLOR, ""));
        }
    } else {
        ui->toggleLeftBox->setStyleSheet(style.replace(color, ""));
    }
    start_box_animation(leftBoxWidth, righBoxWidth, "left");

}

void MainWdo::toggleRightBox( )
{
    // GET WIDTH
    int righBoxWidth = ui->extraRightBox->width();
    int leftBoxWidth = ui->extraLeftBox->width();

    // GET BTN STYLE
    QString color = BTN_RIGHT_BOX_COLOR;
    QString style = ui->settingsTopBtn->styleSheet();

    // SET MAX WIDTH
    if (righBoxWidth < RIGHT_BOX_WIDTH) {
        ui->settingsTopBtn->setStyleSheet(style + color);
        if (leftBoxWidth > LEFT_BOX_MIN) {
            QString style = ui->toggleLeftBox->styleSheet();
            ui->toggleLeftBox->setStyleSheet(style.replace(BTN_LEFT_BOX_COLOR, ""));
        }
    } else {
        ui->settingsTopBtn->setStyleSheet(style.replace(color, ""));
    }
    start_box_animation(leftBoxWidth, righBoxWidth, "right");

}

void MainWdo::buttonClick()
{
    // GET BUTTON CLICKED
    QObject *obj = sender();
    QPushButton *btn = dynamic_cast<QPushButton*>(obj);
    QString btnName = btn->objectName();

    // SHOW WINDOW PAGE
    if (btnName == "btn_home") {
        ui->stackedWidget->setCurrentWidget(ui->home);
        resetStyle(btnName);
        btn->setStyleSheet(selectMenu(btn->styleSheet()));
    }
    if (btnName == "btn_widgets") {
        ui->stackedWidget->setCurrentWidget(ui->widgets);
        resetStyle(btnName);
        btn->setStyleSheet(selectMenu(btn->styleSheet()));
    }
    if (btnName == "btn_new") {
        ui->stackedWidget->setCurrentWidget(ui->new_page);
        resetStyle(btnName);
        btn->setStyleSheet(selectMenu(btn->styleSheet()));
    }
}

void MainWdo::on_maximizeRestoreAppBtn_clicked()
{
    Qt::WindowStates state = windowState();
    if (state & Qt::WindowMaximized) {
        showNormalize();
    } else {
        showMaximize();
    }
}



