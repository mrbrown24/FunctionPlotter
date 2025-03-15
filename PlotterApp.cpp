#include "PlotterApp.h"
#include <QGridLayout>
#include <QStackedWidget>
#include <QSlider>
#include <QtMath>
#include <QMessageBox>
#include <cmath>
#include <QFileDialog>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QApplication>

PlotterMainWindow::PlotterMainWindow(QWidget *parent)
    : QMainWindow(parent), m_resizing(false), m_dragging(false)
{
    // Set frameless window for custom title bar
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    setupUI();

    // Default size
    resize(1200, 800);
}

PlotterMainWindow::~PlotterMainWindow()
{
    // Nothing needed here
}

void PlotterMainWindow::setupUI()
{
    // Set window title
    setWindowTitle("Function Plotter");

    // Create central widget
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Make central widget background transparent
    centralWidget->setAttribute(Qt::WA_TranslucentBackground);

    // Main layout with custom title bar at the top
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10); // Padding
    mainLayout->setSpacing(0);

    // --- Start of Title Bar Area ---
    QWidget *titleBar = new QWidget(centralWidget);
    titleBar->setObjectName("titleBar");
    titleBar->setStyleSheet("#titleBar { background-color: transparent; }");
    titleBar->setFixedHeight(40);

    QHBoxLayout *titleBarLayout = new QHBoxLayout(titleBar);
    titleBarLayout->setContentsMargins(10, 5, 10, 5); // Padding

    // Title label, Centered
    QLabel *titleLabel = new QLabel("Function Plotter", titleBar);
    titleLabel->setStyleSheet("color: white; font-weight: bold; font-size: 14px; background-color: transparent;");
    titleLabel->setAlignment(Qt::AlignCenter);

    // Close button
    QPushButton *closeButton = new QPushButton("×", titleBar);
    closeButton->setFixedSize(30, 30);
    closeButton->setStyleSheet(R"(
        QPushButton {
            color: white;
            background-color: transparent;
            border: none;
            font-size: 18px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #e81123;
        }
    )");


    // Minimize button
    QPushButton *minimizeButton = new QPushButton("−", titleBar);
    minimizeButton->setFixedSize(30, 30);
    minimizeButton->setStyleSheet(R"(
        QPushButton {
            color: white;
            background-color: transparent;
            border: none;
            font-size: 18px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #333333;
        }
    )");

    // Adding titlebar components to titlebarlayout
    titleBarLayout->addStretch(1);
    titleBarLayout->addWidget(titleLabel);
    titleBarLayout->addStretch(1);
    titleBarLayout->addWidget(minimizeButton);
    titleBarLayout->addWidget(closeButton);

    // Add title bar to main layout
    mainLayout->addWidget(titleBar);
    // --- End of Title Bar Area ---

    // --- Start of Content Area ---
    QWidget *contentWidget = new QWidget(centralWidget);
    contentWidget->setObjectName("contentWidget");
    // Transparent background to let the main window's paintEvent handle the background
    contentWidget->setStyleSheet("#contentWidget { background-color: transparent; }");

    QHBoxLayout *contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(8, 8, 8, 8);

    // Create left panel for controls
    QWidget *controlsPanel = new QWidget;
    QVBoxLayout *controlsLayout = new QVBoxLayout(controlsPanel);

    // Equation name input
    QGroupBox *nameGroup = new QGroupBox("Equation Name");
    QVBoxLayout *nameLayout = new QVBoxLayout(nameGroup);
    equationNameInput = new QLineEdit();
    equationNameInput->setPlaceholderText("Enter a name for this equation");
    nameLayout->addWidget(equationNameInput);

    // Equation input
    QGroupBox *equationGroup = new QGroupBox("Equation (use 'x' as variable)");
    QVBoxLayout *equationLayout = new QVBoxLayout(equationGroup);
    equationInput = new QLineEdit();
    equationInput->setPlaceholderText("Example: 2*x^2 + 3*sin(x)");
    equationLayout->addWidget(equationInput);

    // Add and Remove buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    addEquationButton = new QPushButton("Add Equation");
    removeEquationButton = new QPushButton("Remove Equation");
    removeEquationButton->setEnabled(false);
    buttonLayout->addWidget(addEquationButton);
    buttonLayout->addWidget(removeEquationButton);

    // Equations list
    QGroupBox *listGroup = new QGroupBox("Equations");
    QVBoxLayout *listLayout = new QVBoxLayout(listGroup);
    equationsList = new QListWidget();
    listLayout->addWidget(equationsList);
    connect(equationsList, &QListWidget::itemDoubleClicked,
            this, &PlotterMainWindow::onEquationDoubleClicked);

    // Equation properties (visibility, color, line width)
    QGroupBox *propsGroup = new QGroupBox("Properties");
    QVBoxLayout *propsLayout = new QVBoxLayout(propsGroup);

    // Visibility checkbox
    visibilityCheckBox = new QCheckBox("Visible");
    visibilityCheckBox->setChecked(true);
    visibilityCheckBox->setEnabled(false);
    propsLayout->addWidget(visibilityCheckBox);

    // Color selection
    QHBoxLayout *colorLayout = new QHBoxLayout();
    colorLayout->addWidget(new QLabel("Color:"));
    colorButton = new QPushButton();
    colorButton->setEnabled(false);
    colorButton->setMinimumWidth(60);
    colorLayout->addWidget(colorButton);
    propsLayout->addLayout(colorLayout);

    // Line width
    QHBoxLayout *widthLayout = new QHBoxLayout();
    widthLayout->addWidget(new QLabel("Line Width:"));
    lineWidthSpinBox = new QDoubleSpinBox();
    lineWidthSpinBox->setRange(0.5, 10.0);
    lineWidthSpinBox->setSingleStep(0.5);
    lineWidthSpinBox->setValue(2.0);
    lineWidthSpinBox->setEnabled(false);
    widthLayout->addWidget(lineWidthSpinBox);
    propsLayout->addLayout(widthLayout);

    // Plot range controls
    QGroupBox *rangeGroup = new QGroupBox("Plot Range");
    QGridLayout *rangeLayout = new QGridLayout(rangeGroup);

    rangeLayout->addWidget(new QLabel("X Min:"), 0, 0);
    xMinSpinBox = new QDoubleSpinBox();
    xMinSpinBox->setRange(-1000, 1000);
    xMinSpinBox->setValue(-10);
    rangeLayout->addWidget(xMinSpinBox, 0, 1);

    rangeLayout->addWidget(new QLabel("X Max:"), 0, 2);
    xMaxSpinBox = new QDoubleSpinBox();
    xMaxSpinBox->setRange(-1000, 1000);
    xMaxSpinBox->setValue(10);
    rangeLayout->addWidget(xMaxSpinBox, 0, 3);

    rangeLayout->addWidget(new QLabel("Y Min:"), 1, 0);
    yMinSpinBox = new QDoubleSpinBox();
    yMinSpinBox->setRange(-1000, 1000);
    yMinSpinBox->setValue(-10);
    rangeLayout->addWidget(yMinSpinBox, 1, 1);

    rangeLayout->addWidget(new QLabel("Y Max:"), 1, 2);
    yMaxSpinBox = new QDoubleSpinBox();
    yMaxSpinBox->setRange(-1000, 1000);
    yMaxSpinBox->setValue(10);
    rangeLayout->addWidget(yMaxSpinBox, 1, 3);

    rangeLayout->addWidget(new QLabel("Points:"), 2, 0);
    pointsSpinBox = new QSpinBox();
    pointsSpinBox->setRange(10, 5000);
    pointsSpinBox->setValue(1000);
    rangeLayout->addWidget(pointsSpinBox, 2, 1);

    // Plot appearance group
    QGroupBox *appearanceGroup = new QGroupBox("Plot Appearance");
    QGridLayout *appearanceLayout = new QGridLayout(appearanceGroup);

    // Plot title
    appearanceLayout->addWidget(new QLabel("Title:"), 0, 0);
    plotTitleInput = new QLineEdit("Function Plot");  // Default title
    appearanceLayout->addWidget(plotTitleInput, 0, 1, 1, 3);  // Span 3 columns

    // Background color
    appearanceLayout->addWidget(new QLabel("Background:"), 1, 0);
    bgColorButton = new QPushButton();
    bgColorButton->setStyleSheet("background-color: #000000");  // Black
    bgColorButton->setMinimumWidth(60);
    appearanceLayout->addWidget(bgColorButton, 1, 1);

    // Text color
    appearanceLayout->addWidget(new QLabel("Text Color:"), 1, 2);
    textColorButton = new QPushButton();
    textColorButton->setStyleSheet("background-color: #FFFFFF");  // White
    textColorButton->setMinimumWidth(60);
    appearanceLayout->addWidget(textColorButton, 1, 3);

    // Add the appearance group to the controls layout
    controlsLayout->addWidget(appearanceGroup);

    // Plot controls
    QHBoxLayout *plotButtonsLayout = new QHBoxLayout();
    generatePlotButton = new QPushButton("Generate Plot");
    clearPlotButton = new QPushButton("Clear Plot");
    plotButtonsLayout->addWidget(generatePlotButton);
    plotButtonsLayout->addWidget(clearPlotButton);
    // Save Plot as Image Button
    saveImageButton = new QPushButton("Save Plot as Image", controlsPanel);

    // Add all controls to the left panel
    controlsLayout->addWidget(nameGroup);
    controlsLayout->addWidget(equationGroup);
    controlsLayout->addLayout(buttonLayout);
    controlsLayout->addWidget(listGroup);
    controlsLayout->addWidget(propsGroup);
    controlsLayout->addWidget(rangeGroup);
    controlsLayout->addLayout(plotButtonsLayout);
    controlsLayout->addWidget(saveImageButton);
    controlsLayout->addStretch();

    // Create the chart view
    chart = new QChart();
    chart->setTitle("Function Plot");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Set default background color to black
    chart->setBackgroundVisible(true);
    chart->setBackgroundBrush(QBrush(QColor(0, 0, 0)));

    // Set default text color to white
    chart->setTitleBrush(QBrush(QColor(255, 255, 255)));

    axisX = new QValueAxis();
    axisX->setTitleText("X");
    axisX->setRange(-10, 10);
    axisX->setLabelsBrush(QBrush(QColor(255, 255, 255))); // White labels
    axisX->setTitleBrush(QBrush(QColor(255, 255, 255)));  // White title
    chart->addAxis(axisX, Qt::AlignBottom);

    axisY = new QValueAxis();
    axisY->setTitleText("Y");
    axisY->setRange(-10, 10);
    axisY->setLabelsBrush(QBrush(QColor(255, 255, 255))); // White labels
    axisY->setTitleBrush(QBrush(QColor(255, 255, 255)));  // White title
    chart->addAxis(axisY, Qt::AlignLeft);

    // Set legend text color
    chart->legend()->setLabelBrush(QBrush(QColor(255, 255, 255)));

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumWidth(600);

    // Make the chart background transparent
    chartView->setStyleSheet("background: transparent;");

    // Add widgets to content layout
    contentLayout->addWidget(controlsPanel, 1);
    contentLayout->addWidget(chartView, 3);

    // Add content area to main layout
    mainLayout->addWidget(contentWidget, 1);

    // Connect signals
    connect(closeButton, &QPushButton::clicked, this, &QMainWindow::close);
    connect(minimizeButton, &QPushButton::clicked, this, &QMainWindow::showMinimized);
    connect(addEquationButton, &QPushButton::clicked, this, &PlotterMainWindow::onAddEquationClicked);
    connect(removeEquationButton, &QPushButton::clicked, this, &PlotterMainWindow::onRemoveEquationClicked);
    connect(generatePlotButton, &QPushButton::clicked, this, &PlotterMainWindow::onGeneratePlotClicked);
    connect(clearPlotButton, &QPushButton::clicked, this, &PlotterMainWindow::onClearPlotClicked);
    connect(equationsList, &QListWidget::currentRowChanged, this, &PlotterMainWindow::onEquationSelectionChanged);
    connect(visibilityCheckBox, &QCheckBox::stateChanged, this, &PlotterMainWindow::onEquationVisibilityChanged);
    connect(colorButton, &QPushButton::clicked, this, &PlotterMainWindow::onEquationColorChanged);
    connect(lineWidthSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PlotterMainWindow::onLineWidthChanged);
    connect(saveImageButton, &QPushButton::clicked, this, &PlotterMainWindow::onSavePlotAsImageClicked);
    connect(bgColorButton, &QPushButton::clicked, [this]() {
        // Get current background color
        QColor currentColor = chart->isBackgroundVisible() ?
                                  chart->backgroundBrush().color() : Qt::white;

        QColor newColor = QColorDialog::getColor(currentColor, this,
                                                 "Select Chart Background Color");

        if (newColor.isValid()) {
            // Update background color
            chart->setBackgroundVisible(true);
            chart->setBackgroundBrush(QBrush(newColor));

            // Update button appearance
            QString colorStyle = QString("background-color: %1").arg(newColor.name());
            bgColorButton->setStyleSheet(colorStyle);
        }
    });
    connect(textColorButton, &QPushButton::clicked, [this]() {
        // Get current text color from title
        QColor currentColor = chart->titleBrush().color();

        QColor newColor = QColorDialog::getColor(currentColor, this,
                                                 "Select Chart Text Color");

        if (newColor.isValid()) {
            // Update text colors
            chart->setTitleBrush(QBrush(newColor));

            // Update axis labels color
            axisX->setLabelsBrush(QBrush(newColor));
            axisY->setLabelsBrush(QBrush(newColor));

            // Update axis titles color
            axisX->setTitleBrush(QBrush(newColor));
            axisY->setTitleBrush(QBrush(newColor));

            // Update legend text color
            chart->legend()->setLabelBrush(QBrush(newColor));

            // Update button appearance
            QString colorStyle = QString("background-color: %1").arg(newColor.name());
            textColorButton->setStyleSheet(colorStyle);
        }
    });
}

// Note, this paintEvent is key to getting the rounded corners and custom border
void PlotterMainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 1) Clear the entire window (fully transparent)
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.fillRect(rect(), Qt::transparent);

    // 2) Switch back to normal composition
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    // Create a slightly inset rect to accommodate the border
    int borderWidth = 10; // Your desired border width
    QRect insetRect = rect().adjusted(borderWidth/2, borderWidth/2, -borderWidth/2, -borderWidth/2);

    // 3) Draw a rounded rectangle with a thick border
    int radius = 15; // The corner radius
    QPainterPath path;
    path.addRoundedRect(insetRect, radius, radius);

    // Set up the painter
    painter.setBrush(QColor(45, 45, 48)); // Background color
    painter.setPen(QPen(Qt::black, borderWidth)); // Border

    // Draw the rounded rectangle
    painter.drawPath(path);
}

// Window frame drag handling
void PlotterMainWindow::mousePressEvent(QMouseEvent *event)
{
    // For title bar dragging
    if (event->position().y() <= 40) {
        m_dragging = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
        return;
    }

    // Resize area
    const int resizeMargin = 20;

    // Check if mouse is in any resize area
    const QRect resizeArea(rect().x(), rect().y(), rect().width(), rect().height());

    // Check different zones
    bool onLeft = event->position().x() <= resizeMargin;
    bool onRight = event->position().x() >= width() - resizeMargin;
    bool onTop = event->position().y() <= resizeMargin;
    bool onBottom = event->position().y() >= height() - resizeMargin;

    // Store the resize mode
    if (event->button() == Qt::LeftButton) {
        if (onRight && onBottom) {
            // Bottom-right corner
            m_resizeMode = ResizeMode::BottomRight;
            setCursor(Qt::SizeFDiagCursor);
            m_resizing = true;
        }
        else if (onLeft && onBottom) {
            // Bottom-left corner
            m_resizeMode = ResizeMode::BottomLeft;
            setCursor(Qt::SizeBDiagCursor);
            m_resizing = true;
        }
        else if (onRight && onTop) {
            // Top-right corner
            m_resizeMode = ResizeMode::TopRight;
            setCursor(Qt::SizeBDiagCursor);
            m_resizing = true;
        }
        else if (onLeft && onTop) {
            // Top-left corner
            m_resizeMode = ResizeMode::TopLeft;
            setCursor(Qt::SizeFDiagCursor);
            m_resizing = true;
        }
        else if (onRight) {
            // Right edge
            m_resizeMode = ResizeMode::Right;
            setCursor(Qt::SizeHorCursor);
            m_resizing = true;
        }
        else if (onBottom) {
            // Bottom edge
            m_resizeMode = ResizeMode::Bottom;
            setCursor(Qt::SizeVerCursor);
            m_resizing = true;
        }
        else if (onLeft) {
            // Left edge
            m_resizeMode = ResizeMode::Left;
            setCursor(Qt::SizeHorCursor);
            m_resizing = true;
        }
        else if (onTop) {
            // Top edge (only if not in title bar area)
            if (event->position().y() > 40) {
                m_resizeMode = ResizeMode::Top;
                setCursor(Qt::SizeVerCursor);
                m_resizing = true;
            }
        }

        if (m_resizing) {
            m_resizeStartPos = event->globalPosition().toPoint();
            m_initialRect = rect();
            event->accept();
        }
    }
}

void PlotterMainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
        return;
    }

    if (m_resizing) {
        QPoint globalPos = event->globalPosition().toPoint();
        QRect currentGeometry = geometry();

        // Handle resize based on mode
        switch (m_resizeMode) {
        case ResizeMode::BottomRight:
            // Only change width and height
            currentGeometry.setWidth(globalPos.x() - currentGeometry.left());
            currentGeometry.setHeight(globalPos.y() - currentGeometry.top());
            break;

        case ResizeMode::BottomLeft:
            // Change x, width, and height
            {
                int newWidth = currentGeometry.right() - globalPos.x();
                int newX = globalPos.x();
                currentGeometry.setLeft(newX);
                currentGeometry.setHeight(globalPos.y() - currentGeometry.top());
            }
            break;

        case ResizeMode::TopRight:
            // Change y, height, and width
            {
                int newHeight = currentGeometry.bottom() - globalPos.y();
                int newY = globalPos.y();
                currentGeometry.setTop(newY);
                currentGeometry.setWidth(globalPos.x() - currentGeometry.left());
            }
            break;

        case ResizeMode::TopLeft:
            // Change x, y, width, and height
            {
                int newWidth = currentGeometry.right() - globalPos.x();
                int newHeight = currentGeometry.bottom() - globalPos.y();
                int newX = globalPos.x();
                int newY = globalPos.y();
                currentGeometry.setLeft(newX);
                currentGeometry.setTop(newY);
            }
            break;

        case ResizeMode::Right:
            // Only change width
            currentGeometry.setWidth(globalPos.x() - currentGeometry.left());
            break;

        case ResizeMode::Bottom:
            // Only change height
            currentGeometry.setHeight(globalPos.y() - currentGeometry.top());
            break;

        case ResizeMode::Left:
            // Change x and width
            {
                int newWidth = currentGeometry.right() - globalPos.x();
                int newX = globalPos.x();
                currentGeometry.setLeft(newX);
            }
            break;

        case ResizeMode::Top:
            // Change y and height
            {
                int newHeight = currentGeometry.bottom() - globalPos.y();
                int newY = globalPos.y();
                currentGeometry.setTop(newY);
            }
            break;

        default:
            break;
        }

        // Apply minimum size constraints
        int minWidth = minimumWidth();
        int minHeight = minimumHeight();

        if (currentGeometry.width() < minWidth) {
            // Maintain position when applying minimum width
            if (m_resizeMode == ResizeMode::Left || m_resizeMode == ResizeMode::TopLeft || m_resizeMode == ResizeMode::BottomLeft) {
                currentGeometry.setLeft(currentGeometry.right() - minWidth);
            } else {
                currentGeometry.setWidth(minWidth);
            }
        }

        if (currentGeometry.height() < minHeight) {
            // Maintain position when applying minimum height
            if (m_resizeMode == ResizeMode::Top || m_resizeMode == ResizeMode::TopLeft || m_resizeMode == ResizeMode::TopRight) {
                currentGeometry.setTop(currentGeometry.bottom() - minHeight);
            } else {
                currentGeometry.setHeight(minHeight);
            }
        }

        // Apply the new geometry
        setGeometry(currentGeometry);
        event->accept();
        return;
    }

    // Update cursor based on position
    const int resizeMargin = 20;
    bool onLeft = event->position().x() <= resizeMargin;
    bool onRight = event->position().x() >= width() - resizeMargin;
    bool onTop = event->position().y() <= resizeMargin;
    bool onBottom = event->position().y() >= height() - resizeMargin;

    // Skip cursor changes in title bar area
    if (onTop && event->position().y() <= 40) {
        setCursor(Qt::ArrowCursor);
        return;
    }

    // Set cursor based on position
    if ((onLeft && onTop) || (onRight && onBottom)) {
        setCursor(Qt::SizeFDiagCursor);
    } else if ((onRight && onTop) || (onLeft && onBottom)) {
        setCursor(Qt::SizeBDiagCursor);
    } else if (onLeft || onRight) {
        setCursor(Qt::SizeHorCursor);
    } else if (onTop || onBottom) {
        setCursor(Qt::SizeVerCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

void PlotterMainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    m_dragging = false;
    m_resizing = false;
    setCursor(Qt::ArrowCursor);
    event->accept();
}

void PlotterMainWindow::onAddEquationClicked()
{
    QString name = equationNameInput->text().trimmed();
    QString equation = equationInput->text().trimmed();

    // Validate inputs
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Missing Name", "Please enter a name for the equation.");
        return;
    }

    if (equation.isEmpty()) {
        QMessageBox::warning(this, "Missing Equation", "Please enter an equation to plot.");
        return;
    }

    // Check for duplicate names
    for (const EquationPlot &plot : plots) {
        if (plot.name == name) {
            QMessageBox::warning(this, "Duplicate Name", "An equation with this name already exists.");
            return;
        }
    }

    // Create a new equation
    EquationPlot newPlot;
    newPlot.name = name;
    newPlot.equation = equation;
    newPlot.lineWidth = lineWidthSpinBox->value();

    // Assign a color from a predefined list
    QList<QColor> colors = {
        QColor(255, 0, 0),     // Red
        QColor(0, 0, 255),     // Blue
        QColor(0, 128, 0),     // Green
        QColor(255, 165, 0),   // Orange
        QColor(128, 0, 128),   // Purple
        QColor(0, 128, 128),   // Teal
        QColor(255, 0, 255),   // Magenta
        QColor(128, 128, 0)    // Olive
    };

    newPlot.color = colors[plots.size() % colors.size()];

    // Add to lists
    plots.append(newPlot);
    equationsList->addItem(name);

    // Select the new equation
    equationsList->setCurrentRow(plots.size() - 1);

    // Clear inputs
    equationNameInput->clear();
    equationInput->clear();
}

void PlotterMainWindow::onRemoveEquationClicked()
{
    int currentRow = equationsList->currentRow();
    if (currentRow >= 0 && currentRow < plots.size()) {
        // Remove the plot
        delete equationsList->takeItem(currentRow);
        plots.removeAt(currentRow);

        // Update controls
        if (plots.isEmpty()) {
            removeEquationButton->setEnabled(false);
            visibilityCheckBox->setEnabled(false);
            colorButton->setEnabled(false);
            lineWidthSpinBox->setEnabled(false);
        } else {
            // Select another equation
            if (currentRow >= plots.size()) {
                equationsList->setCurrentRow(plots.size() - 1);
            } else {
                equationsList->setCurrentRow(currentRow);
            }
        }

        // Update plot if needed
        if (plots.isEmpty()) {
            onClearPlotClicked();
        } else {
            onGeneratePlotClicked();
        }
    }
}

void PlotterMainWindow::onGeneratePlotClicked()
{
    // Update title
    chart->setTitle(plotTitleInput->text());

    if (plots.isEmpty()) {
        QMessageBox::warning(this, "No Equations", "Please add at least one equation to plot.");
        return;
    }

    // Remove all existing series
    chart->removeAllSeries();

    // Get plot ranges
    double xMin = xMinSpinBox->value();
    double xMax = xMaxSpinBox->value();
    double yMin = yMinSpinBox->value();
    double yMax = yMaxSpinBox->value();

    // Validate ranges
    if (xMin >= xMax) {
        QMessageBox::warning(this, "Invalid Range", "X Min must be less than X Max.");
        return;
    }

    if (yMin >= yMax) {
        QMessageBox::warning(this, "Invalid Range", "Y Min must be less than Y Max.");
        return;
    }

    // Update axis ranges
    axisX->setRange(xMin, xMax);
    axisY->setRange(yMin, yMax);

    // Number of points to generate
    int numPoints = pointsSpinBox->value();
    double step = (xMax - xMin) / (numPoints - 1);

    // Generate each plot
    for (int i = 0; i < plots.size(); i++) {
        EquationPlot &plot = plots[i];

        // Skip if not visible
        if (!plot.visible) continue;

        // Create a new series
        QLineSeries *series = new QLineSeries();
        series->setName(plot.name);
        series->setColor(plot.color);
        series->setPen(QPen(plot.color, plot.lineWidth));

        // Store the series for later updates
        plot.series = series;

        // Generate points
        bool validPoints = false;
        for (int j = 0; j < numPoints; j++) {
            double x = xMin + j * step;

            try {
                double y = evaluateExpression(plot.equation, x);

                // Only add valid points within y-range
                if (std::isfinite(y) && y >= yMin && y <= yMax) {
                    series->append(x, y);
                    validPoints = true;
                }
            } catch (...) {
                // Skip errors and continue with other points
            }
        }

        // Add the series to the chart if it has valid points
        if (validPoints) {
            chart->addSeries(series);
            series->attachAxis(axisX);
            series->attachAxis(axisY);
        } else {
            delete series;
            plot.series = nullptr;
            QMessageBox::warning(this, "Plot Error",
                                 "No valid points found for equation '" + plot.name +
                                     "'. Check your equation and axis ranges.");
        }
    }
}

void PlotterMainWindow::onClearPlotClicked()
{
    chart->removeAllSeries();

    // Reset the series pointers
    for (EquationPlot &plot : plots) {
        plot.series = nullptr;
    }
}

void PlotterMainWindow::onEquationSelectionChanged()
{
    int currentRow = equationsList->currentRow();
    bool hasSelection = (currentRow >= 0 && currentRow < plots.size());

    // Enable/disable buttons based on selection
    removeEquationButton->setEnabled(hasSelection);
    visibilityCheckBox->setEnabled(hasSelection);
    colorButton->setEnabled(hasSelection);
    lineWidthSpinBox->setEnabled(hasSelection);

    // Update controls based on selection
    if (hasSelection) {
        EquationPlot &plot = plots[currentRow];

        // Update visibility checkbox
        visibilityCheckBox->setChecked(plot.visible);

        // Update color button
        QString colorStyle = QString("background-color: %1").arg(plot.color.name());
        colorButton->setStyleSheet(colorStyle);

        // Update line width
        lineWidthSpinBox->setValue(plot.lineWidth);
    }
}

void PlotterMainWindow::onEquationVisibilityChanged(int state)
{
    int currentRow = equationsList->currentRow();
    if (currentRow >= 0 && currentRow < plots.size()) {
        plots[currentRow].visible = (state == Qt::Checked);
        onGeneratePlotClicked(); // Update the plot
    }
}

void PlotterMainWindow::onEquationColorChanged()
{
    int currentRow = equationsList->currentRow();
    if (currentRow >= 0 && currentRow < plots.size()) {
        QColor newColor = QColorDialog::getColor(plots[currentRow].color, this, "Select Equation Color");

        if (newColor.isValid()) {
            // Update the color
            plots[currentRow].color = newColor;

            // Update the color button
            QString colorStyle = QString("background-color: %1").arg(newColor.name());
            colorButton->setStyleSheet(colorStyle);

            // Update the plot
            onGeneratePlotClicked();
        }
    }
}

void PlotterMainWindow::onLineWidthChanged(double width)
{
    int currentRow = equationsList->currentRow();
    if (currentRow >= 0 && currentRow < plots.size()) {
        plots[currentRow].lineWidth = width;
        onGeneratePlotClicked(); // Update the plot
    }
}

double PlotterMainWindow::evaluateExpression(const QString &expression, double x)
{
    // Replace x with its value
    QString jsExpression = expression;
    jsExpression.replace("x", QString::number(x));

    // Add support for common mathematical functions and constants
    static bool engineInitialized = false;
    if (!engineInitialized) {
        // Define Math library functions
        jsEngine.evaluate(
            "function sin(x) { return Math.sin(x); }"
            "function cos(x) { return Math.cos(x); }"
            "function tan(x) { return Math.tan(x); }"
            "function sqrt(x) { return Math.sqrt(x); }"
            "function abs(x) { return Math.abs(x); }"
            "function log(x) { return Math.log(x); }"  // Natural log
            "function log10(x) { return Math.log10(x); }"
            "function exp(x) { return Math.exp(x); }"
            "function pow(x, y) { return Math.pow(x, y); }"
            "var pi = Math.PI;"
            "var e = Math.E;"
            );
        engineInitialized = true;
    }

    // Handle common operations that might not be JS compatible
    jsExpression.replace("^", "**");  // Replace ^ with ** for exponentiation

    // Evaluate the expression
    QJSValue result = jsEngine.evaluate(jsExpression);

    if (result.isError()) {
        qDebug() << "Error evaluating expression:" << jsExpression;
        qDebug() << "Error:" << result.toString();
        return 0;
    }

    return result.toNumber();
}

void PlotterMainWindow::onEquationDoubleClicked(QListWidgetItem *item)
{
    int row = equationsList->row(item);
    if (row >= 0 && row < plots.size()) {
        // Get current equation
        EquationPlot &plot = plots[row];

        // Create a dialog for editing
        QDialog dialog(this);
        dialog.setWindowTitle("Edit Equation");

        QVBoxLayout *layout = new QVBoxLayout(&dialog);

        // Name field
        QLabel *nameLabel = new QLabel("Equation Name:", &dialog);
        QLineEdit *nameEdit = new QLineEdit(plot.name, &dialog);
        layout->addWidget(nameLabel);
        layout->addWidget(nameEdit);

        // Equation field
        QLabel *equationLabel = new QLabel("Equation:", &dialog);
        QLineEdit *equationEdit = new QLineEdit(plot.equation, &dialog);
        layout->addWidget(equationLabel);
        layout->addWidget(equationEdit);

        // Buttons
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        QPushButton *cancelButton = new QPushButton("Cancel", &dialog);
        QPushButton *saveButton = new QPushButton("Save", &dialog);
        saveButton->setDefault(true);

        buttonLayout->addWidget(cancelButton);
        buttonLayout->addWidget(saveButton);
        layout->addLayout(buttonLayout);

        // Connect buttons
        connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);
        connect(saveButton, &QPushButton::clicked, &dialog, &QDialog::accept);

        // Show dialog and process result
        if (dialog.exec() == QDialog::Accepted) {
            QString newName = nameEdit->text().trimmed();
            QString newEquation = equationEdit->text().trimmed();

            // Validate inputs
            if (newName.isEmpty()) {
                QMessageBox::warning(this, "Invalid Name", "Equation name cannot be empty.");
                return;
            }

            if (newEquation.isEmpty()) {
                QMessageBox::warning(this, "Invalid Equation", "Equation cannot be empty.");
                return;
            }

            // Check for duplicate names (except for this equation)
            for (int i = 0; i < plots.size(); i++) {
                if (i != row && plots[i].name == newName) {
                    QMessageBox::warning(this, "Duplicate Name",
                                         "An equation with this name already exists.");
                    return;
                }
            }

            // Update the equation
            plot.name = newName;
            plot.equation = newEquation;

            // Update the list item
            item->setText(newName);

            // Regenerate the plot
            onGeneratePlotClicked();
        }
    }
}

void PlotterMainWindow::onSavePlotAsImageClicked()
{
    // Open a file dialog for the user to select where to save the image
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Save Plot as Image",
                                                    "",
                                                    "PNG Image (*.png);;JPEG Image (*.jpg *.jpeg)");
    if (!fileName.isEmpty()) {
        // Grab the chart view's current display as a pixmap
        QPixmap pixmap = chartView->grab();

        // Attempt to save the pixmap to the chosen file
        if (!pixmap.save(fileName)) {
            QMessageBox::warning(this, "Save Error", "Failed to save the image.");
        }
    }
}
