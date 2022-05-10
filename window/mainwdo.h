#ifndef MAINWDO_H
#define MAINWDO_H

#include <QMainWindow>
#include <QParallelAnimationGroup>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWdo; }
QT_END_NAMESPACE

class MainWdo : public QMainWindow
{
    Q_OBJECT

public:
    MainWdo(QWidget *parent = nullptr);
    ~MainWdo();


private:
    Ui::MainWdo *ui;


private:
    void showMaximize();
    void showNormalize();
    void moveWindow(QMouseEvent *event);

    QString selectMenu(QString getStyle);
    QString deselectMenu(QString getStyle);

    void selectStandardMenu(QString widget);
    void resetStyle(QString widget);

    void theme(QString filename, bool useCustomTheme);
    void start_box_animation(int , int , QString);


private slots:
    void on_maximizeRestoreAppBtn_clicked();
    void toggleMenu();
    void toggleLeftBox();
    void toggleRightBox();
    void buttonClick();


};
#endif // MAINWDO_H
