#ifndef DARKTHEME_H
#define DARKTHEME_H

#include <QApplication>
#include <QPalette>
#include <QStyleFactory>
#include <QColor>
#include <QString>
#include <QWidget>

#ifdef Q_OS_WIN
#    ifndef NOMINMAX
#        define NOMINMAX
#    endif
#    ifndef WIN32_LEAN_AND_MEAN
#        define WIN32_LEAN_AND_MEAN
#    endif
#    include <windows.h>
#endif

/*
 * Dark Fusion Theme for Qt 5/6
 *
 * Design rationale
 * ----------------
 * Surface hierarchy (dark → light):
 *   Base (#21252b)  →  Window (#282c34)  →  Button (#3a3f4b)
 *
 * Fusion draws bevels, frames and grooves from five shading roles:
 *   Light / Midlight / Mid / Dark / Shadow
 * These are tuned so that raised elements read as raised and sunken
 * elements read as sunken, even on a dark background.
 *
 * Text uses One Dark's foreground (#abb2bf) rather than pure white.
 * The accent colour (Highlight) uses One Dark's blue (#61afef).
 */

namespace DarkTheme
{

// ── Colours ─────────────────────────────────────────────────────
// Edit these to taste; the apply() function below reads them.

// Surfaces
inline QColor base()          { return QColor("#21252b"); }  // deepest: input fields, lists, tree views
inline QColor alternateBase() { return QColor("#2c313c"); }  // alternating-row tint
inline QColor window()        { return QColor("#282c34"); }  // main window / panel background
inline QColor button()        { return QColor("#3a3f4b"); }  // raised controls

// Shading (Fusion's bevel engine)
inline QColor light()         { return QColor("#4b5263"); }  // top-left highlight on raised widgets
inline QColor midlight()      { return QColor("#3f4654"); }  // inner highlight
inline QColor mid()           { return QColor("#2c313c"); }  // border / frame colour
inline QColor dark()          { return QColor("#1d2027"); }  // bottom-right edge of raised widgets
inline QColor shadow()        { return QColor("#151820"); }  // drop shadow

// Text
inline QColor text()          { return QColor("#abb2bf"); } // primary text
inline QColor brightText()    { return QColor("#e06c75"); } // urgent / alert
inline QColor placeholderText() { return QColor("#5c6370"); }

// Accent
inline QColor highlight()     { return QColor("#61afef"); } // selection / focus
inline QColor highlightedText() { return QColor("#282c34"); }

// Links
inline QColor link()          { return QColor("#61afef"); }
inline QColor linkVisited()   { return QColor("#c678dd"); }

// Tooltips
inline QColor toolTipBase()   { return QColor("#3a3f4b"); }
inline QColor toolTipText()   { return QColor("#abb2bf"); }

// Disabled text tint
inline QColor disabled()      { return QColor("#5c6370"); }


// ── QtADS ───────────────────────────────────────────────────────

inline QString dockManagerStyleSheet()
{
    return QStringLiteral(R"(
ads--CDockContainerWidget {
    background: #21252b;
}

ads--CDockContainerWidget > QSplitter {
    padding: 1px;
}

ads--CDockContainerWidget ads--CDockSplitter::handle {
    background: #1d2027;
}

ads--CDockAreaWidget {
    background: #21252b;
    border: 1px solid #3a3f4b;
}

ads--CDockWidget {
    background: #21252b;
    border: none;
}

ads--CDockWidgetTab {
    background: #21252b;
    border: none;
    border-right: 1px solid #3a3f4b;
    padding: 0px 4px;
}

ads--CDockWidgetTab[activeTab="true"] {
    background: #282c34;
    border-bottom: 2px solid #61afef;
}

ads--CDockWidgetTab QLabel {
    color: #7f8694;
}

ads--CDockWidgetTab[activeTab="true"] QLabel {
    color: #abb2bf;
}

ads--CTitleBarButton {
    padding: 0px;
    background: transparent;
    border: none;
}

ads--CTitleBarButton:hover,
#tabCloseButton:hover {
    background: #3a3f4b;
    border: 1px solid #4b5263;
}

ads--CTitleBarButton:pressed,
#tabCloseButton:pressed {
    background: #2c313c;
}

QScrollArea#dockWidgetScrollArea {
    padding: 0px;
    border: none;
}

#tabCloseButton {
    margin-top: 2px;
    background: transparent;
    border: none;
    padding: 0px -2px;
    qproperty-icon: url(:/ads/images/close-button.svg),
        url(:/ads/images/close-button-disabled.svg) disabled;
    qproperty-iconSize: 16px;
}

#tabsMenuButton::menu-indicator {
    image: none;
}

#tabsMenuButton {
    qproperty-icon: url(:/ads/images/tabs-menu-button.svg);
    qproperty-iconSize: 16px;
}

#dockAreaCloseButton {
    qproperty-icon: url(:/ads/images/close-button.svg),
        url(:/ads/images/close-button-disabled.svg) disabled;
    qproperty-iconSize: 16px;
}

#detachGroupButton {
    qproperty-icon: url(:/ads/images/detach-button.svg),
        url(:/ads/images/detach-button-disabled.svg) disabled;
    qproperty-iconSize: 16px;
}
)" );
}

// ── Apply ───────────────────────────────────────────────────────

inline void applyPalette()
{
    // 1. Switch to Fusion — the only built-in style that fully
    //    respects QPalette for every widget.
    qApp->setStyle(QStyleFactory::create("Fusion"));

    // 2. Build the palette.
    QPalette p;

    // --- Active & Inactive groups (identical for a dark-only app) ---

    p.setColor(QPalette::Window,          window());
    p.setColor(QPalette::WindowText,      text());
    p.setColor(QPalette::Base,            base());
    p.setColor(QPalette::AlternateBase,   alternateBase());
    p.setColor(QPalette::Text,            text());
    p.setColor(QPalette::Button,          button());
    p.setColor(QPalette::ButtonText,      text());
    p.setColor(QPalette::BrightText,      brightText());

    p.setColor(QPalette::Light,           light());
    p.setColor(QPalette::Midlight,        midlight());
    p.setColor(QPalette::Mid,             mid());
    p.setColor(QPalette::Dark,            dark());
    p.setColor(QPalette::Shadow,          shadow());

    p.setColor(QPalette::Highlight,       highlight());
    p.setColor(QPalette::HighlightedText, highlightedText());

    p.setColor(QPalette::Link,            link());
    p.setColor(QPalette::LinkVisited,     linkVisited());

    p.setColor(QPalette::ToolTipBase,     toolTipBase());
    p.setColor(QPalette::ToolTipText,     toolTipText());

#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
    p.setColor(QPalette::PlaceholderText, placeholderText());
#endif

    // --- Disabled group ---
    // Muted but still legible; Fusion tints disabled controls on
    // its own, so we only need to override the text roles.

    p.setColor(QPalette::Disabled, QPalette::WindowText,      disabled());
    p.setColor(QPalette::Disabled, QPalette::Text,            disabled());
    p.setColor(QPalette::Disabled, QPalette::ButtonText,      disabled());
    p.setColor(QPalette::Disabled, QPalette::HighlightedText, disabled());
    p.setColor(QPalette::Disabled, QPalette::Highlight,       button());
    p.setColor(QPalette::Disabled, QPalette::Base,            window()); // flatten disabled inputs into the background

    qApp->setPalette(p);
}

inline void apply()
{
    applyPalette();
}

inline void applyToWindow(QWidget* window)
{
#ifdef Q_OS_WIN
    if (!window)
        return;

    using DwmSetWindowAttributeFn = HRESULT(WINAPI*)(HWND, DWORD, LPCVOID, DWORD);

    HMODULE dwmapi = LoadLibraryW(L"dwmapi.dll");
    if (!dwmapi)
        return;

    auto setWindowAttribute = reinterpret_cast<DwmSetWindowAttributeFn>(GetProcAddress(dwmapi, "DwmSetWindowAttribute"));
    if (setWindowAttribute)
    {
        const BOOL enabled = TRUE;
        HWND hwnd = reinterpret_cast<HWND>(window->winId());
        constexpr DWORD ImmersiveDarkMode = 20;
        constexpr DWORD ImmersiveDarkModeBefore20H1 = 19;
        setWindowAttribute(hwnd, ImmersiveDarkModeBefore20H1, &enabled, sizeof(enabled));
        setWindowAttribute(hwnd, ImmersiveDarkMode, &enabled, sizeof(enabled));
        SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

        auto refreshWindow = new QWidget(nullptr, Qt::FramelessWindowHint | Qt::Tool | Qt::WindowDoesNotAcceptFocus);
        refreshWindow->setAttribute(Qt::WA_TranslucentBackground);
        refreshWindow->setAttribute(Qt::WA_ShowWithoutActivating);
        refreshWindow->resize(1, 1);
        refreshWindow->move(window->frameGeometry().topLeft());
        refreshWindow->show();
        delete refreshWindow;
    }

    FreeLibrary(dwmapi);
#else
    Q_UNUSED(window);
#endif
}

} // namespace DarkTheme

#endif // DARKTHEME_H
