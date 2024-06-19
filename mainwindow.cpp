#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    currentTreeMode = TreeModes::AVL;

    WindowWidth = 1111;
    WindowHeight = 711;

    BST_GraphicsScene = new QGraphicsScene(this);
    BST_GraphicsScene->setSceneRect(QRectF(0, 0, WindowWidth, WindowHeight));

    ui->BST_GraphicsView->setScene(BST_GraphicsScene);

    connect(ui->setAVL, &QAction::triggered,
            this, [this]() {
        setCurrentTreeMode(TreeModes::AVL);
        BST_GraphicsScene->clear();
        auto info = AVL_tree.getInfo();
        draw_tree(info);
    });

    connect(ui->setRed_Black, &QAction::triggered,
            this, [this]() {
        setCurrentTreeMode(TreeModes::REDBLACK);
        BST_GraphicsScene->clear();
        auto info = REDBLACK_tree.getInfo();
        draw_tree(info);
    });

    connect(ui->setSplay, &QAction::triggered,
            this, [this]() {
        setCurrentTreeMode(TreeModes::SPLAY);
        BST_GraphicsScene->clear();
        auto info = SPLAY_tree.getInfo();
        draw_tree(info);
    });

    connect(ui->setTreap, &QAction::triggered,
            this, [this]() {
        setCurrentTreeMode(TreeModes::TREAP);
        BST_GraphicsScene->clear();
        auto info = TREAP_tree.getInfo();
        draw_tree(info);
    });

    connect(ui->InsertValueButton, &QPushButton::clicked,
            this, &MainWindow::insert_value);

    connect(ui->clearTreeButton, &QPushButton::clicked,
            this, &MainWindow::clear_tree_button);

    connect(ui->InsertRandomButton, &QPushButton::clicked,
            this, &MainWindow::insert_random_values);

    connect(ui->RemoveElementButton, &QPushButton::clicked,
            this, &MainWindow::remove_element_by_value);

    connect(ui->ScopeSlider, &QSlider::valueChanged,
            this, &MainWindow::setScale);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::draw_tree(std::vector<info_struct> &info)
{
    BST_GraphicsScene->clear();

    if (info.empty())
        return;

    QString MaxEl, MinEl, maxNumLen;
    QFontMetrics fontMetrics(font);
    qreal nodeRadius;
    int height = -1;

    switch (getCurrentTreeMode())
    {
        case TreeModes::AVL:
            if (AVL_tree.height() == 0)
                return;

            MaxEl = QString::number(AVL_tree.getMax());
            MinEl = QString::number(AVL_tree.getMin());

            maxNumLen = (MaxEl.size() > MinEl.size()) ? MaxEl : MinEl;
            nodeRadius = (fontMetrics.horizontalAdvance(maxNumLen) + 1) / 2 + 5;

            height = AVL_tree.height();
            WindowWidth = AVL_tree.getLen(nodeRadius) + 100;

            break;
        case TreeModes::REDBLACK:
            if (REDBLACK_tree.height() == 0)
                return;

            MaxEl = QString::number(REDBLACK_tree.getMax());
            MinEl = QString::number(REDBLACK_tree.getMin());

            maxNumLen = (MaxEl.size() > MinEl.size()) ? MaxEl : MinEl;
            nodeRadius = (fontMetrics.horizontalAdvance(maxNumLen) + 1) / 2 + 5;

            WindowWidth = REDBLACK_tree.getLen(nodeRadius) + 100;
            height = REDBLACK_tree.height();

            break;
        case TreeModes::SPLAY:
            if (SPLAY_tree.height() == 0)
                return;

            MaxEl = QString::number(SPLAY_tree.getMax());
            MinEl = QString::number(SPLAY_tree.getMin());

            maxNumLen = (MaxEl.size() > MinEl.size()) ? MaxEl : MinEl;
            nodeRadius = (fontMetrics.horizontalAdvance(maxNumLen) + 1) / 2 + 5;

            WindowWidth = SPLAY_tree.getLen(nodeRadius) + 100;
            height = SPLAY_tree.height();

            break;
        case TreeModes::TREAP:
            if (TREAP_tree.height() == 0)
                return;

            QString maxLenStr = "";
            for (quint32 ind = 0; ind < info.size(); ++ind)
            {
                if (fontMetrics.horizontalAdvance(maxLenStr) <
                        fontMetrics.horizontalAdvance(QString::number(info[ind].value) + " " + QString::number(info[ind].extra)))
                {
                    maxLenStr = QString::number(info[ind].value) + " " + QString::number(info[ind].extra);
                }
            }

            nodeRadius = (fontMetrics.horizontalAdvance(maxLenStr) + 1) / 2 + 5;

            WindowWidth = TREAP_tree.getLen(nodeRadius) + 100;
            height = TREAP_tree.height();

            break;
    }

    auto get_props = [](info_struct& info, double len) -> double
    {
        int prop_left = (info.left_size == 0) ? 1 : info.left_size;
        int prop_right = (info.right_size == 0) ? 1 : info.right_size;
        int prop_all = prop_left + prop_right;
        return len * prop_left / prop_all;
    };


    WindowHeight = 3 * nodeRadius * height + 20;


    BST_GraphicsScene->setSceneRect(0, 0, WindowWidth, WindowHeight);

    struct coordinates {
        double left_x, right_x, x, y;
        int64_t value;
        int64_t low_bord = -9223372036854775807; // -(2^63 - 1)
        int64_t high_bord = 9223372036854775807; // (2^63 - 1)
        coordinates() {}
        coordinates(int _left_x, int _right_x, int _x, int _y, int64_t _value) : left_x(_left_x), right_x(_right_x), x(_x), y(_y), value(_value) {}
        coordinates(int _left_x, int _right_x, int _x, int _y, int64_t _value,
                    int64_t _low_bord, int64_t _high_bord) :
            left_x(_left_x), right_x(_right_x), x(_x), y(_y), value(_value),
            low_bord(_low_bord), high_bord(_high_bord) {}
    };



    std::vector<coordinates> currentInds;
    for (quint32 ind = 0; ind < info.size(); ++ind) {
        if (currentInds.empty())
        {
            currentInds.emplace_back(0, WindowWidth, get_props(info[ind], WindowWidth),
                                     nodeRadius + 20, info[ind].value);
            VisualNode* newVisualNode = new VisualNode({info[ind].value, info[ind].extra}, {currentInds.front().x, nodeRadius + 20},
                                                    nodeRadius, getCurrentTreeMode(), font);
            connect(newVisualNode, &VisualNode::deleteValue,
                    this, &MainWindow::remove_element);
            BST_GraphicsScene->addItem(newVisualNode);
        }
        else
        {
            while (currentInds.back().low_bord >= info[ind].value ||
                   currentInds.back().high_bord <= info[ind].value)
            {
                currentInds.pop_back();
            }
            if (currentInds.back().value < info[ind].value)
            {
                int new_left_x, new_right_x, new_y, new_x;
                new_left_x = currentInds.back().x;
                new_right_x = currentInds.back().right_x;
                new_x = new_left_x + get_props(info[ind], new_right_x - new_left_x);
                new_y = currentInds.back().y + 3 * nodeRadius;
                qreal old_center_x = currentInds.back().x;
                qreal old_center_y = currentInds.back().y;
                qreal new_center_x = new_x;
                qreal new_center_y = new_y;
                VisualNode* newVisualNode = new VisualNode({info[ind].value, info[ind].extra}, {new_center_x, new_center_y},
                                                           nodeRadius, getCurrentTreeMode(), font);
                connect(newVisualNode, &VisualNode::deleteValue,
                        this, &MainWindow::remove_element);
                BST_GraphicsScene->addItem(newVisualNode);
                qreal x_start, y_start, x_fin, y_fin;
                x_start = old_center_x + nodeRadius;
                y_start = old_center_y;
                x_fin = new_center_x;
                y_fin = new_center_y - nodeRadius;
                BST_GraphicsScene->addItem(
                            new QGraphicsLineItem(
                                x_start, y_start,
                                x_fin, y_fin));
                currentInds.emplace_back(new_left_x, new_right_x, new_x, new_y, info[ind].value,
                                         currentInds.back().value, currentInds.back().high_bord);
            }
            else
            {
                int new_left_x, new_right_x, new_y, new_x;
                new_right_x = currentInds.back().x;
                new_left_x = currentInds.back().left_x;
                new_x = new_left_x + get_props(info[ind], new_right_x - new_left_x);
                new_y = currentInds.back().y + 3 * nodeRadius;
                qreal old_center_x = currentInds.back().x;
                qreal old_center_y = currentInds.back().y;
                qreal new_center_x = new_x;
                qreal new_center_y = new_y;
                VisualNode* newVisualNode = new VisualNode({info[ind].value, info[ind].extra}, {new_center_x, new_center_y},
                                                           nodeRadius, getCurrentTreeMode(), font);
                connect(newVisualNode, &VisualNode::deleteValue,
                        this, &MainWindow::remove_element);
                BST_GraphicsScene->addItem(newVisualNode);
                qreal x_start, y_start, x_fin, y_fin;
                x_start = old_center_x - nodeRadius;
                y_start = old_center_y;
                x_fin = new_center_x;
                y_fin = new_center_y - nodeRadius;
                BST_GraphicsScene->addItem(
                            new QGraphicsLineItem(
                                x_start, y_start,
                                x_fin, y_fin));
                currentInds.emplace_back(new_left_x, new_right_x, new_x, new_y, info[ind].value,
                                         currentInds.back().low_bord, currentInds.back().value);
            }
        }
    }
}

void MainWindow::insert_value()
{
    BST_GraphicsScene->clear();

    bool ok;
    int64_t insertVal = ui->ValueInputLineEdit->text().toLongLong(&ok);
    if (!ok)
        return;
    std::vector<info_struct> info;
    switch (getCurrentTreeMode())
    {
    case TreeModes::AVL:
    {
        AVL_tree.insert(insertVal);
        info = AVL_tree.getInfo();
        draw_tree(info);
        return;
    }
    case TreeModes::REDBLACK:
        REDBLACK_tree.insert(insertVal);
        info = REDBLACK_tree.getInfo();
        draw_tree(info);
        return;
    case TreeModes::SPLAY:
        SPLAY_tree.insert(insertVal);
        info = SPLAY_tree.getInfo();
        draw_tree(info);
        return;
    case TreeModes::TREAP:
        TREAP_tree.insert(insertVal);
        info = TREAP_tree.getInfo();
        draw_tree(info);
        return;
    }
}

void MainWindow::insert_random_values()
{
    BST_GraphicsScene->clear();

    bool ok;
    int64_t cntVals = ui->NumberOfRandomElementsLineEdit->text().toLongLong(&ok);
    if (!ok)
        return;
    std::vector<info_struct> info;
    switch (getCurrentTreeMode())
    {
    case TreeModes::AVL:
        AVL_tree.insert_random_integers(cntVals);
        info = AVL_tree.getInfo();
        draw_tree(info);
        return;
    case TreeModes::REDBLACK:
        REDBLACK_tree.insert_random_integers(cntVals);
        info = REDBLACK_tree.getInfo();
        draw_tree(info);
        return;
    case TreeModes::SPLAY:
        SPLAY_tree.insert_random_integers(cntVals);
        info = SPLAY_tree.getInfo();
        draw_tree(info);
        return;
    case TreeModes::TREAP:
        TREAP_tree.insert_random_integers(cntVals);
        info = TREAP_tree.getInfo();
        draw_tree(info);
        return;
    }
}

void MainWindow::remove_element_by_value()
{
    BST_GraphicsScene->clear();

    bool ok;
    int64_t removedVal = ui->RemoveValueLineEdit->text().toLongLong(&ok);
    if (!ok)
        return;
    remove_element(removedVal);
}

void MainWindow::remove_element(int64_t value)
{
    std::vector<info_struct> info;
    switch (getCurrentTreeMode())
    {
    case TreeModes::AVL:
        AVL_tree.remove(value);
        info = AVL_tree.getInfo();
        draw_tree(info);
        return;
    case TreeModes::REDBLACK:
        REDBLACK_tree.remove(value);
        info = REDBLACK_tree.getInfo();
        draw_tree(info);
        return;
    case TreeModes::SPLAY:
        SPLAY_tree.remove(value);
        info = SPLAY_tree.getInfo();
        draw_tree(info);
        return;
    case TreeModes::TREAP:
        TREAP_tree.remove(value);
        info = TREAP_tree.getInfo();
        draw_tree(info);
        return;
    }
}

void MainWindow::clear_tree_button()
{
    BST_GraphicsScene->clear();

    switch (getCurrentTreeMode())
    {
    case TreeModes::AVL:
        AVL_tree.clear();
        return;
    case TreeModes::REDBLACK:
        REDBLACK_tree.clear();
        return;
    case TreeModes::SPLAY:
        SPLAY_tree.clear();
        return;
    case TreeModes::TREAP:
        TREAP_tree.clear();
        return;
    }
}

void MainWindow::setScale(int fontSize)
{
    font.setPointSize(fontSize);
    std::vector<info_struct> info;
    switch (getCurrentTreeMode())
    {
    case TreeModes::AVL:
        info = AVL_tree.getInfo();
        break;
    case TreeModes::REDBLACK:
        info = REDBLACK_tree.getInfo();
        break;
    case TreeModes::SPLAY:
        info = SPLAY_tree.getInfo();
        break;
    case TreeModes::TREAP:
        info = TREAP_tree.getInfo();
        break;
    }
    draw_tree(info);
}

TreeModes MainWindow::getCurrentTreeMode() const
{
    return currentTreeMode;
}

void MainWindow::setCurrentTreeMode(TreeModes newCurrentTreeMode)
{
    currentTreeMode = newCurrentTreeMode;
    switch (newCurrentTreeMode)
    {
    case TreeModes::AVL:
        ui->CurrentTreeModeLabel->setText("AVL Tree");
        break;
    case TreeModes::REDBLACK:
        ui->CurrentTreeModeLabel->setText("Red-Black Tree");
        break;
    case TreeModes::SPLAY:
        ui->CurrentTreeModeLabel->setText("Splay Tree");
        break;
    case TreeModes::TREAP:
        ui->CurrentTreeModeLabel->setText("Cartesian Tree");
        break;
    }
}


