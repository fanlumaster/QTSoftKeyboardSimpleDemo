# 关于

这是一个关于 QT 实现软键盘的一个简单的 demo。

效果如下，

![](https://i.postimg.cc/dVpQrqRy/Animation.gif)

主要逻辑在主窗口 `mainwindow.cpp` 这个代码中。

核心的点在两个地方，或者说，两个函数。

**第一个**是 `MainWindow` 中设置窗口属性的一个 win32 api，即，`SetWindowLong`，这个函数可以通过 `Windows.h` 头文件引入，

```cpp
LONG exs = GetWindowLong((HWND)this->winId(), GWL_EXSTYLE);
exs |= WS_EX_NOACTIVATE;
exs |= WS_EX_APPWINDOW;
exs |= WS_EX_TOPMOST;
SetWindowLong((HWND)this->winId(), GWL_EXSTYLE, exs);
}
```

这里的作用是使得我们的窗口不具有焦点(focus)，也就防止了软键盘在点击的过程中不会抢占目标程序的焦点。同时，这个窗口也是置顶(top most)的。

按：如果想要在所有的情况下都可以置顶，这里需要用到签名，开发过程中可以使用自签名来模拟。这里我们暂时不需要在诸如任务管理器这种管理员窗口中实现置顶，所以暂时没有处理。

**第二个**重要的点就是向前台窗口(程序)的输入框发送按键模拟，这个可以利用 win32 api 提供的 `SendInput` 函数，我们可以实现一个 `SendUnicode` 函数，

```cpp
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
```

这里，获取前台窗口的句柄的函数是 `GetForegroundWindow`，然后 `SetFocus` 一下，就可以让 `SendInput` 函数把 Unicode 字符正确地发送到前台窗口的输入框中了。

------

参考：

1、<http://wowoboke.com/56.html>

