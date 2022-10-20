#pragma once

class QWidget;
class QMainWindow;

void qtRestoreGeometry(QWidget* widget);
void qtSaveGeometry(QWidget* widget);

void qtRestoreState(QMainWindow* window);
void qtSaveState(QMainWindow* window);