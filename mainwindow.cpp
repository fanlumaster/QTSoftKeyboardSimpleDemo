#include "mainwindow.h"

#include <Windows.h>

#include <QPushButton>

#include "./ui_mainwindow.h"

void SendUnicode(const wchar_t data) {
    INPUT input[4];
    HWND current_hwnd = GetForegroundWindow();
    SetFocus(current_hwnd);

    input[0].type = INPUT_KEYBOARD;
    input[0].ki.wVk = 0;
    input[0].ki.wScan = data;
    input[0].ki.dwFlags = KEYEVENTF_UNICODE;
    input[0].ki.time = 0;
    input[0].ki.dwExtraInfo = GetMessageExtraInfo();
    SendInput(1, &input[0], sizeof(INPUT));

    input[1].type = INPUT_KEYBOARD;
    input[1].ki.wVk = 0;
    input[1].ki.wScan = data;
    input[1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
    input[1].ki.time = 0;
    input[1].ki.dwExtraInfo = GetMessageExtraInfo();
    SendInput(1, &input[1], sizeof(INPUT));
}

void FanySendKeys(QString msg) {
    std::wstring data = msg.toStdWString();
    SendUnicode(data.at(0));
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowTitle("fanyfull");
    setWindowFlags(Qt::WindowStaysOnTopHint);

    QPushButton* button = new QPushButton("A", this);
    button->setGeometry(150, 150, 100, 30);
    connect(button, &QPushButton::clicked, this, &MainWindow::buttonClicked);

    LONG exs = GetWindowLong((HWND)this->winId(), GWL_EXSTYLE);
    exs |= WS_EX_NOACTIVATE;
    exs |= WS_EX_APPWINDOW;
    exs |= WS_EX_TOPMOST;
    SetWindowLong((HWND)this->winId(), GWL_EXSTYLE, exs);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::buttonClicked() { FanySendKeys("A"); }