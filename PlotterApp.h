// PlotterApp.h
#ifndef PLOTTERAPP_H
#define PLOTTERAPP_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QColor>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QColorDialog>
#include <QMessageBox>
#include <QtMath>
#include <QJSEngine>

// QtCharts includes
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

QT_BEGIN_NAMESPACE
namespace Ui { class PlotterMainWindow; }
QT_END_NAMESPACE

// Resize mode enum for tracking which edge/corner is being dragged
enum class ResizeMode {
    None,
    Left,
    Right,
    Top,
    Bottom,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
};

class EquationPlot {
public:
    QString name;
    QString equation;
    QColor color;
    bool visible;
    double lineWidth;
    QLineSeries *series;

    EquationPlot() : visible(true), lineWidth(2.0), series(nullptr) {}
};

class PlotterMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    PlotterMainWindow(QWidget *parent = nullptr);
    ~PlotterMainWindow();

protected:
    // Override mouse events for custom title bar behavior
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onAddEquationClicked();
    void onRemoveEquationClicked();
    void onGeneratePlotClicked();
    void onClearPlotClicked();
    void onEquationSelectionChanged();
    void onEquationVisibilityChanged(int state);
    void onEquationColorChanged();
    void onLineWidthChanged(double width);
    void onEquationDoubleClicked(QListWidgetItem *item);
    void onSavePlotAsImageClicked();

private:
    void setupUI();
    double evaluateExpression(const QString &expression, double x);

    // Main UI components
    QWidget *centralWidget;

    // Equation controls
    QLineEdit *equationInput;
    QLineEdit *equationNameInput;
    QPushButton *addEquationButton;
    QPushButton *removeEquationButton;
    QListWidget *equationsList;
    QCheckBox *visibilityCheckBox;
    QPushButton *colorButton;
    QDoubleSpinBox *lineWidthSpinBox;

    // Plot controls
    QDoubleSpinBox *xMinSpinBox;
    QDoubleSpinBox *xMaxSpinBox;
    QDoubleSpinBox *yMinSpinBox;
    QDoubleSpinBox *yMaxSpinBox;
    QSpinBox *pointsSpinBox;
    QPushButton *generatePlotButton;
    QPushButton *clearPlotButton;

    // Chart components
    QChartView *chartView;
    QChart *chart;
    QValueAxis *axisX;
    QValueAxis *axisY;

    // Data storage
    QList<EquationPlot> plots;

    // Additional UI components
    QPushButton *bgColorButton;
    QLineEdit *plotTitleInput;
    QPushButton *textColorButton;
    QJSEngine jsEngine;
    QPushButton *saveImageButton;

    // Custom title bar and resize handling
    bool m_dragging = false;
    QPoint m_dragPosition;
    bool m_resizing = false;
    QPoint m_resizeStartPos;
    QRect m_initialRect;
    ResizeMode m_resizeMode = ResizeMode::None;
};

class CustomDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CustomDialog(QWidget *parent = nullptr)
        : QDialog(parent)
    {
        setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
        setAttribute(Qt::WA_TranslucentBackground);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Clear the entire dialog (fully transparent)
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        painter.fillRect(rect(), Qt::transparent);

        // Switch back to normal composition
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

        // Draw a rounded rectangle for the entire dialog
        int radius = 15; // The corner radius
        QPainterPath path;
        path.addRoundedRect(rect(), radius, radius);

        // Fill with a background color that matches the main window
        painter.setBrush(QColor(45, 45, 48));
        painter.setPen(Qt::NoPen);

        painter.drawPath(path);
    }
};

class DialogMoveFilter : public QObject {
public:
    DialogMoveFilter(QDialog *dialog, QWidget *titleBar)
        : QObject(titleBar), m_dialog(dialog), m_titleBar(titleBar), m_dragging(false) {}

protected:
    bool eventFilter(QObject *obj, QEvent *event) override {
        if (obj == m_titleBar) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

            if (event->type() == QEvent::MouseButtonPress) {
                if (mouseEvent->button() == Qt::LeftButton) {
                    m_dragging = true;
                    m_dragPosition = mouseEvent->globalPosition().toPoint() - m_dialog->frameGeometry().topLeft();
                    return true;
                }
            }
            else if (event->type() == QEvent::MouseMove) {
                if (m_dragging) {
                    m_dialog->move(mouseEvent->globalPosition().toPoint() - m_dragPosition);
                    return true;
                }
            }
            else if (event->type() == QEvent::MouseButtonRelease) {
                m_dragging = false;
                return true;
            }
        }
        return QObject::eventFilter(obj, event);
    }

private:
    QDialog *m_dialog;
    QWidget *m_titleBar;
    bool m_dragging;
    QPoint m_dragPosition;
};

#endif
