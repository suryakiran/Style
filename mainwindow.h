#pragma once

#include <ui_mainwindow.h>
#include <palette.h>

class MainWindow : public QMainWindow, private Ui::MainWindow
{
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    void showEvent(QShowEvent* event) override;

private:
    void populateMenu();
    QIcon iconFromTheme(const std::string& theme);
    void initPalette();
    void readStyleSheet();
    void savePalette();
    void setStyleSheet();
    QString substituteColors(const std::string& line);
    void changeTheme();
    void changeBackground();

private:
    QMenu* themeMenu;
    PaletteMap palette;
    PaletteMap backgrounds;
    std::string currentTheme {"Brave"};
    std::string currentBackground {"Light"};
    std::vector <std::string> styleSheet;
};
