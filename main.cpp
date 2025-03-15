// main.cpp
#include "PlotterApp.h"
#include <QApplication>
#include <QFile>
#include <QStyle>

class StyleManager {
public:
    static void applyDarkStyle(QApplication& app) {
        // Set application style
        app.setStyle("Fusion");

        // Load stylesheet from file if available, otherwise use embedded style
        QFile styleFile(":/styles/dark_style.qss");
        if (styleFile.exists() && styleFile.open(QFile::ReadOnly | QFile::Text)) {
            QString style = QLatin1String(styleFile.readAll());
            app.setStyleSheet(style);
            styleFile.close();
        } else {
            // Dark theme with modern controls
            app.setStyleSheet(R"(
                QWidget {
                    background-color: #2d2d30;
                    color: white;
                }
                QLineEdit, QTextEdit, QComboBox, QSpinBox, QDoubleSpinBox {
                    background-color: #1e1e1e;
                    color: white;
                    border: 1px solid #444444;
                    border-radius: 4px;
                    padding: 3px;
                }
                QComboBox::drop-down {
                    border: 0px;
                }
                QComboBox::down-arrow {
                    image: url(down_arrow.png);
                    width: 14px;
                    height: 14px;
                }
                QListWidget {
                    background-color: #1e1e1e;
                    color: white;
                    border: 1px solid #444444;
                    border-radius: 4px;
                }
                QListWidget::item:selected {
                    background-color: #2a6e9d;
                }
                QGroupBox {
                    border: 1px solid #444444;
                    border-radius: 4px;
                    margin-top: 1em;
                    padding-top: 10px;
                }
                QGroupBox::title {
                    subcontrol-origin: margin;
                    subcontrol-position: top center;
                    padding: 0 5px;
                    color: #cccccc;
                }
                QLabel {
                    background-color: transparent;
                }
                QCheckBox {
                    background-color: transparent;
                }
                QPushButton {
                    background-color: #555555;
                    color: white;
                    border: none;
                    border-radius: 4px;
                    padding: 5px;
                }
                QPushButton:hover {
                    background-color: #666666;
                }
                QPushButton:pressed {
                    background-color: #444444;
                }
                QPushButton:disabled {
                    background-color: #333333;
                    color: #888888;
                }
                QScrollBar:vertical {
                    border: none;
                    background: #2d2d30;
                    width: 12px;
                    margin: 0px;
                }
                QScrollBar::handle:vertical {
                    background: #666666;
                    min-height: 20px;
                    border-radius: 6px;
                }
                QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                    border: none;
                    background: none;
                    height: 0px;
                }
                QTabWidget::pane {
                    border-top: 1px solid #444444;
                }
                QTabBar::tab {
                    background-color: #333333;
                    color: #cccccc;
                    border: 1px solid #444444;
                    border-bottom: none;
                    padding: 5px 10px;
                    border-top-left-radius: 4px;
                    border-top-right-radius: 4px;
                }
                QTabBar::tab:selected {
                    background-color: #2d2d30;
                    color: white;
                    border-bottom: 1px solid #2d2d30;
                }
                QTabBar::tab:!selected {
                    margin-top: 2px;
                }
            )");
        }
    }

    static void applyLightStyle(QApplication& app) {
        // Implementation for light theme
        app.setStyle("Fusion");
        app.setStyleSheet(""); // Reset to default light style
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Apply the dark style using our style manager
    StyleManager::applyDarkStyle(app);

    PlotterMainWindow window;
    window.show();

    return app.exec();
}
