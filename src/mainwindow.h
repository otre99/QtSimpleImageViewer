#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>

namespace Ui {
class MainWindow;
}

class ImageViewer;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void loadImage(const QString &image_path);


private slots:
    void on_actionOpen_Image_triggered();
    void showPixel(int i, int j);
    void on_scfSlider_valueChanged(int value);
    void on_goTo100_clicked();
    void on_tbFull_clicked();

private:
    Ui::MainWindow *ui;
    ImageViewer *viewer;
    QString m_lastImg;
};

#endif // MAINWINDOW_H
